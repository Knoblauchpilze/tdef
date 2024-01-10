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
  mobs::Effects
  Mob::getEffects() const noexcept {
    mobs::Effects e;

    e.freezed = (m_speed.fSpeed != 1.0f);
    e.poisoned = (m_poison.damage != 0.0f);

    e.stunned = (m_speed.speed == 0.0f);

    return e;
  }

  inline
  bool
  Mob::isEnRoute() const noexcept {
    return m_path.enRoute(m_rArrival);
  }


  inline
  std::ostream&
  Mob::operator<<(std::ostream& out) const {
    WorldElement::operator<<(out);

    out << static_cast<int>(m_type) << " ";
    out << m_energy << " ";
    out << m_maxEnergy << " ";
    out << m_energyRefill << " ";
    // Note that we won't save the behavior nor the path:
    // indeed as we're not saving the target anyway it
    // would just need to confusing situation where the
    // mob would try to go to a position without knowing
    // if the target is still here. So rather have the
    // whole thinking process start again and select a
    // new target.
    out << m_attackCost << " ";
    out << m_attack << " ";
    out << m_rArrival << " ";
    // Skip path.
    out << m_bounty << " ";
    out << m_cost << " ";
    out << m_exp << " ";

    // Defense data.
    out << m_defense.shield << " ";
    out << m_defense.shieldEfficiency << " ";
    out << m_defense.shieldDurability << " ";
    out << m_defense.poisonable << " ";
    out << m_defense.slowable << " ";
    out << m_defense.stunnable << " ";

    // Speed data.
    out << m_speed.bSpeed << " ";
    out << m_speed.speed << " ";
    // In order to save the speed time and duration,
    // we only save the duration. Indeed as we only
    // allow a Mob to be saved when the game is in
    // pause, we will have a duration corresponding
    // to the remaining freeze time already. It is
    // then valid to consider that when restoring
    // this data, we will put the current timestamp
    // in the freeze time and use the remaining
    // duration for the freeze time.
    // We apply a similar reasoning for the stun
    // and poison duration.
    out << utils::toMilliseconds(m_speed.fDuration) << " ";
    out << m_speed.fSpeed << " ";
    out << m_speed.sDecrease << " ";
    out << m_speed.sIncrease << " ";
    out << utils::toMilliseconds(m_speed.sDuration) << " ";

    // Poison data.
    out << m_poison.damage << " ";
    out << m_poison.stack << " ";
    out << utils::toMilliseconds(m_poison.pDuration) << " ";

    // Ignore the mob's target as we will reset its
    // behavior and look for a new target when the
    // game is deserialized.

    verbose("Saved mob at " + m_pos.toString());

    return out;
  }

  inline
  std::istream&
  Mob::operator>>(std::istream& in) {
    WorldElement::operator>>(in);

    int i;
    in >> i;
    m_type = static_cast<mobs::Type>(i);
    in >> m_energy;
    in >> m_maxEnergy;
    in >> m_energyRefill;
    // Assume default behavior: this will trigger
    // the definition of a new target.
    m_behavior = Behavior::None;
    in >> m_attackCost;
    in >> m_attack;
    in >> m_rArrival;
    // Reset path to the current position.
    m_path.clear(m_pos);
    in >> m_bounty;
    in >> m_cost;
    in >> m_exp;

    // Defense data.
    in >> m_defense.shield;
    in >> m_defense.shieldEfficiency;
    in >> m_defense.shieldDurability;
    in >> m_defense.poisonable;
    in >> m_defense.slowable;
    in >> m_defense.stunnable;

    // Speed data.
    in >> m_speed.bSpeed;
    in >> m_speed.speed;
    float d;
    // Initialize freeze, stun and poison time to
    // the current moment. It should be overriden
    // when we actually resume the game.
    m_speed.tFreeze = utils::now();
    in >> d;
    m_speed.fDuration = utils::toMilliseconds(d);
    in >> m_speed.fSpeed;
    in >> m_speed.sDecrease;
    in >> m_speed.sIncrease;
    m_speed.tStun = utils::now();
    in >> d;
    m_speed.sDuration = utils::toMilliseconds(d);

    // Poison data.
    in >> m_poison.damage;
    in >> m_poison.stack;
    m_poison.tPoison = utils::now();
    in >> d;
    m_poison.pDuration = utils::toMilliseconds(d);

    // Do not save the target of the mob: as discussed it would
    // require to somehow be able to link it back again when the
    // world is reloaded. We'd rather let the regular thinking
    // process determine a new one (which would probably be the
    // same anyway) when this happens.

    verbose("Restored mob at " + m_pos.toString());

    return in;
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

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Mob& m) noexcept {
  m << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Mob& m) noexcept {
  m >> in;
  return in;
}

#endif    /* MOB_HXX */
