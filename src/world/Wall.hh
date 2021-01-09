#ifndef    WALL_HH
# define   WALL_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"

namespace tdef {

  class Wall: public Block {
    public:

      /**
       * @brief - Definition of new props to define a wall.
       */
      struct WProps: public Block::BProps {
        float height;
      };

      static
      WProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new wall with the specified props.
       * @param props - the properties defining this wall.
       */
      Wall(const WProps& props);

      float
      getHeight() const noexcept;

    private:

      /**
       * @brief - The height of the wall. The higher the wall the
       *          more mobs will be blocked by it.
       */
      float m_height;
  };

  using WallShPtr = std::shared_ptr<Wall>;
}

# include "Wall.hxx"

#endif    /* SPAWNER_HH */
