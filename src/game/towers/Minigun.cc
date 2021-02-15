
# include "Minigun.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace minigun {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 3.5f;

      constexpr float damage = 9.2f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = 4.5f;
      constexpr float rotation = utils::degToRad(40.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = infinite_projectile_speed;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 0.0f;
      constexpr float shootAngle = utils::degToRad(0.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.18f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.0f;

      constexpr float critProb = 0.0f;
      constexpr float critMultiplier = 1.0f;

      constexpr float cost = 110.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Minigun);

        pp.energy = 1.0f;
        pp.maxEnergy = 1.0f;
        pp.refill = buildLinearUpgradable(0.248f, 4.37f, attackSpeed);

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildQuadraticUpgradable(0.0168f, 0.0158f, 3.62f, maxRange);

        pp.damage = buildQuadraticUpgradable(0.966f, -0.588f, 11.0f, damage);
        pp.aoeRadius = buildConstantUpgradable(aoeRadius);

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = true;

        pp.rotationSpeed = buildLinearUpgradable(utils::degToRad(2.2f), utils::degToRad(40.9f), rotation);
        pp.aimSpeed = buildConstantUpgradable(aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = buildConstantUpgradable(duration);
        pp.shootAngle = buildConstantUpgradable(shootAngle);
        pp.projectiles = buildConstantUpgradable(projectiles);
        pp.acceleration = [](int /*upgradeLevel*/, int towerLevel) {
          return acceleration + 0.01f * towerLevel;
        };

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
            return std::round(
              10.3f
              + 138.0f * level
              - 54.1f * level * level
              + 8.63f * level * level * level
            );
          case Upgrade::AimSpeed:
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
