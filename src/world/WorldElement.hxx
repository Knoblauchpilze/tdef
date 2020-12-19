#ifndef    WORLD_ELEMENT_HXX
# define   WORLD_ELEMENT_HXX

# include "WorldElement.hh"

namespace tdef {

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
  WorldElement::WorldElement(const utils::Point2f& pos,
                             float radius,
                             float health,
                             const std::string& name,
                             const utils::Uuid& owner):
    utils::CoreObject(name),

    m_owner(owner),
    m_pos(pos),

    m_radius(radius <= 0.0f ? 1.0f : radius),

    m_totalHealth(std::max(health, 0.0f)),
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
