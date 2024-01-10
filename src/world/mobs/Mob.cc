
# include "Mob.hh"
# include <maths_utils/ComparisonUtils.hh>
# include "Block.hh"
# include "Portal.hh"
# include "Locator.hh"

namespace tdef {

  Mob::Mob(const MProps& props):
    WorldElement(props, mobs::toString(props.type)),

    m_type(props.type),

    m_energy(props.energy),
    m_maxEnergy(props.maxEnergy),
    m_energyRefill(props.refill),

    m_behavior(Behavior::None),

    m_attackCost(props.attackCost),
    m_attack(props.attack),

    m_rArrival(props.arrival),
    m_path(m_pos),

    m_bounty(props.bounty),
    m_cost(props.lives),
    m_exp(props.exp),

    m_defense(fromProps(props)),

    m_speed({
      props.speed,
      props.speed,
      utils::now(),
      utils::Duration::zero(),
      1.0f,
      0.0f,
      props.acceleration,
      utils::now(),
      utils::Duration::zero()
    }),
    m_poison({
      0.0f,
      0,
      utils::now(),
      utils::Duration::zero()
    }),

    m_target(nullptr)
  {
    setService("mob");
  }

  bool
  Mob::hit(StepInfo& info,
           const mobs::Damage& d)
  {
    // First thing is to determine whether the
    // hit succeeded through the accuracy.
    float rnd = info.rng.rndFloat(0.0f, 1.0f);
    if (rnd > d.accuracy) {
      debug("Projectile failed to hit (accuracy: " + std::to_string(d.accuracy) + ", trial: " + std::to_string(rnd) + ")");
      return true;
    }

    // Handle each type of damage.
    applyDamage(info, d);
    applyFreezing(info, d);
    applyStunning(info, d);
    applyPoison(info, d);

    // Register this mob for deletion in case it
    // is not alive anymore.
    bool dead = isDead();
    markForDeletion(dead);

    return !dead;
  }

