
# include "Regular.hh"

namespace tdef {
  namespace mobs {

    namespace regular {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.0f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float attack = 0.2f;
      static constexpr float attackSpeed = 0.5f;

      static constexpr float health = 1.0f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Regular);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.attack = attack;
        pp.bounty = bounty;
        pp.exp = exp;

        // Convert attack speed to attack cost and
        // energy props. The attack speed defines
        // how many attacks are performed per sec
        // so we can just use this as the refill
        // speed as we have a max energy of `1`.
        pp.maxEnergy = 1.0f;
        pp.energy = 0.9f;
        pp.refill = attackSpeed;
        pp.attackCost = 1.0f;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

  }
}
