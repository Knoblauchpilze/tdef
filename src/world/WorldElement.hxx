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
  WorldElement::getHealth() const noexcept {
    return m_health;
  }

  inline
  float
  WorldElement::getTotalHealth() const noexcept {
    return m_totalHealth;
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
  bool
  WorldElement::isDeleted() const noexcept {
    return m_deleted;
  }

  inline
  bool
  WorldElement::damage(StepInfo& /*info*/,
                       float hit)
  {
    // In case the `hit` is in fact a healing
    // we can do that in the limit of the health
    // pool for this element.
    if (hit < 0.0f) {
      m_health = std::min(m_health - hit, m_totalHealth);

      return true;
    }

    // Otherwise, clamp to a null health.
    m_health = std::max(m_health - hit, 0.0f);

    return (m_health > 0.0f);
  }

  inline
  void
  WorldElement::markForDeletion(bool toDelete) {
    m_deleted = toDelete;
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
    m_health(m_totalHealth),

    m_deleted(false)
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
