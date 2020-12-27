#ifndef    BLOCK_HH
# define   BLOCK_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"
# include "StepInfo.hh"

namespace tdef {

  class Block: public WorldElement {
    public:

      void
      init(StepInfo& info) override;

      void
      step(StepInfo& info) override;

      void
      pause(const utils::TimeStamp& t) override;

      void
      resume(const utils::TimeStamp& t) override;

      void
      destroy(StepInfo& info) override;

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
