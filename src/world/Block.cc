
# include "Block.hh"

namespace tdef {

  Block::Block(const utils::Point2f& pos,
               const std::string& name,
               const utils::Uuid& owner):
    WorldElement(pos, 1.0f, 1.0f, name, owner)
  {}

}
