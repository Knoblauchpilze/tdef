
# include "GameMenu.hh"

namespace tdef {

  const olc::Pixel GameMenu::sk_activeBGColor = olc::Pixel(147, 196, 125);
  const olc::Pixel GameMenu::sk_activeTextColor = olc::BLACK;
  const olc::Pixel GameMenu::sk_disabledBGColor = olc::Pixel(224, 102, 102);
  const olc::Pixel GameMenu::sk_disabledTextColor = olc::VERY_DARK_GREY;

  GameMenu::GameMenu(const std::string& text,
                     const menu::RegisterAction& cb,
                     Menu* parent):
    SimpleMenu(
      olc::vi2d(),
      olc::vf2d(),
      menu::newColoredBackground(sk_activeBGColor),
      menu::newTextContent(text, sk_activeTextColor),
      cb,
      parent
    )
  {

  }

}
