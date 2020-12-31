#ifndef    MENU_HXX
# define   MENU_HXX

# include "Menu.hh"

namespace tdef {

  inline
  Menu::~Menu() {
    clearContent();
    clear();
  }

  inline
  void
  Menu::setVisible(bool visible) noexcept {
    m_state.visible = visible;
  }

  inline
  olc::vf2d
  Menu::getSize() const noexcept {
    return m_size;
  }

  inline
  void
  Menu::setContent(const menu::MenuContentDesc& mcd) {
    clearContent();
    m_fg = mcd;
    loadFGTile();
  }

  inline
  void
  Menu::onClick(std::vector<ActionShPtr>& /*actions*/) const {
    // Nothing to do.
  }

  inline
  olc::vi2d
  Menu::absolutePosition() const noexcept {
    olc::vi2d p(0, 0);

    if (m_parent != nullptr) {
      p = m_parent->absolutePosition();
    }

    p += m_pos;

    return p;
  }

  inline
  void
  Menu::clear() {}

  inline
  void
  Menu::clearContent() {
    if (m_fgSprite != nullptr) {
      delete m_fgSprite;
    }

    m_fgSprite = nullptr;
  }

}

#endif    /* MENU_HXX */