  void
  Mob::step(StepInfo& info) {
    // Refilll the energy.
    m_energy = std::min(m_energy + info.elapsed * m_energyRefill, m_maxEnergy);

    // Handle the speed and poisoning effect.
    updateSpeed(info);
    updateHealth(info);

    // Note that in case the mob is now dead (due to
    // the damage applied in the above method) we do
    // not want to pursue the process (and possibly
    // lose a life).
    if (isDead()) {
      markForDeletion(true);
      return;
    }

    // In case we're already following a path, go
    // there. We also want to make sure that the
    // target is still valid (i.e. has not been
    // deleted) in which case we should try to
    // find a new one.
    if (isEnRoute()) {
      // Adjust the target and check whether it has
      // been deleted. If this is the case we want
      // to try to find a new one asap.
      if (m_target != nullptr && !m_target->isDeleted()) {
        m_path.advance(m_speed.speed, info.elapsed, m_rArrival);
        m_pos = m_path.cur();

        return;
      }

      verbose(
        "Current target at " + m_target->getPos().toString() +
        " does not exist anymore"
      );

      // The target is either null (weird) or has
      // been deleted (probably by another mob).
      m_behavior = Behavior::None;
      m_target = nullptr;
      m_path.clear(m_pos);
    }

    // We arrived at the target: check what we need
    // to do now. It can also happen that we didn't
    // had a target in the first place in which case
    // we need to find one.
    if (m_behavior == Behavior::None) {
      Path np(m_pos);

      // First, attempt to locate a portal: if this
      // succeeds, use this target.
      if (locatePortal(info.frustum, np)) {
        std::swap(m_path, np);
        return;
      }

      verbose("Failed to find portal, trying to find defense");

      if (destroyDefenses(info.frustum, np)) {
        std::swap(m_path, np);
        return;
      }

      // Failed to locate a portal or a tower or
      // a wall to break. We don't really know
      // what to to here.
      warn("Failed to find a valid target, mob is now stuck");
      return;
    }
    if (m_behavior == Behavior::PortalSeeker) {
      BlockShPtr b = info.frustum->getClosestBlock(m_pos, world::BlockType::Portal, -1.0f, nullptr);
      PortalShPtr p = std::dynamic_pointer_cast<Portal>(b);

      if (p == nullptr || utils::d(p->getPos(), m_pos) > m_rArrival) {
        warn("Target portal is either missing or too far");
        m_behavior = Behavior::None;
        m_path.clear(m_pos);
        return;
      }

      // Breach the portal and mark the mob for deletion.
      p->breach(m_cost);
      markForDeletion(true);

      this->info(
        "Mob made it through (health: " +
        std::to_string(getHealth()) + "/" + std::to_string(getTotalHealth()) + ")" +
        ", lives: " + std::to_string(p->getLives())
      );

      return;
    }
    if (m_behavior == Behavior::WallBreaker) {
      WallShPtr w = std::dynamic_pointer_cast<Wall>(m_target);
      if (w != nullptr) {
        float d = utils::d(w->getPos(), m_pos);
        if (d > m_rArrival) {
          warn("Target wall is too far (d: " + std::to_string(d) + ")");
          m_behavior = Behavior::None;
          m_path.clear(m_pos);
          return;
        }

        // Perform the attack if possible.
        if (m_energy < m_attackCost) {
          return;
        }

        m_energy -= m_attackCost;
        if (!w->damage(info, m_attack)) {
          // Mark the wall for deletion, reset the behavior
          // so that we get a new chance to evaluate whether
          // a portal is reachable and reset the target.
          w->markForDeletion(true);
          m_behavior = Behavior::None;
          m_target = nullptr;

          debug("Killed wall at " + w->getPos().toString());
        }

        return;
      }

      TowerShPtr t = std::dynamic_pointer_cast<Tower>(m_target);
      if (t != nullptr) {
        float d = utils::d(t->getPos(), m_pos);
        if (d > m_rArrival) {
          warn("Target tower is too far (d: " + std::to_string(d) + ")");
          m_behavior = Behavior::None;
          m_path.clear(m_pos);
          return;
        }

        // Perform the attack if possible.
        if (m_energy < m_attackCost) {
          return;
        }

        m_energy -= m_attackCost;
        if (!t->damage(info, m_attack)) {
          // Mark the wall for deletion, reset the behavior
          // so that we get a new chance to evaluate whether
          // a portal is reachable and reset the target.
          t->markForDeletion(true);
          m_behavior = Behavior::None;
          m_target = nullptr;

          debug("Killed tower " + towers::toString(t->getType()) + " at " + t->getPos().toString());
        }

        return;
      }

      // Failed to interpret target either as a wall or
      // a tower. This is weird.
      warn("Target element could not be interpreted");
      m_behavior = Behavior::None;
      m_path.clear(m_pos);
      m_target = nullptr;

      return;
    }
  }

