#ifndef    WALL_HXX
# define   WALL_HXX

# include "Wall.hxx"

namespace tdef {

  inline
  float
  Wall::getHeight() const noexcept {
    return m_height;
  }

}

#endif    /* Wall_HXX */
