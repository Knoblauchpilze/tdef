
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

    m_enabled(enabled),

    m_activeBGColor(sk_activeBGColor),
    m_disabledBGColor(sk_disabledBGColor),
    m_activeTextColor(sk_activeTextColor),
    m_disabledTextColor(sk_disabledTextColor)
  {}

  GameMenu::GameMenu(const menu::BackgroundDesc& bg,
                     const menu::MenuContentDesc& fg,
                     const menu::RegisterAction& cb,
                     Menu* parent,
                     bool enabled):
    SimpleMenu(
      olc::vi2d(),
      olc::vf2d(),
      bg,
      fg,
      cb,
      parent
    ),

    m_enabled(enabled),

    m_activeBGColor(bg.color),
    m_disabledBGColor(sk_disabledBGColor),
    m_activeTextColor(fg.color),
    m_disabledTextColor(sk_disabledTextColor)
  {}

  void
  GameMenu::enable(bool enabled) {
    // Speed up similar assignment of the status.
    if (enabled == m_enabled) {
      return;
    }

    menu::BackgroundDesc bg;
    bg = menu::newColoredBackground(enabled ? m_activeBGColor : m_disabledBGColor);

    menu::MenuContentDesc fg;
    fg = menu::newTextContent(
      getText(),
      enabled ? m_activeTextColor : m_disabledTextColor
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
