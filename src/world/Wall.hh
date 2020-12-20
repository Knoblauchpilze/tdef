#ifndef    WALL_HH
# define   WALL_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"

namespace tdef {

  class Wall: public Block {
    public:

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param pos - the position of this spawner.
       * @param height - the height of the wall. Used to prevent
       *                 some entities from moving through it.
       * @param owner - the owner of the mob.
       */
      Wall(const utils::Point2f& pos,
           float height = 1.0f,
           const utils::Uuid& owner = utils::Uuid());

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
