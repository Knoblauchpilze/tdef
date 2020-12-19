#ifndef    BLOCK_HH
# define   BLOCK_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"
# include "StepInfo.hh"

namespace tdef {

  class Block: public WorldElement {
    public:

      /**
       * @brief - Create a new solid element with the tile
       *          and name. Only used to forward the args
       *          to the base class.
       * @param pos - the position of this world element at the
       *              moment of its creation.
       * @param name - the name of the object (mainly used for
       *               logging purposes).
       * @param owner - the identifier of the owner of this
       *                element. It is assigned to a null value
       *                by default, meaning that the element is
       *                not owned.
       */
      Block(const utils::Point2f& pos,
            const std::string& name,
            const utils::Uuid& owner = utils::Uuid());

      /**
       * @brief - Base implementation for an element which
       *          does nothing.
       * @param info - information about the world.
       */
      void
      step(StepInfo& info) override;

      /**
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to pause.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const utils::TimeStamp& t) override;

      /**
       * @brief - Base implementation for an element which
       *          does nothing: there are no internal ops
       *          to resume.
       * @param t - the timestamp at which the resume occur.
       */
      void
      resume(const utils::TimeStamp& t) override;
  };

  using BlockShPtr = std::shared_ptr<Block>;
}

# include "Block.hxx"

#endif    /* BLOCK_HH */
