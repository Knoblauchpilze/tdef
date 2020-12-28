
# include "TowerAction.hh"
# include "Game.hh"

namespace tdef {

  TowerAction::TowerAction(const towers::Type& type):
    Action("tower_action"),

    m_type(type)
  {}

  void
  TowerAction::apply(Game& g) const {
    g.setTowerType(m_type);
  }

}