  void
  Mob::pause(const utils::TimeStamp& t) {
    // The `pause` method should prepare the call to
    // `resume` by shifting the origin of time-based
    // effects so that they correspond to the moment
    // where the pause was triggered.
    // This is indeed the last moment where we know
    // that the timestamps correspond to meaningful
    // values.
    // It will make the resume operation very easy
    // as we only have to shift again the origin
    // timestamps (given that the duration for each
    // effect is consistent).

    // Each timestamp will be updated so that the
    // remaining duration corresponds to the value
    // currently remaining and the time stamp is
    // equivalent to the start of the pause moment.
    if (m_speed.fDuration != utils::Duration::zero()) {
      utils::Duration elapsed = t - m_speed.tFreeze;

      verbose(
        "Freeze started at " + utils::timeToString(m_speed.tFreeze) +
        " for " + utils::durationToMsString(m_speed.fDuration) +
        ", " + utils::durationToMsString(elapsed) + " elapsed, " +
        utils::durationToMsString(m_speed.fDuration - elapsed) + " remaining"
      );

      m_speed.tFreeze = t;
      m_speed.fDuration = m_speed.fDuration - elapsed;
    }

    if (m_speed.sDuration != utils::Duration::zero()) {
      utils::Duration elapsed = t - m_speed.tStun;

      verbose(
        "Stun started at " + utils::timeToString(m_speed.tStun) +
        " for " + utils::durationToMsString(m_speed.sDuration) +
        ", " + utils::durationToMsString(elapsed) + " elapsed, " +
        utils::durationToMsString(m_speed.sDuration - elapsed) + " remaining"
      );

      m_speed.tStun = t;
      m_speed.sDuration = m_speed.sDuration - elapsed;
    }

    if (m_poison.pDuration != utils::Duration::zero()) {
      utils::Duration elapsed = t - m_poison.tPoison;

      verbose(
        "Poison started at " + utils::timeToString(m_poison.tPoison) +
        " for " + utils::durationToMsString(m_poison.pDuration) +
        ", " + utils::durationToMsString(elapsed) + " elapsed, " +
        utils::durationToMsString(m_poison.pDuration - elapsed) + " remaining"
      );

      m_poison.tPoison = t;
      m_poison.pDuration = m_poison.pDuration - elapsed;
    }
  }

  void
  Mob::resume(const utils::TimeStamp& t) {
    // As the pause method handled the duration so
    // that it really reflects how long the effects
    // should be going on, we just have to offset
    // the timestamps to the current one.
    verbose(
      "Moved speed to start from " + utils::timeToString(m_speed.tFreeze) +
      " to " + utils::timeToString(t) +
      ", end in " + utils::durationToMsString(m_speed.fDuration) +
      " at " + utils::timeToString(t + m_speed.fDuration)
    );
    verbose(
      "Moved stun to start from " + utils::timeToString(m_speed.tStun) +
      " to " + utils::timeToString(t) +
      ", end in " + utils::durationToMsString(m_speed.sDuration) +
      " at " + utils::timeToString(t + m_speed.sDuration)
    );
    verbose(
      "Moved poison to start from " + utils::timeToString(m_poison.tPoison) +
      " to " + utils::timeToString(t) +
      ", end in " + utils::durationToMsString(m_poison.pDuration) +
      " at " + utils::timeToString(t + m_poison.pDuration)
    );

    m_speed.tFreeze = t;
    m_speed.tStun = t;

    m_poison.tPoison = t;
  }

  void
  Mob::worldUpdate(LocatorShPtr loc) {
    // We need to recompute the path to the target if
    // a valid path is assigned.
    if (!isEnRoute()) {
      return;
    }

    // Even in case the mob is currently trying to
    // reach a defense, we will assume that the main
    // objective is to reach a portal. So we will
    // first try to reach one, and if this fails we
    // will try to locate a tower or a wall.
    Path np(m_pos);

    if (locatePortal(loc, np)) {
      std::swap(m_path, np);
      return;
    }

    verbose("Failed to find portal, trying to find defense");

    if (destroyDefenses(loc, np)) {
      std::swap(m_path, np);
      return;
    }

    warn("Failed to find a valid target, mob is now stuck");
  }

  void
  Mob::applyDamage(StepInfo& info,
                   const mobs::Damage& d)
  {
    // First, shield the incoming damage: this will
    // reduce the durability of the shield based on
    // a formula describing the impact of the damage.
    // This only occurs if the shield durability is
    // not exhausted.
    float hit = d.hit;

    if (m_defense.shieldDurability >= 0.0f) {
      float absorbed = hit * m_defense.shieldEfficiency;
      float remaining = hit - absorbed;

      // The durability loss is equivalent to 1% of the
      // difference between the shield health and the
      // amount absorbed.
      // Note that `durabLoss` is negative or `0`.
      float sHP = m_defense.shield * m_totalHealth;
      float durabLoss = sk_shieldHitDurab * std::min(sHP - absorbed, 0.0f);

      m_defense.shieldDurability += durabLoss;

      hit = remaining;
    }

    // Handle the remaining damage if needed.
    if (hit > 0.0f) {
      WorldElement::damage(info, hit);
    }
  }

