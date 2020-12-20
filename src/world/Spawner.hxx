#ifndef    SPAWNER_HXX
# define   SPAWNER_HXX

# include "Spawner.hh"

namespace tdef {

  inline
  Spawner::SProps
  Spawner::newProps(const utils::Point2f& p,
                    const utils::Uuid& owner) noexcept
  {
    Spawner::SProps pp;

    pp.pos = p;
    pp.radius = 1.0f;

    pp.health = 1.0f;

    pp.owner = owner;

    pp.spawnRadius = 4.0f;

    pp.threshold = 1.0f;
    pp.reserve = 0.9f;
    pp.refill = 0.0f;

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
