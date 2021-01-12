#ifndef    LOCATOR_HXX
# define   LOCATOR_HXX

# include "Locator.hh"
# include <algorithm>
# include "Mob.hh"
# include "Wall.hh"
# include "Spawner.hh"
# include "Portal.hh"
# include "Tower.hh"
# include <maths_utils/LocationUtils.hh>

namespace tdef {

  inline
  world::Block
  Locator::block(int id) const noexcept {
    BlockShPtr b = m_blocks[id];

    world::BlockType bt = world::BlockType::Wall;
    int var = 0;

    if (std::dynamic_pointer_cast<Spawner>(b) != nullptr) {
      bt = world::BlockType::Spawner;
    }
    else if (std::dynamic_pointer_cast<Portal>(b) != nullptr) {
      bt = world::BlockType::Portal;
    }
    else if (std::dynamic_pointer_cast<Tower>(b) != nullptr) {
      bt = world::BlockType::Tower;

      TowerShPtr t = std::dynamic_pointer_cast<Tower>(b);
      switch (t->getType()) {
        case towers::Type::Sniper:
          var = 1;
          break;
        case towers::Type::Cannon:
          var = 2;
          break;
        case towers::Type::Freezing:
          var = 3;
          break;
        case towers::Type::Venom:
          var = 4;
          break;
        case towers::Type::Splash:
          var = 5;
          break;
        case towers::Type::Blast:
          var = 6;
          break;
        case towers::Type::Multishot:
          var = 7;
          break;
        case towers::Type::Minigun:
          var = 8;
          break;
        case towers::Type::Antiair:
          var = 9;
          break;
        case towers::Type::Tesla:
          var = 10;
          break;
        case towers::Type::Missile:
          var = 11;
          break;
        case towers::Type::Basic:
        default:
          // Assume regular type.
          var = 0;
          break;
      }
    }

    world::Block bd;
    bd.p = b->getPos();
    bd.radius = b->getRadius();
    bd.orientation = b->getOrientation();
    bd.health = b->getHealthRatio();
    bd.type = bt;
    bd.id = var;

    return bd;
  }

  inline
  world::Mob
  Locator::mob(int id) const noexcept {
    MobShPtr m = m_mobs[id];

    world::Mob md;
    md.p = m->getPos();
    md.radius = m->getRadius();
    md.health = m->getHealthRatio();

    md.path = m->getPath();

    switch (m->getType()) {
      case mobs::Type::Fast:
        md.id = 1;
        break;
      case mobs::Type::Strong:
        md.id = 2;
        break;
      case mobs::Type::Heli:
        md.id = 3;
        break;
      case mobs::Type::Jet:
        md.id = 4;
        break;
      case mobs::Type::Armored:
        md.id = 5;
        break;
      case mobs::Type::Healer:
        md.id = 6;
        break;
      case mobs::Type::Toxic:
        md.id = 7;
        break;
      case mobs::Type::Icy:
        md.id = 8;
        break;
      case mobs::Type::Fighter:
        md.id = 9;
        break;
      case mobs::Type::Light:
        md.id = 10;
        break;
      case mobs::Type::Regular:
      default:
        // Assume regular type.
        md.id = 0;
        break;
    }

    return md;
  }

  inline
  world::Projectile
  Locator::projectile(int id) const noexcept {
    ProjectileShPtr p = m_projectiles[id];

    world::Projectile pd;
    pd.p = p->getPos();

    return pd;
  }

  inline
  WorldElementShPtr
  Locator::itemAt(const utils::Point2f& p, bool includeMobs) const noexcept {
    return itemAt(p.x(), p.y(), includeMobs);
  }

  inline
  bool
  Locator::obstructed(float x, float y, bool includeMobs) const noexcept {
    return itemAt(x, y, includeMobs) != nullptr;
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
                            const world::BlockType& type,
                            const world::Filter* filter,
                            world::Sort sort) const noexcept
  {
    // Fetch visible blocks descriptions.
    world::ItemType t = world::ItemType::Block;
    std::vector<world::ItemEntry> bds = getVisible(p, r, &t, filter, sort);

    std::vector<BlockShPtr> bs;
    for (unsigned i = 0u ; i < bds.size() ; ++i) {
      BlockShPtr b = m_blocks[bds[i].index];

      // Check whether the type matches what is desired.
      bool valid = false;
      switch (type) {
        case world::BlockType::Spawner:
          valid = (std::dynamic_pointer_cast<Spawner>(b) != nullptr);
          break;
        case world::BlockType::Wall:
          valid = (std::dynamic_pointer_cast<Wall>(b) != nullptr);
          break;
        case world::BlockType::Portal:
          valid = (std::dynamic_pointer_cast<Portal>(b) != nullptr);
          break;
        case world::BlockType::Tower:
          valid = (std::dynamic_pointer_cast<Tower>(b) != nullptr);
          break;
      }

      if (!valid) {
        continue;
      }

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
                           const world::BlockType& type,
                           float r,
                           const world::Filter* filter) const noexcept
  {
    std::vector<BlockShPtr> bs = getVisibleBlocks(p, r, type, filter, world::Sort::Distance);

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
