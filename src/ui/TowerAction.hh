#ifndef    TOWER_ACTION_HH
# define   TOWER_ACTION_HH

# include <memory>
# include "Action.hh"
# include "Tower.hh"

namespace tdef {

  class TowerAction: public Action {
    public:

      /**
       * @brief - Create a new action allowing to perform some
       *          modification of a block. Typical use cases
       *          include spawning or removing a block from the
       *          world.
       * @param type - the type of tower to create with this
       *               action.
       */
      TowerAction(const towers::Type& type);

      ~TowerAction() = default;

      void
      apply(World& w) const override;

    private:

      /**
       * @brief - The description of the type of tower to create
       *          from this action.
       */
      towers::Type m_type;
  };

  using TowerActionShPtr = std::shared_ptr<TowerAction>;
}

#endif    /* TOWER_ACTION_HH */
