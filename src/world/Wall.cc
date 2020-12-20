
# include "Wall.hh"

namespace tdef {

  Wall::Wall(const utils::Point2f& pos,
             float height,
             const utils::Uuid& owner):
    Block(pos, "wall", owner),

    m_height(height)
  {}

}
