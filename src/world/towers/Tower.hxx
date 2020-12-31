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

    pp.range = 3.0f;
    pp.attack = 0.2f;
    pp.attackCost = 0.55f;

    pp.type = type;

    return pp;
  }

  inline
  const towers::Type&
  Tower::getType() const noexcept {
    return m_type;
  }

  inline
  float
  Tower::getRange() const noexcept {
    return m_range;
  }

  inline
  float
  Tower::getAttack() const noexcept {
    return m_attack;
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

}

#endif    /* TOWER_HXX */
