
# include "MobData.hh"

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
  }

  namespace fast {
    static const bool poisonable = true;
    static const bool slowable = true;
    static const bool stunnable = true;

    static const float bounty = 0.7f;
    static const float speed = 1.25f;
    static const float lives = 1.0f;

    static const float health = 0.8f;
    static const float shield = 0.0f;
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
  }

  namespace icy {
    static const bool poisonable = true;
    static const bool slowable = false;
    static const bool stunnable = false;

    static const float bounty = 0.8f;
    static const float speed = 1.05f;
    static const float lives = 1.0f;

    static const float health = 0.8f;
    static const float shield = 0.4f;
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
  }

}
