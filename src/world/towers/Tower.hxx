#ifndef    TOWER_HXX
# define   TOWER_HXX

# include "Tower.hh"
# include <maths_utils/ComparisonUtils.hh>

namespace tdef {
  namespace towers {

    inline
    Upgradable
    buildConstantUpgradable(float value) noexcept {
      return [value](int /*level*/) {
        return value;
      };
    }

    inline
    std::string
    toString(const Type& t) noexcept {
      switch (t) {
        case Type::Basic:
          return "Basic";
        case Type::Sniper:
          return "Sniper";
        case Type::Cannon:
          return "Cannon";
        case Type::Freezing:
          return "Freezing";
        case Type::Venom:
          return "Venom";
        case Type::Splash:
          return "Splash";
        case Type::Blast:
          return "Blast";
        case Type::Multishot:
          return "Multishot";
        case Type::Minigun:
          return "Minigun";
        case Type::Antiair:
          return "Antiair";
        case Type::Tesla:
          return "Tesla";
        case Type::Missile:
          return "Missile";
        default:
          return "Unknown";
      }
    }

    inline
    std::string
    toString(const Upgrade& t) noexcept {
      switch (t) {
        case Upgrade::Range:
          return "Range";
        case Upgrade::Damage:
          return "Damage";
        case Upgrade::RotationSpeed:
          return "RotationSpeed";
        case Upgrade::AttackSpeed:
          return "AttackSpeed";
        case Upgrade::ProjectileSpeed:
          return "ProjectileSpeed";
        case Upgrade::FreezingPower:
          return "FreezingPower";
        case Upgrade::FreezingSpeed:
          return "FreezingSpeed";
        case Upgrade::FreezingDuration:
          return "FreezingDuration";
        case Upgrade::PoisonDuration:
          return "PoisonDuration";
        case Upgrade::StunChance:
          return "StunChance";
        case Upgrade::StunDuration:
          return "StunDuration";
        default:
          return "Unknown";
      }
    }

  }

  inline
  Tower::TProps
  Tower::newProps(const utils::Point2f& p,
                  const towers::Type& type,
                  const utils::Uuid& owner) noexcept
  {
    TProps pp;
    assignProps(pp, p, owner);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.1f;

    pp.minRange = towers::buildConstantUpgradable(0.0f);
    pp.maxRange = towers::buildConstantUpgradable(3.0f);

    pp.damage = towers::buildConstantUpgradable(0.2f);
    pp.aoeRadius = towers::buildConstantUpgradable(0.0f);

    pp.aimSpeed = towers::buildConstantUpgradable(1.0f);
    pp.projectileSpeed = towers::buildConstantUpgradable(1.0f);
    pp.accuracy = towers::buildConstantUpgradable(1.0f);

    pp.duration = towers::buildConstantUpgradable(0.0f);
    pp.shootAngle = towers::buildConstantUpgradable(0.0f);
    pp.projectiles = towers::buildConstantUpgradable(1.0f);
    pp.acceleration = towers::buildConstantUpgradable(0.0f);

    pp.freezePercent = towers::buildConstantUpgradable(0.0f);
    pp.freezeSpeed = towers::buildConstantUpgradable(0.0f);

    pp.stunProb = towers::buildConstantUpgradable(0.0f);
    pp.critProb = towers::buildConstantUpgradable(0.0f);
    pp.critMultiplier = towers::buildConstantUpgradable(1.0f);

    pp.attackCost = 0.5f;

    pp.type = type;

    pp.cost = 10.0f;

    pp.upgrades.push_back(towers::Upgrade::Range);
    pp.upgrades.push_back(towers::Upgrade::Damage);

    return pp;
  }

  inline
  bool
  Tower::hasInfiniteProjectileSpeed(float speed) noexcept {
    return speed < 0.0f;
  }

  inline
  const towers::Type&
  Tower::getType() const noexcept {
    return m_type;
  }

  inline
  float
  Tower::getRange() const noexcept {
    return m_maxRange(m_exp.level);
  }

  inline
  float
  Tower::getAttack() const noexcept {
    return m_attack.damage(m_exp.level);
  }

