
# include "TowerData.hh"
# include "Tower.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace tdef {
  namespace towers {

    static constexpr const float INFINITE_PROJECTILE_SPEED = -1.0f;
    static constexpr const float INFINITE_AIM_SPEED = 0.0f;
    static constexpr const float INFINITE_ATTACK_SPEED = 100.0f;

    static constexpr const float NO_SPEED_DECREASE = 0.0f;

    float degToRad(float deg) noexcept {
      return deg * 3.1415926535f / 180.0f;
    }

    MobShPtr
    basicTargetPicking(StepInfo& info, PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.maxRange, nullptr);
    }

    bool
    basicDamaging(StepInfo& info, MobShPtr mob, DamageData& data) {
      mobs::Damage d;
      d.hit = data.damage;

      d.accuracy = data.accuracy;

      d.speed = data.speed;
      d.sDecraseSpeed = data.slowdown;
      d.sDuration = data.sDuration;

      d.pDuration = data.pDuration;

      return mob->damage(info, d);
    }

    namespace basic {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 7.5f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 1.25f;
      static const float rotation = degToRad(90.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 3.0f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 48.0f;

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

        pp.freezePercent = NO_SPEED_DECREASE;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace sniper {
      static const float minRange = 0.0f;
      static const float maxRange = 4.5f;

      static const float damage = 35.0f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 0.3f;
      static const float rotation = degToRad(50.0f);
      static const float aimSpeed = 100.0f;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 80.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace cannon {
      static const float minRange = 0.0f;
      static const float maxRange = 1.7f;

      static const float damage = 12.5f;
      // TODO: 0.6 + 0.04 * Tower level.
      static const float aoeRadius = 0.6f;

      static const float attackSpeed = 0.6f;
      static const float rotation = degToRad(50.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.4f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 60.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Cannon);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace freezing {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 0.0f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = INFINITE_ATTACK_SPEED;
      static const float rotation = degToRad(0.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = 20.0f;
      static const float freezeSpeed = 12.0f;

      static const float cost = 80.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Freezing);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace venom {
      static const float minRange = 0.0f;
      static const float maxRange = 2.125f;

      static const float damage = 12.2f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 0.45f;
      static const float rotation = degToRad(75.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 1.25f;
      static const float accuracy = 1.0f;

      // TODO: 5.0 + 0.3 / Tower level ?
      static const float duration = 5.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 100.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Venom);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace splash {
      static const float minRange = 0.0f;
      static const float maxRange = 2.0f;

      static const float damage = 14.0f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 0.9f;
      static const float rotation = degToRad(0.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 1.5f;
      static const float accuracy = 0.5f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      // TODO: 6 + 0.35 * Tower level
      static const int projectiles = 6;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 80.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Splash);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace blast {
      static const float minRange = 0.0f;
      static const float maxRange = 1.65f;

      static const float damage = 18.0f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 0.5f;
      static const float rotation = degToRad(0.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 360.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 75.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Blast);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace multishot {
      static const float minRange = 0.0f;
      static const float maxRange = 3.0f;

      static const float damage = 5.7f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 1.1f;
      static const float rotation = degToRad(50.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.0f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      // TODO: 65 + 1 / Tower level.
      static const float shootAngle = 65.0f;
      // TODO: 5 + 0.2 * Tower level.
      static const int projectiles = 5;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 90.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Multishot);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace minigun {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 9.2f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 4.5f;
      static const float rotation = degToRad(40.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      // TODO: 18 + Tower level.
      static const float acceleration = 18.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 110.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Minigun);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace antiair {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 4.2f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 1.8f;
      static const float rotation = degToRad(75.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = 2.3f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 42.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Antiair);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace tesla {
      static const float minRange = 0.0f;
      static const float maxRange = 3.5f;

      static const float damage = 5.0f;
      static const float aoeRadius = 0.0f;

      static const float attackSpeed = 1.5f;
      static const float rotation = degToRad(60.0f);
      static const float aimSpeed = INFINITE_AIM_SPEED;
      static const float projectileSpeed = INFINITE_PROJECTILE_SPEED;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 120.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Tesla);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

    namespace missile {
      static const float minRange = 1.5f;
      static const float maxRange = 4.7f;

      static const float damage = 35.0f;
      // TODO: 0.73 + 0.045 / Tower level ?
      static const float aoeRadius = 0.73f;

      static const float attackSpeed = 0.28f;
      static const float rotation = degToRad(45.0f);
      // TODO: 12 + 0.4 / Tower level.
      static const float aimSpeed = 12.0f;
      static const float projectileSpeed = 1.8f;
      static const float accuracy = 1.0f;

      static const float duration = 0.0f;
      static const float shootAngle = 0.0f;
      static const int projectiles = 1;
      static const float acceleration = 0.0f;

      static const float freezePercent = NO_SPEED_DECREASE;
      static const float freezeSpeed = 0.0f;

      static const float cost = 150.0f;

      Data
      generateData(int /*level*/) noexcept {
        Data dd;

        dd.pickMob = basicTargetPicking;
        dd.damage = basicDamaging;

        return dd;
      }

      Tower::TProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Tower::TProps pp = Tower::newProps(p, Type::Missile);

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

        pp.freezePercent = freezePercent;
        pp.freezeSpeed = freezeSpeed;

        pp.attackCost = attackSpeed;

        pp.cost = cost;

        return pp;
      }

      float
      getCost(int /*level*/) noexcept {
        return cost;
      }

    }

  }
}