  void
  Mob::applyFreezing(StepInfo& info,
                     const mobs::Damage& d)
  {
    // Mob is not slowable, abort.
    if (!m_defense.slowable) {
      return;
    }

    // Damage does not define freezing data, abort.
    if (d.speed == 1.0f) {
      return;
    }

    // Refresh the slowing effect if any.
    m_speed.tFreeze = info.moment;

    // Update freezed speed and refresh slowing effect.
    // In case the new freeze speed is bigger than the
    // current freeze speed we will wait for the freeze
    // effect to wear off so that we benefit better
    // from the current (stronger) applied effect.
    if (m_speed.fSpeed >= d.speed) {
      // The new speed would make the mob even slower.
      m_speed.tFreeze = info.moment;
      m_speed.fSpeed = d.speed;

      m_speed.fDuration = d.fDuration;
      m_speed.sDecrease = d.sDecraseSpeed;
    }
  }

  void
  Mob::applyStunning(StepInfo& info,
                     const mobs::Damage& d)
  {
    // Mob is not stunnable, abort.
    if (!m_defense.stunnable) {
      return;
    }

    // Damage does not define stunning data, abort.
    if (d.stunProb == 0.0f) {
      return;
    }

    // Check whether the mob is stunned.
    if (info.rng.rndFloat(0.0f, 1.0f) > d.stunProb) {
      return;
    }

    // The mob is now stunned: we will only apply
    // the effect in case the new stun effect is
    // lasting longer than the currently applied
    // one.
    utils::TimeStamp cur = m_speed.tStun + m_speed.sDuration;
    utils::TimeStamp better = info.moment + d.sDuration;
    if (cur > better) {
      return;
    }

    m_speed.tStun = info.moment;
    m_speed.sDuration = d.sDuration;
  }

  void
  Mob::applyPoison(StepInfo& info,
                   const mobs::Damage& d)
  {
    // Mob is not poisonable, abort.
    if (!m_defense.poisonable) {
      return;
    }

    // Damage does not define poisoning data, abort.
    if (d.pDuration == utils::Duration::zero()) {
      return;
    }

    // We will register a new stack of poison for
    // the mob. This will also refresh the duration
    // of all the stack to last as long as the last
    // one applied. We only register the effect in
    // case the expected duration of the poison will
    // last longer than the one currently applied.
    utils::TimeStamp cur = m_poison.tPoison + m_poison.pDuration;
    utils::TimeStamp better = info.moment + d.pDuration;
    if (cur < better) {
      m_poison.tPoison = info.moment;
      m_poison.pDuration = d.pDuration;
    }

    debug(
      "Poisoning mob for " + utils::durationToMsString(d.pDuration) +
      " and for " + std::to_string(d.hit) +
      " after " + std::to_string(m_poison.stack) + " stacks(s)" +
      " already " + std::to_string(m_poison.damage) + " registered"
    );

    // The damage applied per stack is 100% for the
    // first stack, then 50% for the second, 25% for
    // the third etc. Note that we clamp the number
    // of poison stacks to a maximum amount.
    if (m_poison.stack < sk_poisonStacksLimit) {
      m_poison.damage += d.hit * std::pow(2.0f, -1.0f * m_poison.stack);
      ++m_poison.stack;
    }
  }

