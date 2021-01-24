
# include "Tower.hh"
# include <maths_utils/LocationUtils.hh>
# include "Mob.hh"
# include "Locator.hh"
# include "Projectile.hh"

namespace {

  static constexpr int max_level = 20;
  static constexpr int max_level_exp_count = 2000.0f;

  int
  levelFromExperience(float exp) noexcept {
    // We use a simple strategy where the max level
    // corresponds to the maximum experience count.
    // A linear relation is used between 0 and the
    // max value and any experience larger than the
    // maximum will result in the same level.
    int lvl = static_cast<int>(std::round(max_level * exp / max_level_exp_count));
    return std::min(std::max(lvl, 0), max_level);
  }

}

namespace tdef {

  Tower::Tower(const TProps& props,
               const towers::Processes& desc):
    Block(props, towers::toString(props.type)),

    m_type(props.type),
    m_upgrades(),
    m_exp(ExperienceData{0.0f, 0}),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_attackCost(props.attackCost),

    m_minRange(props.minRange),
    m_maxRange(props.maxRange),
    m_aoeRadius(props.aoeRadius),
    m_shootAngle(props.shootAngle),
    m_projectileSpeed(props.projectileSpeed),
    m_aimSpeed(props.aimSpeed),

    m_attack(fromProps(props)),
    m_processes(desc),

    m_target(nullptr)
  {
    setService("tower");

    // Convert the upgrades to internal format.
    for (unsigned id = 0u ; id < props.upgrades.size() ; ++id) {
      UpgradeData ud;
      ud.type = props.upgrades[id];
      ud.level = 0;

      m_upgrades.push_back(ud);
    }
  }

  float
  Tower::getTotalCost() const noexcept {
    // TODO: Handle this.
    return 1.0f;
  }

  void
  Tower::upgrade(const towers::Upgrade& upgrade,
                 int level)
  {
    // First, determine whether this upgrade is possible
    // for this tower.
    unsigned id = 0u;
    while (id < m_upgrades.size() && m_upgrades[id].type != upgrade) {
      ++id;
    }

    if (id >= m_upgrades.size()) {
      log(
        "Failed to upgrade " + towers::toString(upgrade) + " in tower not defining it",
        utils::Level::Error
      );

      return;
    }

    log(
      "Upgrading " + towers::toString(upgrade) +
      " from " + std::to_string(m_upgrades[id].level) +
      " to level " + std::to_string(level),
      utils::Level::Info
    );

    // Handle the upgrade: this basically consists in
    // increasing the level of the property by `1`.
    m_upgrades[id].level = level;
  }

  void
  Tower::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // Pick and align with the target.
    if (!pickAndAlignWithTarget(info) || m_target == nullptr) {
      // Can't do anything: we didn't find a
      // target or we are not aligned with it
      // yet.
      return;
    }

    // TODO: Handle aiming speed.

    // Check whether we can attack.
    if (m_energy < m_attackCost) {
      return;
    }

    if (m_attack.damage > 0.0f) {
      log(
        "Hitting mob " + mobs::toString(m_target->getType()) +
        " at " + m_target->getPos().toString() +
        " for " + std::to_string(m_attack.damage) + " damage" +
        " (health: " + std::to_string(m_target->getHealth()) + ")",
        utils::Level::Verbose
      );
    }

    // Hit the mob with a devastating attack. Note that
    // in case the attack destroys the mob it will be
    // marked as deleted automatically. Alos, note that
    // we only want to claim the reward in case the mob
    // is not already dead.
    m_energy -= m_attackCost;
    if (m_target->isDead() || attack(info)) {
      return;
    }

    info.gold += m_target->getBounty();
    m_exp.exp += m_target->getExpReward();
    m_exp.level = levelFromExperience(m_exp.exp);

    log(
      "Killed " + mobs::toString(m_target->getType()) +
      " at " + m_target->getPos().toString() +
      ", earned " + std::to_string(m_target->getBounty()) + " coin(s)" +
      ", exp is now " + std::to_string(m_exp.exp) +
      " (level: " + std::to_string(m_exp.level) + ")" +
      " deleted: " + std::to_string(m_target->isDeleted())
    );

