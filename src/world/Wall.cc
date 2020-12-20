
# include "Wall.hh"

namespace tdef {

  Wall::Wall(const WProps& props):
    Block(props, "wall"),

    m_height(props.height)
  {}

}
