#ifndef    MOB_HXX
# define   MOB_HXX

# include "Mob.hh"
# include <maths_utils/ComparisonUtils.hh>

namespace tdef {
  namespace mobs {

    inline
    std::string
    toString(const Type& t) noexcept {
      switch (t) {
        case Type::Regular:
          return "Regular";
        case Type::Fast:
          return "Fast";
        case Type::Strong:
          return "Strong";
        case Type::Heli:
          return "Heli";
        case Type::Jet:
          return "Jet";
        case Type::Armored:
          return "Armored";
        case Type::Healer:
          return "Healer";
        case Type::Toxic:
          return "Toxic";
        case Type::Icy:
          return "Icy";
        case Type::Fighter:
          return "Fighter";
        case Type::Light:
          return "Light";
        default:
          return "Unknown";
      }
    }

  }

  inline
  Mob::MProps
  Mob::newProps(const utils::Point2f& p,
                const mobs::Type& type,
                const utils::Uuid& owner) noexcept
  {
    MProps pp;
    assignProps(pp, p, owner);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.01f;

    pp.speed = 1.0f;
    pp.acceleration = 1.0f;
    pp.arrival = 0.0001f;

    pp.attack = 0.2f;
    pp.attackCost = 1.0f;

    pp.bounty = 1.0f;
    pp.lives = 1.0f;

    pp.shield = 0.0f;

    pp.poisonable = true;
    pp.slowable = true;
    pp.stunnable = true;

    pp.type = type;

    return pp;
  }

  inline
  const mobs::Type&
  Mob::getType() const noexcept {
    return m_type;
  }

  inline
  const Path&
  Mob::getPath() const noexcept {
    return m_path;
  }

  inline
  float
  Mob::getBounty() const noexcept {
    return m_bounty;
  }

  inline
  float
  Mob::getExpReward() const noexcept {
    return m_exp;
  }

  inline
  float
  Mob::getSpeed() const noexcept {
    return m_speed.speed;
  }

  inline
  bool
  Mob::isEnRoute() const noexcept {
    return m_path.enRoute(m_rArrival);
  }

  inline
  void
  Mob::init(StepInfo& /*info*/) {}

  inline
  void
  Mob::destroy(StepInfo& /*info*/) {}

  inline
  mobs::DefenseData
  Mob::fromProps(const MProps& props) noexcept {
    mobs::DefenseData dd;

    dd.shield = utils::clamp(props.shield, 0.0f, 1.0f);
    dd.shieldEfficiency = utils::clamp(props.shieldEfficiency, 0.0f, 1.0f);
    dd.shieldDurability = 1.0f;

    dd.poisonable = props.poisonable;
    dd.slowable = props.slowable;
    dd.stunnable = props.stunnable;

    return dd;
  }

}

#endif    /* MOB_HXX */
