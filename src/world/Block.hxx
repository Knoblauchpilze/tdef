#ifndef    BLOCK_HXX
# define   BLOCK_HXX

# include "Block.hh"

namespace tdef {

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

}

#endif    /* BLOCK_HXX */
