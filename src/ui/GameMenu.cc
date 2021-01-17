
# include "GameMenu.hh"

namespace tdef {

  const olc::Pixel GameMenu::sk_activeBGColor = olc::Pixel(147, 196, 125);
  const olc::Pixel GameMenu::sk_activeTextColor = olc::BLACK;
  const olc::Pixel GameMenu::sk_disabledBGColor = olc::Pixel(224, 102, 102);
  const olc::Pixel GameMenu::sk_disabledTextColor = olc::VERY_DARK_GREY;

  GameMenu::GameMenu(const std::string& text,
                     const menu::RegisterAction& cb,
                     Menu* parent,
                     bool enabled):
    SimpleMenu(
      olc::vi2d(),
      olc::vf2d(),
      menu::newColoredBackground(enabled ? sk_activeBGColor : sk_disabledBGColor),
      menu::newTextContent(text, enabled ? sk_activeTextColor : sk_disabledTextColor),
      cb,
      parent
    ),

    m_enabled(enabled)
  {}

  void
  GameMenu::enable(bool enabled) {
    // Speed up similar assignment of the status.
    if (enabled == m_enabled) {
      return;
    }

    menu::BackgroundDesc bg;
    bg = menu::newColoredBackground(enabled ? sk_activeBGColor : sk_disabledBGColor);

    menu::MenuContentDesc fg;
    fg = menu::newTextContent(
      getText(),
      enabled ? sk_activeTextColor : sk_disabledTextColor
    );

    setBackground(bg);
    setContent(fg);

    m_enabled = enabled;
  }

  bool
  GameMenu::onHighlight() const {
    return m_enabled;
  }

}
