#ifndef    WALL_HXX
# define   WALL_HXX

# include "Wall.hxx"

namespace tdef {

  inline
  Wall::WProps
  Wall::newProps(const utils::Point2f& p,
                 const utils::Uuid& owner) noexcept
  {
    Wall::WProps pp;

    pp.pos = p;
    pp.radius = 1.0f;

    pp.health = 1.0f;

    pp.owner = owner;

    pp.height = 1.0f;
    
    return pp;
  }

  inline
  float
  Wall::getHeight() const noexcept {
    return m_height;
  }

}

#endif    /* Wall_HXX */
