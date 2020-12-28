#ifndef    ACTION_HXX
# define   ACTION_HXX

# include "Action.hh"

namespace tdef {

  inline
  Action::Action(const std::string& name):
    utils::CoreObject(name)
  {
    setService("action");
  }

  inline
  Action::~Action() {}

}

#endif    /* ACTION_HXX */
