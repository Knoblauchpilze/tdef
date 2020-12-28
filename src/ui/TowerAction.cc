
# include "TowerAction.hh"

namespace tdef {

  TowerAction::TowerAction(const towers::Type& type):
    Action("tower_action"),

    m_type(type)
  {}

  void
  TowerAction::apply(World& w) const {
    w.setTowerType(m_type);
  }

}
