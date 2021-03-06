
# include "Cannon.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace cannon {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 1.7f;

      constexpr float damage = 12.5f;
      constexpr float aoeRadius = 0.6f;

      constexpr float attackSpeed = 0.6f;
      constexpr float rotation = utils::degToRad(50.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = 2.4f;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 0.0f;
      constexpr float shootAngle = utils::degToRad(0.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.0f;

      constexpr float critProb = 0.0f;
      constexpr float critMultiplier = 1.0f;

      constexpr float cost = 60.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Cannon);

        pp.energy = 1.0f;
        pp.maxEnergy = 1.0f;
        pp.refill = buildLinearUpgradable(0.0859f, 0.607f, attackSpeed);

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildQuadraticUpgradable(7.34e-3f, 0.0775f, 1.77f, maxRange);

        pp.damage = buildQuadraticUpgradable(2.18f, -1.79f, 17.8f, damage);
        pp.aoeRadius = [](int /*upgradeLevel*/, int towerLevel) {
          return aoeRadius + 0.04f * towerLevel;
        };

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = true;

        pp.rotationSpeed = buildLinearUpgradable(utils::degToRad(8.73f), utils::degToRad(59.5f), rotation);
        pp.aimSpeed = buildConstantUpgradable(aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = buildConstantUpgradable(duration);
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
              13.7f
              + 124.0f * level
              - 44.7f * level * level
              + 7.49f * level * level * level
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
