#ifndef    SIMPLE_ACTION_HH
# define   SIMPLE_ACTION_HH

# include <memory>
# include "Action.hh"

namespace tdef {
  namespace action {

    /**
     * @brief - Defines a common function called whenever
     *          the action is triggered. This allows to
     *          specialize the menu class easily when not
     *          much is needed.
     */
    using Process = std::function<void(Game& g)>;

  }

  class SimpleAction: public Action {
    public:

      /**
       * @brief - Create a new action with the specified
       *          process to trigger when the action is
       *          being processed..
       * @param cb - the callback to trigger when this
       *             action is executed.
       */
      SimpleAction(const action::Process& cb);

      ~SimpleAction() = default;

      void
      apply(Game& g) const override;

      /**
       * @brief - Assign a new process to be triggered in
       *          case this action is applied.
       * @param cb - the new process to trigger when this
       *             action is applied.
       */
      void
      setProcess(const action::Process& cb);

    private:

      /**
       * @brief - The callback to trigger when this action
       *          is triggered.
       */
      action::Process m_callback;
  };

}

#endif    /* SIMPLE_ACTION_HH */
