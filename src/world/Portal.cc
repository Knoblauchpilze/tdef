
# include "Portal.hh"

namespace tdef {

  Portal::Portal(const PProps& props):
    Block(props, "portal"),

    m_lives(props.lives)
  {
    setService("portal");
  }

  void
  Portal::breach(float lives) {
    m_lives -= lives;
  }

}
