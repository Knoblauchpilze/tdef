
# include "Jet.hh"

namespace tdef {
  namespace mobs {

    namespace jet {
      constexpr bool poisonable = true;
      constexpr bool slowable = false;
      constexpr bool stunnable = false;

      constexpr float bounty = 1.0f;
      constexpr float speed = 1.20f;
      constexpr float acceleration = 1.0f;
      constexpr float lives = 1.0f;
      constexpr float exp = 1.0f;

      constexpr float attack = 0.2f;
      constexpr float attackSpeed = 0.5f;

      constexpr float health = 0.8f;
      constexpr float shield = 0.0f;
      constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Jet);

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
