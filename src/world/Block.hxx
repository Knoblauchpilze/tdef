#ifndef    BLOCK_HXX
# define   BLOCK_HXX

# include "Block.hh"

namespace tdef {

  inline
  void
  Block::assignProps(BProps& pp,
                     const utils::Point2f& p,
                     const utils::Uuid& owner) noexcept
  {
    WorldElement::assignProps(pp, p, owner);

    pp.orientation = 0.0f;
    pp.rotation = 3.1415926535f;
  }

  inline
  float
  Block::getOrientation() const noexcept {
    return m_orientation;
  }

  inline
  float
  Block::getRotationSpeed() const noexcept {
    return m_rotationSpeed;
  }

  inline
  void
  Block::init(StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  void
  Block::step(StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  void
  Block::pause(const utils::TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Block::resume(const utils::TimeStamp& /*t*/) {
    // Nothing to do.
  }

  inline
  void
  Block::destroy(StepInfo& /*info*/) {
    // Nothing to do.
  }

  inline
  void
  Block::worldUpdate(LocatorShPtr /*loc*/) {
    // Nothing to do.
  }

}

#endif    /* BLOCK_HXX */