  inline
  towers::Upgrades
  Tower::getUpgrades() const noexcept {
    towers::Upgrades us;

    for (unsigned id = 0u ; id < m_upgrades.size() ; ++id) {
      us[m_upgrades[id].type] = m_upgrades[id].level;
    }

    return us;
  }

  inline
  int
  Tower::getUpgradeLevel(const towers::Upgrade& upgrade) const noexcept {
    // Search for the upgrade in the list of available
    // ones and return the level if available.
    for (unsigned id = 0u ; id < m_upgrades.size() ; ++id) {
      if (m_upgrades[id].type == upgrade) {
        return m_upgrades[id].level;
      }
    }

    // The upgrade is not available, return a null level.
    return 0;
  }

  inline
  float
  Tower::getAttackSpeed() const noexcept {
    // In case the attack cost is set to `0`, use
    // a null attack speed to indicate that the
    // tower has an infinite attack speed.
    if (m_attackCost <= 0.0f) {
      return 0.0f;
    }

    // Otherwise the attack speed is computed as the
    // ratio of the energy refill on the attack cost.
    return m_energyRefill / m_attackCost;
  }

  inline
  float
  Tower::getProjectileSpeed() const noexcept {
    return m_shooting.projectileSpeed(m_exp.level);
  }

  inline
  float
  Tower::getFreezingPower() const noexcept {
    return utils::clamp(100.0f * (1.0f - m_attack.speed(m_exp.level)), 0.0f, 100.0f);
  }

  inline
  float
  Tower::getFreezingSpeed() const noexcept {
    return utils::clamp(100.0f * m_attack.slowdown(m_exp.level), 0.0f, 100.0f);
  }

  inline
  float
  Tower::getFreezingDuration() const noexcept {
    return m_attack.fDuration(m_exp.level);
  }

  inline
  float
  Tower::getPoisonDuration() const noexcept {
    return m_attack.pDuration(m_exp.level);
  }

  inline
  float
  Tower::getStunChance() const noexcept {
    return utils::clamp(100.0f * m_attack.stunProb(m_exp.level), 0.0f, 100.0f);
  }

  inline
  float
  Tower::getStunDuration() const noexcept {
    return m_attack.sDuration(m_exp.level);
  }

  inline
  float
  Tower::getMinReachableOrientation() const noexcept {
    return m_orientation - m_shooting.shootAngle(m_exp.level) / 2.0f;
  }

  inline
  float
  Tower::getMaxReachableOrientation() const noexcept {
    return m_orientation + m_shooting.shootAngle(m_exp.level) / 2.0f;
  }

  inline
  void
  Tower::pause(const utils::TimeStamp& /*t*/) {}

  inline
  void
  Tower::resume(const utils::TimeStamp& /*t*/) {}

  inline
  Tower::DamageData
  Tower::fromProps(const TProps& props) noexcept {
    DamageData dd;

    dd.damage = props.damage;

    dd.accuracy = props.accuracy;

    // Convert freeze percent to final speed value.
    dd.speed = props.freezePercent;
    dd.slowdown = props.freezeSpeed;

    // Only assign the duration in case the freezing
    // speed is not `0` for the first level (we will
    // consider that it is also `0` for any other
    // level).
    bool noFreezing = true;
    dd.fDuration = towers::buildConstantUpgradable(0.0f);
    if (props.freezePercent(0) > 0.0f) {
      dd.fDuration = props.duration;

      noFreezing = false;
    }

    // Only assign the duration in case the stun prob
    // is not `0` and the freezing percentage indicates
    // no freezing.
    bool noStunning = true;
    dd.stunProb = towers::buildConstantUpgradable(0.0f);
    dd.sDuration = towers::buildConstantUpgradable(0.0f);
    if (noFreezing) {
      dd.stunProb = props.stunProb;
      dd.sDuration = props.duration;

      noStunning = false;
    }

    // Assume the duration refers to the poisoning
    // in case the freezing speed is set to a `zero`
    // value and no stun chance is defined.
    dd.pDuration = towers::buildConstantUpgradable(0.0f);
    if (noFreezing && noStunning) {
      dd.pDuration = props.duration;
    }

    dd.critProb = props.critProb;
    dd.critMultiplier = props.critMultiplier;

    return dd;
  }

}

#endif    /* TOWER_HXX */
