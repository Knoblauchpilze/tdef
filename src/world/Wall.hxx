#ifndef    WALL_HXX
# define   WALL_HXX

# include "Wall.hxx"

namespace tdef {

  BlockShPtr
  newWall(const utils::Point2f& pos,
          const utils::Uuid& owner)
  {
    return std::make_shared<Block>(pos, "wall", owner);
  }

}

#endif    /* Wall_HXX */
