
# include "Sniper.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace sniper {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 4.5f;

      constexpr float damage = 35.0f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = 0.3f;
      constexpr float rotation = utils::degToRad(50.0f);
      constexpr float aimSpeed = 100.0f;
      constexpr float projectileSpeed = infinite_projectile_speed;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 0.0f;
      constexpr float shootAngle = utils::degToRad(0.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.0f;

      constexpr float critProb = 0.2f;
      constexpr float critMultiplier = 1.5f;

      constexpr float cost = 80.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Sniper);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildQuadraticUpgradable(8.74e-3f, 0.219f, 4.64f, maxRange);

        pp.damage = buildQuadraticUpgradable(2.46f, 6.71f, 35.3f, damage);
        pp.aoeRadius = buildConstantUpgradable(aoeRadius);

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = true;

        pp.rotationSpeed = buildLinearUpgradable(utils::degToRad(4.13f), utils::degToRad(52.5f), rotation);
        pp.aimSpeed = buildQuadraticUpgradable(0.892f, 3.86f, 102.0f, aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = buildConstantUpgradable(duration);
        pp.shootAngle = buildConstantUpgradable(shootAngle);
        pp.projectiles = buildConstantUpgradable(projectiles);
        pp.acceleration = buildConstantUpgradable(acceleration);

        pp.freezePercent = buildConstantUpgradable(freezePercent);
        pp.freezeSpeed = buildConstantUpgradable(freezeSpeed);

        pp.stunProb = buildConstantUpgradable(stunProb);

        pp.critProb = [](int /*upgradeLevel*/, int towerLevel) {
          return critProb + 0.016f * towerLevel;
        };
        pp.critMultiplier = [](int /*upgradeLevel*/, int towerLevel) {
          return critMultiplier + 0.05f * towerLevel;
        };

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        pp.upgrades.clear();
        pp.upgrades.push_back(towers::Upgrade::Range);
        pp.upgrades.push_back(towers::Upgrade::Damage);
        pp.upgrades.push_back(towers::Upgrade::RotationSpeed);
        pp.upgrades.push_back(towers::Upgrade::AttackSpeed);
        pp.upgrades.push_back(towers::Upgrade::AimSpeed);

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
          case Upgrade::AttackSpeed:
          case Upgrade::AimSpeed:
            // This formula yields a negative value for
            // the level `0` so we clamp it to get the
            // desired output.
            return std::max(
              std::round(
                -13.7f
                + 185.0f * level
                - 61.3f * level * level
                + 8.54f * level * level * level
              ),
              42.0f
            );
          case Upgrade::Accuracy:
          case Upgrade::ProjectileSpeed:
          case Upgrade::FreezingPower:
          case Upgrade::FreezingSpeed:
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
