
# include "TowerAction.hh"

namespace tdef {

  TowerAction::TowerAction(const towers::Type& type):
    Action("tower_action"),

    m_type(type)
  {}

  void
  TowerAction::apply(World& /*w*/) const {
    // TODO: Handle this.
    log("Should apply tower action with type " + std::to_string(static_cast<int>(m_type)));
  }

}
