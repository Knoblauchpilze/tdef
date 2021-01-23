
# include "Basic.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace basic {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 2.0f;

      constexpr float damage = 7.5f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = 1.25f;
      constexpr float rotation = utils::degToRad(90.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = 3.0f;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 0.0f;
      constexpr float shootAngle = 0.0f;
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.0f;

      constexpr float cost = 48.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Basic);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.minRange = minRange;
        pp.maxRange = maxRange;

        pp.damage = damage;
        pp.aoeRadius = aoeRadius;

        pp.attackSpeed = attackSpeed;
        pp.rotation = rotation;
        pp.aimSpeed = aimSpeed;
        pp.projectileSpeed = projectileSpeed;
        pp.accuracy = accuracy;

        pp.duration = duration;
        pp.shootAngle = shootAngle;
        pp.projectiles = projectiles;
        pp.acceleration = acceleration;

        pp.freezePercent = no_speed_decrease;
        pp.freezeSpeed = freezeSpeed;

        pp.stunProb = stunProb;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        pp.upgrades.clear();
        pp.upgrades.push_back(towers::Upgrade::Range);
        pp.upgrades.push_back(towers::Upgrade::Damage);
        pp.upgrades.push_back(towers::Upgrade::RotationSpeed);
        pp.upgrades.push_back(towers::Upgrade::ProjectileSpeed);

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

      float
      getUpgradeCost(const Upgrade& upgrade,
                     int level) noexcept
      {
        switch (upgrade) {
          case Upgrade::Range:
          case Upgrade::Damage:
          case Upgrade::RotationSpeed:
          case Upgrade::ProjectileSpeed:
            return 49.0f * (level + 1);
          case Upgrade::AttackSpeed:
          case Upgrade::FreezingPower:
          case Upgrade::FreezingSpeed:
          case Upgrade::FreezingDuration:
          case Upgrade::PoisonDuration:
          case Upgrade::StunChance:
          case Upgrade::StunDuration:
          default:
            // Default upgrade cost is null.
            return 0.0f;
        }
      }

    }

  }
}
