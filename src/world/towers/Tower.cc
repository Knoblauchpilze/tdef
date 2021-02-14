
# include "Tower.hh"
# include <maths_utils/LocationUtils.hh>
# include <maths_utils/ComparisonUtils.hh>
# include "Mob.hh"
# include "Locator.hh"
# include "Projectile.hh"
# include "TowerData.hh"
# include "TowerFactory.hh"

namespace {

  // Defines the maximum experience level reachable
  // by a tower.
  constexpr int max_level = 20;

  // Defines the experience points to gain to reach
  // the maximum level.
  constexpr int max_level_exp_count = 2000.0f;

  // Defines the width of the aiming cone at the
  // beginning of the aiming process. This value
  // is progressively decreased to `0` while the
  // tower is aiming.
  constexpr float init_aiming_cone = 3.1415926535f;

  int
  levelFromExperience(float exp) noexcept {
    // We use a simple strategy where the max level
    // corresponds to the maximum experience count.
    // A linear relation is used between 0 and the
    // max value and any experience larger than the
    // maximum will result in the same level.
    int lvl = static_cast<int>(std::round(max_level * exp / max_level_exp_count));
    return utils::clamp(lvl, 0, max_level);
  }

  utils::Duration
  aimDuration(float aimSpeed) noexcept {
    // Handle instantaneous aiming speed.
    if (aimSpeed == tdef::towers::infinite_aim_speed) {
      return utils::Duration::zero();
    }

    // Otherwise the aiming speed is used
    // to compute a ratio giving the time
    // to aim.
    float secs = 100.0f / aimSpeed;

    return utils::toMilliseconds(static_cast<int>(std::round(1000.0f * secs)));
  }

}

namespace tdef {

  Tower::Tower(const TProps& props):
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
    m_rotationSpeed(props.rotationSpeed),
    m_targetMode(props.targetting),
    m_persistTargets(props.persistTargets),

    m_shooting(
      ShootingData{
        props.shootAngle,
        props.projectileSpeed,
        props.aimSpeed,
        props.acceleration,
        false,
        utils::now(),
        0.0f,
        utils::now()
      }
    ),

    m_attack(fromProps(props)),
    m_processes(towers::generateData(m_type)),

    // No targets at first.
    m_targets()
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
    // The total cost is the initial cost plus the
    // cost of each upgrade.
    float cost = towers::getCost(getType(), m_exp.level);

    for (unsigned id = 0u ; id < m_upgrades.size() ; ++id) {
      float uc = 0.0f;

      for (int lvl = 0 ; lvl < m_upgrades[id].level ; ++lvl) {
        uc += towers::getUpgradeCost(getType(), m_upgrades[id].type, lvl);
      }

      cost += uc;
    }

    log("Tower cost in total " + std::to_string(cost), utils::Level::Verbose);

