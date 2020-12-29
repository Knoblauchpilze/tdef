
# include "SimpleAction.hh"
# include "Game.hh"

namespace tdef {

  SimpleAction::SimpleAction(const action::Process& cb):
    Action("simple_action"),

    m_callback(cb)
  {}

  void
  SimpleAction::apply(Game& g) const {
    m_callback(g);
  }

}
