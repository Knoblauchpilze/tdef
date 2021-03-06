#ifndef    ACTION_HH
# define   ACTION_HH

# include <memory>
# include <core_utils/CoreObject.hh>

namespace tdef {

  // Forward declare the game class onto which the
  // actions will be applied.
  class Game;

  class Action: public utils::CoreObject {
    public:

      /**
       * @brief - Desctruction of the object.
       */
      virtual ~Action();

      /**
       * @brief - Interface method allowing to perform an
       *          action. Nothing is known of the actual
       *          process performed by the action and we
       *          don't need to.
       *          The goal is to provide a base interface
       *          allowing menus to trigger some changes
       *          in the game.
       *          Inheriting classes should redefine this
       *          method based on their effect.
       * @param g - the game onto which the action should
       *            be applied.
       */
      virtual void
      apply(Game& g) const = 0;

    protected:

      /**
       * @brief - Create a new empty action.
       * @param name - the name of the action (for logging purposes).
       */
      Action(const std::string& name);
  };

  using ActionShPtr = std::shared_ptr<Action>;
}

# include "Action.hxx"

#endif    /* ACTION_HH */
