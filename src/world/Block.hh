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

    protected:

      /**
       * @brief - Definition of new props to define a block.
       */
      struct BProps: public Block::Props {
      };

      static
      void
      assignProps(BProps& pp,
                  const utils::Point2f& p,
                  const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Create a new solid element with the tile
       *          and name. Only used to forward the args
       *          to the base class.
       * @param props - the properties defining this block.
       * @param name- the name of the block.
       */
      Block(const BProps& props,
            const std::string& name);
  };

  using BlockShPtr = std::shared_ptr<Block>;
}

# include "Block.hxx"

#endif    /* BLOCK_HH */
