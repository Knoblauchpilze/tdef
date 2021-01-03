
# include "TowerData.hh"
# include "Tower.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace tdef {
  namespace towers {

    static const float INFINITE_PROJECTILE_SPEED = 100.0f;
    static const float INFINITE_AIM_SPEED = 0.0f;
    static const float INFINITE_ATTACK_SPEED = 100.0f;

    MobShPtr
    basicTargetPicking(StepInfo& info, PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.range, nullptr);
    }

    bool
    basicDamaging(StepInfo& /*info*/, MobShPtr mob, DamageData& data) {
      return mob->damage(data.attack);
    }

    namespace basic {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 7.5f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 1.25f;
      static const float rotation = 90.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 3.0f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Basic);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace sniper {
      static const float minRange = 0.0f;
      static const float maxRange = 4.5f;

      static const float damage = 35.0f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 1.0f;

      static const float attackSpeed = 0.3f;
      static const float rotation = 50.0f;
      static const float aimSpeed = 100.0f;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Sniper);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace cannon {
      static const float minRange = 0.0f;
      static const float maxRange = 1.7f;

      static const float damage = 12.5f;
      // TODO: 0.6 + 0.04 * Tower level.
      static const float aoeRadius = 0.6f;
      static const float aoeDamage = 1.0f;

      static const float attackSpeed = 0.6f;
      static const float rotation = 50.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.4f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Cannon);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace freezing {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 0.0f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = INFINITE_ATTACK_SPEED;
      static const float rotation = 0.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = 20.0f;
      static const float freezeSpeed = 12.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Freezing);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace venom {
      static const float minRange = 0.0f;
      static const float maxRange = 2.125f;

      static const float damage = 12.2f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 0.45f;
      static const float rotation = 75.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 1.25f;
      static const float accuracy = 1.0f;

      // TODO: 5.0 + 0.3 / Tower level ?
      static const float duration = 5.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Venom);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace splash {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 14.0f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 0.9f;
      static const float rotation = 0.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 1.5f;
      static const float accuracy = 0.5f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      // TODO: 6 + 0.35 * Tower level
      static const int projectiles = 6;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Splash);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace blast {
      static const float minRange = 0.0f;
      static const float maxRange = 1.65f;

      static const float damage = 18.0f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 0.5f;
      static const float rotation = 0.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Blast);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace multishot {
      static const float minRange = 0.0f;
      static const float maxRange = 3.0f;

      static const float damage = 5.7f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 1.1f;
      static const float rotation = 50.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.0f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      // TODO: 65 + 1 / Tower level.
      static const float shootAngle = 65.0f;
      // TODO: 5 + 0.2 * Tower level.
      static const int projectiles = 5;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Multishot);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace minigun {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 9.2f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 4.5f;
      static const float rotation = 40.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      // TODO: 18 + Tower level.
      static const float acceleration = 18.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Minigun);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace antiair {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 4.2f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.0f;

      static const float attackSpeed = 1.8f;
      static const float rotation = 75.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.3f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Antiair);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace tesla {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 5.0f;
      static const float aoeRadius = 0.0f;
      static const float aoeDamage = 0.5f;

      static const float attackSpeed = 1.5f;
      static const float rotation = 60.0f;
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Tesla);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

    namespace missile {
      static const float minRange = 1.5f;
      static const float maxRange = 4.7f;

      static const float damage = 35.0f;
      // TODO: 0.73 + 0.045 / Tower level ?
      static const float aoeRadius = 0.73f;
      static const float aoeDamage = 1.0f;

      static const float attackSpeed = 0.28f;
      static const float rotation = 45.0f;
      // TODO: 12 + 0.4 / Tower level.
      static const float aimSpeed = 12.0f;
      static const float projectileSpeed = 1.8f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = 0.0f;
      static const float freezeSpeed = 0.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, towers::Type::Missile);

        pp.energy = attackSpeed;
        pp.maxEnergy = attackSpeed;
        pp.refill = 1.0f;

        pp.range = maxRange;
        pp.attack = damage;
        pp.attackCost = attackSpeed;

        return pp;
      }

    }

  }
}