  void
  Mob::updateSpeed(StepInfo& info) {
    // First, handle stun effect.
    if (m_speed.tStun + m_speed.sDuration >= info.moment) {
      m_speed.speed = 0.0f;

      return;
    }

    // Handle defreezing of mob.
    if (m_speed.tFreeze + m_speed.fDuration < info.moment) {
      // Mob is not freezed anymore
      m_speed.fSpeed = 1.0f;
      m_speed.sDecrease = 0.0f;
    }

    // Update the current speed of the mob based
    // on the desired speed and the acceleration
    // factor.
    float target = m_speed.bSpeed * m_speed.fSpeed;

    // Compute the modifier that will be applied
    // to the speed of this mob. This will either
    // be equal tot he `sDecrease` in case the
    // current speed is larger than the desired
    // speed and `sIncrease` in case the current
    // speed is smaller.
    float mod = m_speed.sIncrease;
    if (m_speed.speed > target) {
      mod = -m_speed.sDecrease;
    }

    // The modifier is a duration expressed in
    // seconds so let's compute the delta. Note
    // though that in case the speed is very
    // small it would also mean that the rate
    // of acceleration/deceleration would be
    // also very slow.
    // So we take the max between the current
    // speed and the target speed so that we
    // always vary the quickest.
    float delta = std::max(m_speed.speed, target) * mod * info.elapsed;

    // Don't forget to clamp the final speed to be
    // consistent with what we desire.
    // We can't really use a min/max approach as
    // the only real bounds would be `0` and the
    // `base speed` but this tells nothing as to a
    // freezed speed for example (typically this
    // is *not* a lower bound as if the mob was
    // stunned we will have a lower speed than the
    // target).
    // So we follow an approach where we clamp in
    // accordance to the speed modifier (so whether
    // or not we're accelerating).
    if (mod > 0) {
      m_speed.speed = utils::clamp(m_speed.speed + delta, 0.0f, target);
    }
    else {
      m_speed.speed = std::max(m_speed.speed + delta, target);
    }
  }

  void
  Mob::updateHealth(StepInfo& info) {
    // Deactivate the poisoning in case the effect
    // has finished.
    if (m_poison.tPoison + m_poison.pDuration < info.moment) {
      m_poison.damage = 0.0f;

      // Also reset the stacks: the next application
      // of poinson will start from scratch.
      m_poison.stack = 0;

      return;
    }

    if (m_poison.damage == 0.0f) {
      return;
    }

    // Apply the damage per second.
    damage(info, m_poison.damage * info.elapsed);
  }

  bool
  Mob::locatePortal(LocatorShPtr loc,
                    Path& path)
  {
    // Clear the path.
    path.clear(m_pos);

    // Attempt to find a portal to reach.
    BlockShPtr b = loc->getClosestBlock(m_pos, world::BlockType::Portal, -1.0f, nullptr);
    PortalShPtr p = std::dynamic_pointer_cast<Portal>(b);

    if (p != nullptr) {
      bool valid = path.generatePathTo(loc, p->getPos(), true, sk_maxPathFindingDistance);
      if (valid) {
        verbose("Found portal at " + p->getPos().toString());
        m_behavior = Behavior::PortalSeeker;
        m_target = b;

        return true;
      }
    }

    // Failed to find a portal or failed to find
    // a path to it.
    return false;
  }

  bool
  Mob::destroyDefenses(LocatorShPtr loc,
                       Path& path)
  {
    // Clear the path.
    path.clear(m_pos);

    // Attempt to find a wall to break.
    BlockShPtr b = loc->getClosestBlock(m_pos, world::BlockType::Wall, -1.0f, nullptr);
    WallShPtr w = std::dynamic_pointer_cast<Wall>(b);

    if (w != nullptr) {
      bool valid = path.generatePathTo(loc, w->getPos(), true, sk_maxPathFindingDistance);
      if (valid) {
        verbose("Found wall at " + w->getPos().toString());
        m_behavior = Behavior::WallBreaker;
        m_target = b;

        return true;
      }
    }

    // Attempt to find a tower to break.
    b = loc->getClosestBlock(m_pos, world::BlockType::Tower, -1.0f, nullptr);
    TowerShPtr t = std::dynamic_pointer_cast<Tower>(b);

    if (t != nullptr) {
      bool valid = path.generatePathTo(loc, t->getPos(), true, sk_maxPathFindingDistance);
      if (valid) {
        verbose("Found tower at " + t->getPos().toString());
        m_behavior = Behavior::WallBreaker;
        m_target = b;

        return true;
      }
    }

    // Failed to find a wall or tower or failed
    // to find a path to it.
    return false;
  }

}
