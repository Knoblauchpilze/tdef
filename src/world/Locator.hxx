#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"
# include <algorithm>
# include "Mob.hh"
# include "Spawner.hh"
# include "Portal.hh"
# include <maths_utils/LocationUtils.hh>

namespace tdef {

  inline
  int
  Locator::w() const noexcept {
    return m_w;
  }

  inline
  int
  Locator::h() const noexcept {
    return m_h;
  }

  inline
  world::Block
  Locator::block(int id) const noexcept {
    BlockShPtr b = m_blocks[id];

    world::BlockType bt = world::BlockType::Wall;
    if (std::dynamic_pointer_cast<Spawner>(b) != nullptr) {
      bt = world::BlockType::Spawner;
    }
    else if (std::dynamic_pointer_cast<Portal>(b) != nullptr) {
      bt = world::BlockType::Portal;
    }

    world::Block bd{
      b->getPos(),         // p
      b->getRadius(),      // radius
      b->getHealthRatio(), // health
      bt                   // type
    };

    return bd;
  }

  inline
  world::Mob
  Locator::mob(int id) const noexcept {
    MobShPtr m = m_mobs[id];

    world::MobType mt = world::MobType::Fast;
    // TODO: Handle mobe type.

    world::Mob md{
      m->getPos(),
      m->getRadius(),
      m->getHealthRatio(),
      mt,
      m->getPath()
    };

    return md;
  }

  inline
  void
  Locator::refresh() {
    m_blocksIDs.clear();
    initialize();
  }

  inline
  bool
  Locator::obstructed(float x, float y) const noexcept {
    // Convert input coordinates to integer cell
    // coordinates.
    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);

    // Verify that no block occupies this location.
    return m_blocksIDs.count(yi * m_w + xi) > 0;
  }

  inline
  bool
  Locator::obstructed(const utils::Point2f& p) const noexcept {
    return obstructed(p.x(), p.y());
  }

  inline
  bool
  Locator::obstructed(utils::Point2f p,
                      utils::Point2f e,
                      std::vector<utils::Point2f>& cPoints,
                      utils::Point2f* obs,
                      float sample,
                      bool allowLog) const noexcept
  {
    // Convert the segment defined by `p` and `e` to
    // a direction syntax.
    float xD, yD, d;
    utils::toDirection(p, e, xD, yD, d);

    // Use the dedicated variable.
    return obstructed(p, xD, yD, d, cPoints, obs, sample, allowLog);
  }

  inline
  world::ItemEntry
  Locator::getClosest(const utils::Point2f& p,
                      const world::ItemType& type,
                      const world::Filter& filter) const noexcept
  {
    // Use the dedicated handler.
    std::vector<world::ItemEntry> all = getVisible(p, -1.0f, &type, &filter, world::Sort::Distance);

    // Return the closest one if any has
    // been found or an invalid entry. As
    // we requested the list to be sorted
    // we can pick the first element.
    if (all.empty()) {
      return world::ItemEntry{type, -1};
    }

    return all.front();
  }

  inline
  std::vector<BlockShPtr>
  Locator::getVisibleBlocks(const utils::Point2f& p,
                            float r,
                            const world::Filter* filter,
                            world::Sort sort) const noexcept
  {
    // Fetch visible blocks descriptions.
    world::ItemType t = world::ItemType::Block;
    std::vector<world::ItemEntry> bds = getVisible(p, r, &t, filter, sort);

    std::vector<BlockShPtr> bs;
    for (unsigned i = 0u ; i < bds.size() ; ++i) {
      BlockShPtr b = m_blocks[bds[i].index];
      bs.push_back(b);
    }

    return bs;
  }

  inline
  std::vector<MobShPtr>
  Locator::getVisibleMobs(const utils::Point2f& p,
                          float r,
                          const world::Filter* filter,
                          world::Sort sort) const noexcept
  {
    // Fetch visible entities descriptions.
    world::ItemType t = world::ItemType::Mob;
    std::vector<world::ItemEntry> mds = getVisible(p, r, &t, filter, sort);

    std::vector<MobShPtr> ms;
    for (unsigned i = 0u ; i < mds.size() ; ++i) {
      MobShPtr m = m_mobs[mds[i].index];
      ms.push_back(m);
    }

    return ms;
  }

  inline
  BlockShPtr
  Locator::getClosestBlock(const utils::Point2f& p,
                           float r,
                           const world::Filter* filter) const noexcept
  {
    std::vector<BlockShPtr> bs = getVisibleBlocks(p, r, filter, world::Sort::Distance);

    if (bs.empty()) {
      return nullptr;
    }

    return bs.front();
  }

  inline
  MobShPtr
  Locator::getClosestMob(const utils::Point2f& p,
                         float r,
                         const world::Filter* filter) const noexcept
  {
    std::vector<MobShPtr> ms = getVisibleMobs(p, r, filter, world::Sort::Distance);

    if (ms.empty()) {
      return nullptr;
    }

    return ms.front();
  }

}

#endif    /* LOCATOR_HXX */
