#ifndef    TOWER_HXX
# define   TOWER_HXX

# include "Tower.hh"

namespace tdef {
  namespace towers {

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

    pp.minRange = 0.0f;
    pp.maxRange = 3.0f;

    pp.damage = 0.2f;
    pp.aoeRadius = 0.0f;

    pp.attackSpeed = 1.0f;
    pp.aimSpeed = 1.0f;
    pp.projectileSpeed = 1.0f;
    pp.accuracy = 1.0f;

    pp.duration = 0.0f;
    pp.shootAngle = 0.0f;
    pp.projectiles = 1;
    pp.acceleration = 0.0f;

    pp.freezePercent = 0.0f;
    pp.freezeSpeed = 0.0f;

    pp.attackCost = 0.5f;

    pp.type = type;

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
    return m_maxRange;
  }

  inline
  float
  Tower::getAttack() const noexcept {
    return m_attack.damage;
  }

  inline
  float
  Tower::getAttackSpeed() const noexcept {
    // In case the energy refill is set to `0`, use
    // a null attack speed as the tower cannot do
    // more than one attack (at most).
    if (m_energyRefill <= 0.0f) {
      return 0.0f;
    }

    // Otherwise the attack speed is computed as the
    // ratio of the energy refill on the attack cost.
    return m_energyRefill / m_attackCost;
  }

  inline
  void
  Tower::pause(const utils::TimeStamp& /*t*/) {}

  inline
  void
  Tower::resume(const utils::TimeStamp& /*t*/) {}

  inline
  towers::DamageData
  Tower::fromProps(const TProps& props) noexcept {
    towers::DamageData dd;

    dd.damage = props.damage;

    dd.aoeRadius = props.aoeRadius;

    dd.accuracy = props.accuracy;

    dd.speed = props.freezePercent;
    dd.slowdown = props.freezeSpeed;
    dd.sDuration = utils::toMilliseconds(
      static_cast<int>(
        std::round(props.duration)
      )
    );

    dd.pDuration = utils::toMilliseconds(
      static_cast<int>(
        std::round(props.duration)
      )
    );

    return dd;
  }

}

#endif    /* TOWER_HXX */
