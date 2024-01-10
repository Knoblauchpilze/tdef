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
  }

  inline
  float
  Block::getOrientation() const noexcept {
    return m_orientation;
  }

  inline
  std::ostream&
  Block::operator<<(std::ostream& out) const {
    WorldElement::operator<<(out);
    out << m_orientation << " ";

    verbose("Saved block at " + m_pos.toString());

    return out;
  }

  inline
  std::istream&
  Block::operator>>(std::istream& in) {
    WorldElement::operator>>(in);
    in >> m_orientation;

    verbose("Restored block at " + m_pos.toString());

    return in;
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

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Block& b) noexcept {
  b << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Block& b) noexcept {
  b >> in;
  return in;
}

#endif    /* BLOCK_HXX */
