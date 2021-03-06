
# include "Freezing.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"
# include "Locator.hh"

namespace tdef {
  namespace towers {

    namespace freezing {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 2.0f;

      constexpr float damage = 0.0f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = infinite_attack_speed;
      constexpr float rotation = utils::degToRad(0.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = infinite_projectile_speed;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 2000.0f;
      constexpr float shootAngle = utils::degToRad(360.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = freezePercentageToSpeedRatio(20.0f);
      constexpr float freezeSpeed = 0.12f;

      constexpr float stunProb = 0.0f;

      constexpr float critProb = 0.0f;
      constexpr float critMultiplier = 1.0f;

      constexpr float cost = 80.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = multipleTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Freezing);

        pp.energy = 1.0f;
        pp.maxEnergy = 1.0f;
        pp.refill = buildConstantUpgradable(attackSpeed);

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildQuadraticUpgradable(1.57e-3f, 0.101f, 2.02f, maxRange);

        pp.damage = buildConstantUpgradable(damage);
        pp.aoeRadius = buildConstantUpgradable(aoeRadius);

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = false;

        pp.rotationSpeed = buildConstantUpgradable(rotation);
        pp.aimSpeed = buildConstantUpgradable(aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = buildConstantUpgradable(duration);
        pp.shootAngle = buildConstantUpgradable(shootAngle);
        pp.projectiles = buildConstantUpgradable(projectiles);
        pp.acceleration = buildConstantUpgradable(acceleration);

        // We will create the expression right here as we need
        // to convert from the raw value to the freeze percent.
        pp.freezePercent = [](int upgradeLevel, int /*towerLevel*/) {
          if (upgradeLevel == 0) {
            return freezePercent;
          }

          return freezePercentageToSpeedRatio(2.5f * upgradeLevel + 20.2f);
        };
        pp.freezeSpeed = buildLinearUpgradable(0.02f, 0.12f, freezeSpeed);

        pp.stunProb = buildConstantUpgradable(stunProb);

        pp.critProb = buildConstantUpgradable(critProb);
        pp.critMultiplier = buildConstantUpgradable(critMultiplier);

        // The attack costs nothing for the freezing tower.
        pp.attackCost = 0.0f;

        pp.cost = cost;

        pp.upgrades.clear();
        pp.upgrades.push_back(towers::Upgrade::Range);
        pp.upgrades.push_back(towers::Upgrade::FreezingPower);
        pp.upgrades.push_back(towers::Upgrade::FreezingSpeed);

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
          case Upgrade::FreezingPower:
          case Upgrade::FreezingSpeed:
            return std::round(
              9.72f
              + 140.0f * level
              - 51.3f * level * level
              + 7.62f * level * level * level
            );
          case Upgrade::Damage:
          case Upgrade::RotationSpeed:
          case Upgrade::AttackSpeed:
          case Upgrade::AimSpeed:
          case Upgrade::Accuracy:
          case Upgrade::ProjectileSpeed:
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
