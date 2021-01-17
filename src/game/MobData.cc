
# include "MobData.hh"

namespace tdef {
  namespace mobs {

    namespace regular {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.0f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 1.0f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Regular);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace fast {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 0.7f;
      static const float speed = 1.25f;
      static const float lives = 1.0f;

      static const float health = 800.8f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Fast);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace strong {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.2f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 1.2f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Strong);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace heli {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = false;

      static const float bounty = 1.0f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 1.0f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Heli);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace jet {
      static const bool poisonable = true;
      static const bool slowable = false;
      static const bool stunnable = false;

      static const float bounty = 1.0f;
      static const float speed = 1.20f;
      static const float lives = 1.0f;

      static const float health = 0.8f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Jet);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace armored {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.1f;
      static const float speed = 0.95f;
      static const float lives = 1.0f;

      static const float health = 1.1f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Armored);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace healer {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.15f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 0.95f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Healer);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace toxic {
      static const bool poisonable = false;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.0f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 1.0f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Toxic);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace icy {
      static const bool poisonable = true;
      static const bool slowable = false;
      static const bool stunnable = false;

      static const float bounty = 0.8f;
      static const float speed = 1.05f;
      static const float lives = 1.0f;

      static const float health = 0.8f;
      static const float shield = 0.5f;
      static const float shieldEfficiency = 0.8f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Icy);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace fighter {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.0f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 1.0f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Fighter);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

        pp.shield = shield;
        pp.shieldEfficiency = shieldEfficiency;

        pp.poisonable = poisonable;
        pp.slowable = slowable;
        pp.stunnable = stunnable;

        return pp;
      }

    }

    namespace light {
      static const bool poisonable = true;
      static const bool slowable = true;
      static const bool stunnable = true;

      static const float bounty = 1.1f;
      static const float speed = 1.0f;
      static const float lives = 1.0f;

      static const float health = 0.95f;
      static const float shield = 0.0f;
      static const float shieldEfficiency = 0.0f;

      Mob::MProps
      generateProps(const utils::Point2f& p, int /*level*/) noexcept {
        Mob::MProps pp = Mob::newProps(p, Type::Light);

        pp.health = health;

        pp.speed = speed;
        pp.bounty = bounty;

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
