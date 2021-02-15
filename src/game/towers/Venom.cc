
# include "Venom.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace venom {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 2.125f;

      constexpr float damage = 12.2f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = 0.45f;
      constexpr float rotation = utils::degToRad(75.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = 1.25f;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 5000.0f;
      constexpr float shootAngle = utils::degToRad(0.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.0f;

      constexpr float critProb = 0.0f;
      constexpr float critMultiplier = 1.0f;

      constexpr float cost = 100.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Venom);

        pp.energy = 1.0f;
        pp.maxEnergy = 1.0f;
        pp.refill = buildLinearUpgradable(0.0684f, 0.381f, attackSpeed);

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildQuadraticUpgradable(3.23e-03f, 0.0811f, 2.13f, maxRange);

        pp.damage = buildQuadraticUpgradable(1.48f, -2.44f, 15.4f, damage);
        pp.aoeRadius = buildConstantUpgradable(aoeRadius);

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = true;

        pp.rotationSpeed = buildLinearUpgradable(utils::degToRad(4.82f), utils::degToRad(74.5f), rotation);
        pp.aimSpeed = buildConstantUpgradable(aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = [](int /*upgradeLevel*/, int towerLevel) {
          return duration + 300.0f * towerLevel;
        };
        pp.shootAngle = buildConstantUpgradable(shootAngle);
        pp.projectiles = buildConstantUpgradable(projectiles);
        pp.acceleration = buildConstantUpgradable(acceleration);

        pp.freezePercent = buildConstantUpgradable(freezePercent);
        pp.freezeSpeed = buildConstantUpgradable(freezeSpeed);

        pp.stunProb = buildConstantUpgradable(stunProb);

        pp.critProb = buildConstantUpgradable(critProb);
        pp.critMultiplier = buildConstantUpgradable(critMultiplier);

        pp.attackCost = 1.0f;

        pp.cost = cost;

        pp.upgrades.clear();
        pp.upgrades.push_back(towers::Upgrade::Range);
        pp.upgrades.push_back(towers::Upgrade::Damage);
        pp.upgrades.push_back(towers::Upgrade::RotationSpeed);
        pp.upgrades.push_back(towers::Upgrade::AttackSpeed);
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
          case Upgrade::AttackSpeed:
          case Upgrade::ProjectileSpeed:
            return std::round(
              8.38f
              + 127.0f * level
              - 47.8f * level * level
              + 7.29f * level * level * level
            );
          case Upgrade::AimSpeed:
          case Upgrade::Accuracy:
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
