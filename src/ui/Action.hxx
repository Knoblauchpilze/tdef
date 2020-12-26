#ifndef    ACTION_HXX
# define   ACTION_HXX

# include "Action.hh"

namespace tdef {

  inline
  Action::Action(const action::Type& type,
                 const std::string& name):
    utils::CoreObject(name),

    m_type(type)
  {
    setService("action");
  }

  inline
  Action::~Action() {}

  inline
  const action::Type&
  Action::getType() const noexcept {
    return m_type;
  }

}

#endif    /* ACTION_HXX */
