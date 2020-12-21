#ifndef    TOWER_HH
# define   TOWER_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"

namespace tdef {

  class Tower: public Block {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct TProps: public Block::BProps {
      };

      static
      TProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param props - the properties defining this wall.
       */
      Tower(const TProps& props);

    private:
  };

  using TowerShPtr = std::shared_ptr<Tower>;
}

# include "Tower.hxx"

#endif    /* TOWER_HH */