    // Clear the target.
    m_target.reset();
  }

  bool
  Tower::pickAndAlignWithTarget(StepInfo& info) {
    // Check whether a target is already defined.
    // If this is the case we will make sure that
    // the distance is still less than the range
    // of the tower (so that we can shoot it).
    if (m_target != nullptr) {
      float d = utils::d(m_target->getPos(), getPos());

      if (m_target->isDead()) {
        m_target.reset();
      }

      if (m_target != nullptr && m_target->isDeleted()) {
        m_target.reset();
      }

      if (d < m_minRange || d > m_maxRange) {
        m_target.reset();
      }
    }

    // The above section might have reset the
    // target: in this case we want to find a
    // new one so that we can still spend the
    // frame trying to face it.
    if (m_target == nullptr) {
      // Find the closest mob.
      towers::PickData pd;
      pd.pos = m_pos;
      pd.minRange = m_minRange;
      pd.maxRange = m_maxRange;

      m_target = m_processes.pickMob(info, pd);

      if (m_target == nullptr) {
        // No mobs are visible, nothing to do.
        return false;
      }
    }

    // We now know that a target *is* set and
    // that it lies within the range of the
    // tower. We will try to align with it:
    // this means rotating the tower until we
    // are pointing right at the target.
    float dx = m_target->getPos().x() - getPos().x();
    float dy = m_target->getPos().y() - getPos().y();

    float theta = std::atan2(dy, dx);

    // This topic was quite helpful in determining
    // the shortest angle difference to apply with
    // both values:
    // https://stackoverflow.com/questions/28036652/finding-the-shortest-distance-between-two-angles/28037434
    // `angle2` seems to be the `to` angle and the
    // `angle1` seems to be the `from` angle.
    float dif = (theta - m_orientation + 3.1415926535f);
    dif = std::fmod(dif, 6.283185307f) - 3.1415926535f;
    dif = (dif < -3.1415926535f ? dif + 6.283185307f : dif);

    float aDif = std::abs(dif);

    float achieved = std::min(aDif, m_rotationSpeed * info.elapsed);
    achieved = std::copysign(achieved, dif);

    m_orientation += achieved;

    // Clamp within the range `]-pi; pi]`.
    float dp = std::fmod(m_orientation + 3.1415926535f, 6.283185307f);
    if (dp < 0) {
      m_orientation += 6.283185307;
    }

    float p = std::fmod(m_orientation - 3.1415926535f, 3.1415926535f);
    if (p > 0) {
      m_orientation -= 6.283185307f;
    }

    // The rotation part has been handled, which
    // means that we are now facing the target as
    // best as we can for this frame. In order to
    // shot at it we need to determine whether it
    // lies within the firing cone.
    return std::abs(m_orientation - theta) <= m_shootAngle;
  }

  bool
  Tower::attack(StepInfo& info) {
    // We have to distinguish between two main cases:
    //   - the tower has an infinite projectile speed.
    //   - the tower should shoot projectiles.
    // We could theoretically create a projectile even
    // in the first case but we figured it would just
    // overload the simulation with useless objects.

    // Case of an infinite projectile speed.
    if (hasInfiniteProjectileSpeed(m_projectileSpeed)) {
      return m_processes.damage(info, m_target, m_attack);
    }

    // Otherwise we need to create a projectile.
    Projectile::PProps pp = Projectile::newProps(getPos(), getOwner());
    pp.speed = m_projectileSpeed;

    pp.damage = m_attack.damage;
    pp.aoeRadius = m_aoeRadius;

    pp.accuracy = m_attack.accuracy;

    pp.freezePercent = m_attack.speed;
    pp.freezeSpeed = m_attack.slowdown;

    pp.stunProb = m_attack.stunProb;

    pp.freezeDuration = m_attack.fDuration;
    pp.stunDuration = m_attack.sDuration;
    pp.poisonDuration = m_attack.pDuration;

    info.spawnProjectile(std::make_shared<Projectile>(pp, m_target));

    // Consider that the projectile won't kill
    // the mob. This is probably false because
    // it means that a tower would fire more
    // projectiles that needed (as the mob's
    // health does not reflect the health and
    // the damage from flying projectiles) but
    // handling this would require the mob to
    // somehow provide a method to get the
    // health where all projectiles directed
    // towards a mob and all the ones that are
    // exploding within the aoe. Quite hard
    // to do right now.
    return true;
  }

}
