#ifndef    TOWER_HXX
# define   TOWER_HXX

# include "Tower.hh"
# include <maths_utils/ComparisonUtils.hh>

namespace tdef {
  namespace towers {

    inline
    Upgradable
    buildConstantUpgradable(float value) noexcept {
      return [value](int /*upgradeLevel*/, int /*towerLevel*/) {
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
    toString(const Upgrade& u) noexcept {
      switch (u) {
        case Upgrade::Range:
          return "Range";
        case Upgrade::Damage:
          return "Damage";
        case Upgrade::RotationSpeed:
          return "RotationSpeed";
        case Upgrade::AttackSpeed:
          return "AttackSpeed";
        case Upgrade::AimSpeed:
          return "AimSpeed";
        case Upgrade::Accuracy:
          return "Accuracy";
        case Upgrade::ProjectileSpeed:
          return "ProjectileSpeed";
        case Upgrade::FreezingPower:
          return "FreezingPower";
        case Upgrade::FreezingSpeed:
          return "FreezingSpeed";
        case Upgrade::StunChance:
          return "StunChance";
        case Upgrade::StunDuration:
          return "StunDuration";
        default:
          return "Unknown";
      }
    }

    inline
    std::string
    toString(const Targetting& t) noexcept {
      switch (t) {
        case Targetting::First:
          return "First";
        case Targetting::Last:
          return "Last";
        case Targetting::Strongest:
          return "Strongest";
        case Targetting::Weak:
          return "Weak";
        case Targetting::Closest:
          return "Closest";
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
    pp.refill = towers::buildConstantUpgradable(0.1f);

    pp.minRange = towers::buildConstantUpgradable(0.0f);
    pp.maxRange = towers::buildConstantUpgradable(3.0f);

    pp.damage = towers::buildConstantUpgradable(0.2f);
    pp.aoeRadius = towers::buildConstantUpgradable(0.0f);
    pp.rotationSpeed = towers::buildConstantUpgradable(3.1415926535f);
    pp.targetting = towers::Targetting::First;
    pp.persistTargets = true;

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
  const towers::Type&
  Tower::getType() const noexcept {
    return m_type;
  }

  inline
  float
  Tower::getRange() const noexcept {
    return queryUpgradable(m_maxRange, towers::Upgrade::Range);
  }

  inline
  float
  Tower::getAttack() const noexcept {
    return queryUpgradable(m_attack.damage, towers::Upgrade::Damage);
  }

  inline
  float
  Tower::getRotationSpeed() const noexcept {
    return queryUpgradable(m_rotationSpeed, towers::Upgrade::RotationSpeed);
  }

  inline
  const towers::Targetting&
  Tower::getTargetMode() const noexcept {
    return m_targetMode;
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
    return fetchUpgradeLevel(upgrade);
  }

  inline
  float
  Tower::getEnergyRefill() const noexcept {
    return queryUpgradable(m_energyRefill, towers::Upgrade::AttackSpeed);
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
    return getEnergyRefill() / m_attackCost;
  }

  inline
  float
  Tower::getAimingSpeed() const noexcept {
    return queryUpgradable(m_shooting.aimSpeed, towers::Upgrade::AimSpeed);
  }

  inline
  float
  Tower::getAccuracy() const noexcept {
    return queryUpgradable(m_attack.accuracy, towers::Upgrade::Accuracy);
  }

  inline
  float
  Tower::getProjectileSpeed() const noexcept {
    return queryUpgradable(m_shooting.projectileSpeed, towers::Upgrade::ProjectileSpeed);
  }

  inline
  float
  Tower::getFreezingPower() const noexcept {
    return utils::clamp(
      100.0f * (1.0f - queryUpgradable(m_attack.speed, towers::Upgrade::FreezingPower)),
      0.0f,
      100.0f
    );
  }

  inline
  float
  Tower::getFreezingSpeed() const noexcept {
    return utils::clamp(
      100.0f * queryUpgradable(m_attack.slowdown, towers::Upgrade::FreezingSpeed),
      0.0f,
      100.0f
    );
  }

  inline
  float
  Tower::getFreezingDuration() const noexcept {
    // No upgrade related to freeze duration.
    return m_attack.fDuration(0, m_exp.level);
  }

  inline
  float
  Tower::getPoisonDuration() const noexcept {
    // No upgrade related to poison duration.
    return m_attack.pDuration(0, m_exp.level);
  }

  inline
  float
  Tower::getStunChance() const noexcept {
    return utils::clamp(
      100.0f * queryUpgradable(m_attack.stunProb, towers::Upgrade::StunChance),
      0.0f,
      100.0f
    );
  }

  inline
  float
  Tower::getStunDuration() const noexcept {
    return queryUpgradable(m_attack.sDuration, towers::Upgrade::StunDuration);
  }

  inline
  float
  Tower::getAimingCone() const noexcept {
    return m_shooting.aimingCone;
  }

  inline
  void
  Tower::setTargetMode(const towers::Targetting& mode) noexcept {
    // Assign the new target mode and reset the target
    // data to allow the application of the new mode
    // right away.
    debug(
      "Switch target mode from " + towers::toString(m_targetMode) +
      " to " + towers::toString(mode)
    );

    m_targetMode = mode;
    m_targets.clear();
  }

  inline
  std::ostream&
  Tower::operator<<(std::ostream& out) const {
    Block::operator<<(out);

    out << static_cast<int>(m_type) << " ";

    // Upgrades.
    out << m_upgrades.size() << " ";
    for (unsigned id = 0u ; id < m_upgrades.size() ; ++id) {
      out << static_cast<int>(m_upgrades[id].type) << " ";
      out << m_upgrades[id].level << " ";
    }

    // Experience data.
    out << m_exp.exp << " ";
    out << m_exp.level << " ";

    out << m_energy << " ";
    out << m_maxEnergy << " ";
    out << m_attackCost << " ";

    out << static_cast<int>(m_targetMode) << " ";

    // Do not save any of the shooting nor attack data
    // as we won't save the target anyway. So no need
    // to save the aiming start and current value as
    // we will probably start it all over again when
    // the tower is deserialized anyway.

    // The targets picked by a mob will not be saved to
    // the stream. Indeed in order to be able to reuse
    // this information when the data is deserialized,
    // we would need to somehow persist an identifier
    // of the mob so that we can link it to the tower
    // again afterwards. There is no simple way as of
    // now. Moreover, when the game will be resumed as
    // we restored the mob in the same position as it
    // was we should obtain the same target selection
    // with the tower's behavior: the only info that
    // is lost will be the benefit of the aim time or
    // acceleration that was already built up.

    verbose("Saved tower at " + m_pos.toString());

    return out;
  }

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
    if (props.freezePercent(0, 0) > 0.0f) {
      dd.fDuration = props.duration;

      noFreezing = false;
    }

    // Only assign the duration in case the stun prob
    // is not `0` and the freezing percentage indicates
    // no freezing and the probability of stunning is
    // not zero for the first level.
    bool noStunning = true;
    dd.stunProb = towers::buildConstantUpgradable(0.0f);
    dd.sDuration = towers::buildConstantUpgradable(0.0f);
    if (noFreezing && props.stunProb(0, 0) > 0.0f) {
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

  inline
  bool
  Tower::hasInfiniteProjectileSpeed(float speed) noexcept {
    return speed < 0.0f;
  }

  inline
  bool
  Tower::hasInfiniteAimingSpeed(float speed) noexcept {
    return speed <= 0.0f;
  }

  inline
  int
  Tower::fetchUpgradeLevel(const towers::Upgrade& upgrade) const noexcept {
    // Check whether an upgrade for the input type is available
    // and use it as the level of the upgradable.
    unsigned id = 0u;
    while (id < m_upgrades.size() && m_upgrades[id].type != upgrade) {
      ++id;
    }

      // Assume base level.
    if (id >= m_upgrades.size()) {
      return 0;
    }

    return m_upgrades[id].level;
  }

  inline
  float
  Tower::queryUpgradable(const towers::Upgradable& ug,
                         const towers::Upgrade& type) const noexcept
  {
    return ug(fetchUpgradeLevel(type), m_exp.level);
  }

}

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Tower& t) noexcept {
  t << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Tower& t) noexcept {
  t >> in;
  return in;
}

#endif    /* TOWER_HXX */
