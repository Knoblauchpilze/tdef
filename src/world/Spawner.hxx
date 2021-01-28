#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace tdef {
  namespace spawners {

    inline
    std::string
    toString(const Level& l) noexcept {
      switch (l) {
        case Level::Easy:
          return "Easy";
        case Level::Normal:
          return "Normal";
        case Level::Hard:
          return "Hard";
        default:
          return "Unknown";
      }
    }

  }

  inline
  Spawner::SProps
  Spawner::newProps(const utils::Point2f& p,
                    const spawners::Distribution& dist,
                    const utils::Uuid& owner) noexcept
  {
    SProps pp;
    assignProps(pp, p, owner);

    pp.spawnRadius = 4.0f;

    pp.threshold = 1.0f;
    pp.reserve = 0.9f;
    pp.refill = 0.1f;

    pp.mobs = dist;

    return pp;
  }

  inline
  float
  Spawner::refill(float delta, bool force) {
    // In case the `delta` is positive the operation
    // always succeed.
    if (delta >= 0.0f) {
      m_stock += delta;

      return delta;
    }

    // Otherwise we have to have a sufficient amount
    // of resource to perform the operation. In case
    // the `force` is `true` we will decrease the
    // stock no matter if it becomes negative. If we
    // don't force the update then we only make the
    // stock `0`.
    if (m_stock < std::abs(delta) && force) {
      m_stock += delta;

      return delta;
    }

    float var = m_stock;
    m_stock = std::max(m_stock + delta, 0.0f);

    return var;
  }

}

#endif    /* SPAWNER_HXX */
