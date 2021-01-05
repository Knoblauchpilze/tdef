
# include "Block.hh"

namespace tdef {

  Block::Block(const BProps& props,
               const std::string& name):
    WorldElement(props, name),

    m_orientation(props.orientation),
    m_rotationSpeed(props.rotation)
  {}

}
