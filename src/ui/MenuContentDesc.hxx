#ifndef    MENU_CONTENT_DESC_HXX
# define   MENU_CONTENT_DESC_HXX

# include "MenuContentDesc.hh"

namespace tdef {
  namespace menu {

    inline
    MenuContentDesc
    newTextContent(const std::string& text,
                   const olc::Pixel& color,
                   const Alignment& align) noexcept
    {
      MenuContentDesc m;

      m.text = text;
      m.color = color;

      m.icon = "";
      m.size = olc::vi2d();

      m.align = align;
      m.order = Ordering::TextFirst;

      m.expand = true;

      return m;
    }

    inline
    MenuContentDesc
    newImageContent(const std::string& sprite, const olc::vi2d& size) noexcept {
      MenuContentDesc m;

      m.text = "";
      m.color = olc::MAGENTA;

      m.icon = sprite;
      m.size = size;

      m.align = Alignment::Left;
      m.order = Ordering::ImageFirst;

      m.expand = true;

      return m;
    }

    inline
    MenuContentDesc
    newMenuContent(const std::string& text,
                   const std::string& sprite,
                   const olc::vi2d& size,
                   const Alignment& align) noexcept
    {
      MenuContentDesc m;

      m.text = text;
      m.color = olc::MAGENTA;

      m.icon = sprite;
      m.size = size;

      m.align = align;
      m.order = Ordering::ImageFirst;

      m.expand = true;

      return m;
    }

  }
}

#endif    /* MENU_CONTENT_DESC_HXX */
