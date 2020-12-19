#ifndef    WALL_HH
# define   WALL_HH

# include <core_utils/Uuid.hh>
# include "Block.hh"

namespace tdef {

  /**
   * @brief - Generates a new wall at the specified position.
   * @param pos - the position of the wall.
   * @param owner - the owner of this wall, no owner by def.
   * @return - the created wall.
   */
  BlockShPtr
  newWall(const utils::Point2f& pos,
          const utils::Uuid& owner = utils::Uuid());

}

# include "Wall.hxx"

#endif    /* Wall_HH */
