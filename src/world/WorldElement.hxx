#ifndef    WORLD_ELEMENT_HXX
# define   WORLD_ELEMENT_HXX

# include "WorldElement.hh"

namespace tdef {

  inline
  WorldElement::Props::~Props() {}

  inline
  const utils::Point2f&
  WorldElement::getPos() const noexcept {
    return m_pos;
  }

  inline
  bool
  WorldElement::isOwned() const noexcept {
    return m_owner.valid();
  }

  inline
  const utils::Uuid&
  WorldElement::getOwner() const noexcept {
    return m_owner;
  }

  inline
  float
  WorldElement::getRadius() const noexcept {
    return m_radius;
  }

  inline
  float
  WorldElement::getHealthRatio() const noexcept {
    if (m_totalHealth <= 0.0f) {
      return 1.0f;
    }

    return m_health / m_totalHealth;
  }

  inline
  bool
  WorldElement::isDead() const noexcept {
    return getHealthRatio() <= 0.0f;
  }

  inline
  void
  WorldElement::assignProps(Props& pp,
                            const utils::Point2f& p,
                            const utils::Uuid& owner) noexcept
  {
    pp.pos = p;
    pp.radius = 1.0f;

    pp.health = 1.0f;

    pp.owner = owner;
  }

  inline
  WorldElement::WorldElement(const Props& props,
                             const std::string& name):
    utils::CoreObject(name),

    m_owner(props.owner),
    m_pos(props.pos),

    m_radius(props.radius <= 0.0f ? 1.0f : props.radius),

    m_totalHealth(std::max(props.health, 0.0f)),
    m_health(m_totalHealth)
  {
    setService("element");
  }

  inline
  void
  WorldElement::setOwner(const utils::Uuid& uuid) {
    m_owner = uuid;
  }

}

#endif    /* WORLD_ELEMENT_HXX */