    return cost;
  }

  void
  Tower::gainExp(float exp) noexcept {
    // Gain the experience.
    m_exp.exp += exp;

    // And update the level.
    m_exp.level = levelFromExperience(m_exp.exp);

    log(
      "Tower gained " + std::to_string(exp) +
      " xp to reach " + std::to_string(m_exp.exp) +
      " and level " + std::to_string(m_exp.level),
      utils::Level::Verbose
    );
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

  std::istream&
  Tower::operator>>(std::istream& in) {
    Block::operator>>(in);

    int i;
    in >> i;
    m_type = static_cast<towers::Type>(i);

    // Upgrades.
    unsigned count;
    in >> count;
    for (unsigned id = 0u ; id < count ; ++id) {
      int tt;
      in >> tt;
      float level;
      in >> level;

      UpgradeData it;
      it.type = static_cast<towers::Upgrade>(tt);
      it.level = level;

      m_upgrades.push_back(it);
    }

    // Experience data.
    in >> m_exp.exp;
    in >> m_exp.level;

    in >> m_energy;
    in >> m_maxEnergy;
    in >> m_energyRefill;
    in >> m_attackCost;

    // Restore properties from the type of the tower.
    TProps pp = towers::generateProps(m_type, m_pos);
    m_minRange = pp.minRange;
    m_maxRange = pp.maxRange;
    m_aoeRadius = pp.aoeRadius;
    m_rotationSpeed = pp.rotationSpeed;

    in >> i;
    m_targetMode = static_cast<towers::Targetting>(i);

    m_shooting.shootAngle = pp.shootAngle;
    m_shooting.projectileSpeed = pp.projectileSpeed;
    m_shooting.aimSpeed = pp.aimSpeed;
    m_shooting.acceleration = pp.acceleration;
    in >> m_shooting.aiming;
    in >> m_shooting.aimStart;
    in >> m_shooting.aimingCone;
    in >> m_shooting.pauseTime;

    m_attack = fromProps(pp);

    m_processes = towers::generateData(m_type);
    // As discussed in the serialization function we won't
    // restore targets of this tower: we assume that the
    // application of the tower's behavior should result in
    // picking the same targets.
    m_targets.clear();

    log("Restored tower at " + m_pos.toString(), utils::Level::Verbose);

    return in;
  }

  void
  Tower::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // Pick and align with the target.
    if (!pickAndAlignWithTarget(info) || m_targets.empty()) {
      // Can't do anything: we didn't find a
      // target or we are not aligned with it
      // yet. The tower is not aiming anymore,
      // so reset the props.
      m_shooting.aiming = false;
      if (!hasInfiniteAimingSpeed(m_shooting.aimSpeed(m_exp.level))) {
        m_shooting.aimingCone = init_aiming_cone;
      }

      return;
    }

    // In case we're not aiming yet, do it.
    if (!m_shooting.aiming) {
      m_shooting.aiming = true;
      m_shooting.aimStart = info.moment;
    }

    // Determine whether the aiming period is
    // over and adapt the aiming cone to show
    // the current progress.
    utils::Duration d = aimDuration(m_shooting.aimSpeed(m_exp.level));
    if (m_shooting.aimStart + d > info.moment) {
      float e = utils::toMilliseconds(info.moment - m_shooting.aimStart);
      float t = utils::toMilliseconds(d);

      m_shooting.aimingCone = init_aiming_cone * utils::clamp(1.0f - e / t, 0.0f, 1.0f);

      return;
    }

    // Check whether we can attack.
    if (m_energy < m_attackCost) {
      return;
    }

    // Reset the aiming process as we fired a shot.
    m_shooting.aimStart = info.moment;
    if (!hasInfiniteAimingSpeed(m_shooting.aimSpeed(m_exp.level))) {
      m_shooting.aimingCone = init_aiming_cone;
    }

    // Fire a shot at each target.
    for (unsigned id = 0u ; id < m_targets.size() ; ++id) {
      MobShPtr m = m_targets[id];

      if (m_attack.damage(m_exp.level) > 0.0f) {
        log(
          "Hitting mob " + mobs::toString(m->getType()) +
          " at " + m->getPos().toString() +
          " for " + std::to_string(m_attack.damage(m_exp.level)) + " damage" +
          " (health: " + std::to_string(m->getHealth()) + ", " +
          " accuracy: " + std::to_string(m_attack.accuracy(m_exp.level)) + ")",
          utils::Level::Verbose
        );
      }

      // Hit the mob with a devastating attack. Note that
      // in case the attack destroys the mob it will be
      // marked as deleted automatically. Also, note that
      // we only want to claim the reward in case the mob
      // is not already dead but we consider that we do
      // attack it even if it's dead.
      m_energy -= m_attackCost;
      if (m->isDead() || m->isDeleted() || attack(info, m)) {
        continue;
      }

      info.gold += m->getBounty();
      gainExp(m->getExpReward());

      log(
        "Killed " + mobs::toString(m->getType()) +
        " at " + m->getPos().toString() +
        ", earned " + std::to_string(m->getBounty()) + " coin(s)" +
        ", exp is now " + std::to_string(m_exp.exp) +
        " (level: " + std::to_string(m_exp.level) + ")"
      );
    }

    // Clear dead or removed targets or all of them
    // in case the tower does not persist targets.
    if (!m_persistTargets) {
      m_targets.clear();
    }
    else {
      m_targets.erase(
        std::remove_if(
          m_targets.begin(),
          m_targets.end(),
          [](const MobShPtr m) -> bool {
            return m->isDead() || m->isDeleted();
          }
        ),
        m_targets.end()
      );
    }
  }

  void
  Tower::pause(const utils::TimeStamp& t) {
    // Similarly to what happen for `Mob`, the goal
    // of this method is to prepare the resume call
    // which will make the process advance again.
    // Note that as the aiming speed needs to keep
    // track of the actual starting time, we can't
    // just reduce the aiming time as we're doing
    // in the case of `Mob`.
    // Instead we will just register this time and
    // let the process of `resume` handle things.
    m_shooting.pauseTime = t;
  }

  void
  Tower::resume(const utils::TimeStamp& t) {
    // Update the start of the aiming time so that
    // it corresponds to a time in the past similar
    // to how it was when the process was paused.
    utils::Duration e = m_shooting.pauseTime - m_shooting.aimStart;
    m_shooting.aimStart = t - e;
  }

  bool
  Tower::pickAndAlignWithTarget(StepInfo& info) {
    // Check whether a target is already defined.
    // If this is the case we will make sure that
    // the distance is still less than the range
    // of the tower (so that we can shoot it).
    // This process applies to all the targets
    // selected so far.
    if (!m_targets.empty()) {
      std::vector<MobShPtr>::iterator toRm;
      toRm = std::remove_if(
        m_targets.begin(),
        m_targets.end(),
        [this](const MobShPtr m) -> bool {
          // In case the target is already dead we
          // will try to find a new one.
          if (m->isDead()) {
            return true;
          }

          // In case the target is already deleted
          // we will try to find a new one.
          if (m->isDeleted()) {
            return true;
          }

          // In case the target is closer than the
          // min range or farther than the max range
          // we will try to find a new one.
          float d = utils::d(m->getPos(), getPos());
          if (d < m_minRange(fetchUpgradeLevel(towers::Upgrade::Range)) ||
              d > m_maxRange(fetchUpgradeLevel(towers::Upgrade::Range)))
          {
            return true;
          }

          // The target is still valid.
          return false;
        }
      );

      // In case at least one target was removed we
      // will reset the aim flag.
      if (toRm != m_targets.end()) {
        m_shooting.aiming = false;
      }

      m_targets.erase(toRm, m_targets.end());
    }

    // The above section might have reset the
    // target: in this case we want to find a
    // new one so that we can still spend the
    // frame trying to face it.
    if (m_targets.empty()) {
      // Find the closest mob.
      towers::PickData pd;
      pd.pos = m_pos;
      pd.minRange = m_minRange(fetchUpgradeLevel(towers::Upgrade::Range));
      pd.maxRange = m_maxRange(fetchUpgradeLevel(towers::Upgrade::Range));
      pd.mode = m_targetMode;

      m_targets = m_processes.pickMob(info, pd);

      if (m_targets.empty()) {
        // No mobs are visible, nothing to do.
        m_shooting.aiming = false;
        return false;
      }
    }

    // We now know that at least one target *is* set
    // and that it lies within the range of the tower.
    // We will try to align with it: this means rotating
    // the tower until we are pointing right at the
    // target.
    // We will pick the first element of the targets'
    // list to perform the aiming.
    MobShPtr tgt = m_targets.front();

    float dx = tgt->getPos().x() - getPos().x();
    float dy = tgt->getPos().y() - getPos().y();

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

    float achieved = std::min(aDif, getRotationSpeed() * info.elapsed);
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
    return std::abs(m_orientation - theta) <= m_shooting.shootAngle(m_exp.level);
  }

  bool
  Tower::attack(StepInfo& info,
                MobShPtr mob)
  {
    // We have to distinguish between two main cases:
    //   - the tower has an infinite projectile speed.
    //   - the tower should shoot projectiles.
    // We could theoretically create a projectile even
    // in the first case but we figured it would just
    // overload the simulation with useless objects.

    // Case of an infinite projectile speed.
    if (hasInfiniteProjectileSpeed(getProjectileSpeed())) {
      // Convert to get the current damage values for
      // the tower given its level.
      towers::Damage dd;
      dd.damage = getAttack();
      dd.accuracy = m_attack.accuracy(m_exp.level);
      dd.speed = m_attack.speed(fetchUpgradeLevel(towers::Upgrade::FreezingPower));
      dd.slowdown = m_attack.slowdown(fetchUpgradeLevel(towers::Upgrade::FreezingSpeed));
      dd.stunProb = m_attack.stunProb(fetchUpgradeLevel(towers::Upgrade::StunChance));
      dd.critProb = m_attack.critProb(m_exp.level);
      dd.critMultiplier = m_attack.critMultiplier(m_exp.level);

      // Convert durations from raw milliseconds to a
      // usable time data.
      int ms = static_cast<int>(std::round(getFreezingDuration()));
      dd.fDuration = utils::toMilliseconds(ms);
      ms = static_cast<int>(std::round(getStunDuration()));
      dd.sDuration = utils::toMilliseconds(ms);
      ms = static_cast<int>(std::round(getPoisonDuration()));
      dd.pDuration = utils::toMilliseconds(ms);

      return m_processes.damage(info, mob, dd);
    }

    // Otherwise we need to create a projectile.
    Projectile::PProps pp = Projectile::newProps(getPos(), getOwner());
    pp.speed = m_shooting.projectileSpeed(fetchUpgradeLevel(towers::Upgrade::ProjectileSpeed));

    pp.damage = getAttack();
    pp.aoeRadius = m_aoeRadius(m_exp.level);

    pp.accuracy = m_attack.accuracy(m_exp.level);

    pp.freezePercent = m_attack.speed(fetchUpgradeLevel(towers::Upgrade::FreezingPower));
    pp.freezeSpeed = m_attack.slowdown(fetchUpgradeLevel(towers::Upgrade::FreezingSpeed));

    pp.stunProb = m_attack.stunProb(fetchUpgradeLevel(towers::Upgrade::StunChance));

    int ms = static_cast<int>(std::round(getFreezingDuration()));
    pp.freezeDuration = utils::toMilliseconds(ms);
    ms = static_cast<int>(std::round(getStunDuration()));
    pp.stunDuration = utils::toMilliseconds(ms);
    ms = static_cast<int>(std::round(getPoisonDuration()));
    pp.poisonDuration = utils::toMilliseconds(ms);

    info.spawnProjectile(std::make_shared<Projectile>(pp, this, mob));

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
