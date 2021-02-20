
# include "Blast.hh"
# include <maths_utils/AngleUtils.hh>
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    namespace blast {
      constexpr float minRange = 0.0f;
      constexpr float maxRange = 1.65f;

      constexpr float damage = 18.0f;
      constexpr float aoeRadius = 0.0f;

      constexpr float attackSpeed = 0.5f;
      constexpr float rotation = utils::degToRad(0.0f);
      constexpr float aimSpeed = infinite_aim_speed;
      constexpr float projectileSpeed = infinite_projectile_speed;
      constexpr float accuracy = 1.0f;

      constexpr float duration = 2000.0f;
      constexpr float shootAngle = utils::degToRad(360.0f);
      constexpr int projectiles = 1;
      constexpr float acceleration = 0.0f;

      constexpr float freezePercent = no_speed_decrease;
      constexpr float freezeSpeed = 0.0f;

      constexpr float stunProb = 0.15f;

      constexpr float critProb = 0.0f;
      constexpr float critMultiplier = 1.0f;

      constexpr float cost = 75.0f;

      Processes
      generateData(int /*level*/) noexcept {
        Processes dd;

        dd.pickMob = multipleTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Blast);

        pp.energy = 1.0f;
        pp.maxEnergy = 1.0f;
        pp.refill = buildLinearUpgradable(0.0699f, 0.475f, attackSpeed);

        pp.minRange = buildConstantUpgradable(minRange);
        pp.maxRange = buildCubicUpgradable(-1.02e-3f, 0.0162f, -0.0466f, 1.67f, maxRange);

        pp.damage = buildCubicUpgradable(0.283f, -1.22f, 9.01f, 17.1f, damage);
        pp.aoeRadius = buildConstantUpgradable(aoeRadius);

        pp.targetting = towers::Targetting::First;
        pp.persistTargets = false;

        pp.rotationSpeed = buildConstantUpgradable(rotation);
        pp.aimSpeed = buildConstantUpgradable(aimSpeed);
        pp.projectileSpeed = buildConstantUpgradable(projectileSpeed);
        pp.accuracy = buildConstantUpgradable(accuracy);

        pp.duration = [](int upgradeLevel, int /*towerLevel*/) {
          return duration + 100.0f * upgradeLevel;
        };
        pp.shootAngle = buildConstantUpgradable(shootAngle);
        pp.projectiles = buildConstantUpgradable(projectiles);
        pp.acceleration = buildConstantUpgradable(acceleration);

        pp.freezePercent = buildConstantUpgradable(freezePercent);
        pp.freezeSpeed = buildConstantUpgradable(freezeSpeed);

        pp.stunProb = [](int /*upgradeLevel*/, int towerLevel) {
          return stunProb + 0.01f * towerLevel;
        };

        pp.critProb = buildConstantUpgradable(critProb);
        pp.critMultiplier = buildConstantUpgradable(critMultiplier);

        pp.attackCost = 1.0f;

        pp.cost = cost;

        pp.upgrades.clear();
        pp.upgrades.push_back(towers::Upgrade::Range);
        pp.upgrades.push_back(towers::Upgrade::Damage);
        pp.upgrades.push_back(towers::Upgrade::AttackSpeed);
        pp.upgrades.push_back(towers::Upgrade::StunChance);
        pp.upgrades.push_back(towers::Upgrade::StunDuration);

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
          case Upgrade::AttackSpeed:
          case Upgrade::StunChance:
          case Upgrade::StunDuration:
            // This formula yields a negative value for
            // the level `0` so we clamp it to get the
            // desired output.
            return std::max(
              std::round(
                -11.6f
                + 168.0f * level
                - 57.2f * level * level
                + 8.86f * level * level * level
              ),
              34.0f
            );
          case Upgrade::RotationSpeed:
          case Upgrade::AimSpeed:
          case Upgrade::Accuracy:
          case Upgrade::ProjectileSpeed:
          case Upgrade::FreezingPower:
          case Upgrade::FreezingSpeed:
          default:
            // Default upgrade cost is null.
            return 0.0f;
        }
      }

    }

  }
}
