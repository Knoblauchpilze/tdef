#ifndef    ACTION_HH
# define   ACTION_HH

# include <memory>
# include <core_utils/CoreObject.hh>
# include "World.hh"

namespace tdef {

  namespace action {

    /**
     * @brief - Describe general type of actions that can be
     *          used in various context.
     */
    enum class Type {
      Creation,
      Deletion
    };

  }

  class Action: public utils::CoreObject {
    public:

      /**
       * @brief - Desctruction of the object.
       */
      virtual ~Action();

      /**
       * @brief - Interface method allowing to perform an
       *          action on the input world. It aims at
       *          providing a common interface allowing
       *          menus to trigger some real changes in the
       *          world.
       *          Inheriting classes should redefine this
       *          method based on their effect on the world.
       * @param w - the world onto which the action should
       *            be applied.
       */
      virtual void
      apply(World& w) const = 0;

    protected:

      /**
       * @brief - Create a new empty action with the specified
       *          type.
       * @param type - the general type of this action.
       * @param name - the name of the action (for logging purposes).
       */
      Action(const action::Type& type,
             const std::string& name);

      const action::Type&
      getType() const noexcept;

    private:

      /**
       * @brief - The general type of this action.
       */
      action::Type m_type;
  };

  using ActionShPtr = std::shared_ptr<Action>;
}

# include "Action.hxx"

#endif    /* ACTION_HH */
