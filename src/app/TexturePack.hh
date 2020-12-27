#ifndef    TEXTURE_PACK_HH
# define   TEXTURE_PACK_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "olcEngine.hh"

namespace tdef {
  namespace sprites {

    /**
     * @brief - Convenience structure allowing to represent the
     *          sprites inside a file. In addition to the name
     *          of the file the layout of the sprites is also
     *          specified so as to be able to identify tiles in
     *          a correct way.
     */
    struct Pack {
      // The `file` defines the path to the file defining the
      // textures. It should be a valid path.
      std::string file;

      // The `sSize` defines the size in pixels of a single
      // texture in the file. This allows to interpret the
      // sprites correctly.
      olc::vi2d sSize;

      // The `layout` defines the dimensions of the pack in
      // sprites. It allows to interpret coordinates so that
      // we can 'locate' a sprite in the pack.
      olc::vi2d layout;
    };

    /**
     * @brief - Convenience structure regrouping needed props
     *          to draw a sprite.
     */
    struct Sprite {
      // The `sprite` defines an identifier for the sprite. The
      // position of the sprite in the resource pack will be
      // computed from this identifier.
      int sprite;

      // The `id` allows to select a variant for the sprite. By
      // default this value is `0` meaning the principal display
      // for the sprite.
      int id;

      // The `tint` defines a color to apply to tint the sprite
      // as a whole. Can also be used to provide some alpha.
      olc::Pixel tint;
    };

  }

  class TexturePack: public utils::CoreObject {
    public:

      /**
       * @brief - Generate a new texture pack from the resource
       *          pack.
       * @param pack - defines the needed resources to describe
       *               textures aggregated in a pack.
       */
      TexturePack(const sprites::Pack& pack);

      /**
       * @brief - Detroys the texture pack and release the sprites
       *          and resources attached to it.
       */
      ~TexturePack();

      /**
       * @brief - Used to perform the drawing of the sprite as
       *          defined by the input argument using the engine.
       *          The sprite will be associated internally with
       *          the corresponding visual.
       * @param pge - the engine to use to perform the rendering.
       * @param s - the sprite to draw.
       * @param p - the position where the sprite will be drawn.
       * @param scale - defines a scaling factor to apply to the
       *                sprite.
       */
      void
      draw(olc::PixelGameEngine* pge,
           const sprites::Sprite& s,
           const olc::vf2d& p,
           const olc::vf2d& scale = olc::vf2d(1.0f, 1.0f)) const;

      /**
       * @brief - Variant of the above method to accept a scale
       *          expressed as a single float: the scale will be
       *          applied in both directions.
       * @param pge - the engine to use to perform the rendering.
       * @param s - the sprite to draw.
       * @param p - the position where the sprite will be drawn.
       * @param scale - defines a scaling factor to apply to the
       *                sprite. Both directions will be scaled
       *                uniformly.
       */
      void
      draw(olc::PixelGameEngine* pge,
           const sprites::Sprite& s,
           const olc::vf2d& p,
           float scale = 1.0f) const;

    private:

      /**
       * @brief - Performs the analysis of the file defining the
       *          sprites to load.
       * @param file - the name of the file describing the pack.
       */
      void
      load(const std::string& file);

      /**
       * @brief - Used to convert from sprite coordinates to the
       *          corresponding pixels coordinates. This method
       *          should mostly be used to locate a sprite in a
       *          resource pack. The internal layout allows to
       *          know in advance how sprites are laid out in
       *          the pack and thus find the correct location
       *          based on the coordinates of the sprite and
       *          its identifier.
       *          In order to find the correct sprite, both some
       *          coordinates and a variation id should be set
       *          to fix a single element in the sprites.
       * @param coord - the coordinates of the sprite to convert
       *                to pixels in the resource pack.
       * @param id - the index of the variation of the sprite
       *             to use: default is `0`.
       * @return - a vector representing the pixels coordinates
       *           for the input sprite coords.
       */
      olc::vi2d
      spriteCoords(const olc::vi2d& coord,
                   int id = 0) const noexcept;
  
    private:

      /**
       * @brief - The sprite size defines the dimensions of each
       *          individual sprite defined in the texture pack.
       *          Each sprite should have an identical size as
       *          we only define one size for the whole pack.
       */
      olc::vi2d m_sSize;

      /**
       * @brief - Defines the layout of the resource pack linked
       *          to this pack. It helps interpreting the coords
       *          provided to reference a tile.
       */
      olc::vi2d m_layout;

      /**
       * @brief - Defines the pge object representing the texture
       *          pack. This object is created during the load of
       *          the pack and released when the pack is freed.
       */
      olc::Decal* m_pack;
  };

  using TexturePackShPtr = std::shared_ptr<TexturePack>;
}

# include "TexturePack.hxx"

#endif    /* TEXTURE_PACK_HH */
