
# include "MobData.hh"

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

      static constexpr float health = 1.0f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Regular);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace fast {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 0.7f;
      static constexpr float speed = 1.25f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 200.8f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Fast);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace strong {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.2f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 1.2f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Strong);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace heli {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = false;

      static constexpr float bounty = 1.0f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 1.0f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Heli);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace jet {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = false;
      static constexpr bool stunnable = false;

      static constexpr float bounty = 1.0f;
      static constexpr float speed = 1.20f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 0.8f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Jet);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace armored {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.1f;
      static constexpr float speed = 0.95f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 1.1f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Armored);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace healer {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.15f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 0.95f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Healer);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace toxic {
      static constexpr bool poisonable = false;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.0f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 1.0f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Toxic);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace icy {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = false;
      static constexpr bool stunnable = false;

      static constexpr float bounty = 0.8f;
      static constexpr float speed = 1.05f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 0.8f;
      static constexpr float shield = 0.5f;
      static constexpr float shieldEfficiency = 0.8f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Icy);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace fighter {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.0f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 1.0f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Fighter);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace light {
      static constexpr bool poisonable = true;
      static constexpr bool slowable = true;
      static constexpr bool stunnable = true;

      static constexpr float bounty = 1.1f;
      static constexpr float speed = 1.0f;
      static constexpr float acceleration = 1.0f;
      static constexpr float lives = 1.0f;
      static constexpr float exp = 1.0f;

      static constexpr float health = 0.95f;
      static constexpr float shield = 0.0f;
      static constexpr float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Light);

        pp.health = health;

        pp.speed = speed;
        pp.acceleration = acceleration;
        pp.bounty = bounty;
        pp.exp = exp;

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
