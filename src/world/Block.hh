#ifndef    BLOCK_HH
# define   BLOCK_HH

# include <memory>
# include <core_utils/Uuid.hh>
# include "WorldElement.hh"
# include "StepInfo.hh"

namespace tdef {

  class Block: public WorldElement {
    public:

      float
      getOrientation() const noexcept;

      float
      getRotationSpeed() const noexcept;

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

      void
      worldUpdate(LocatorShPtr loc) override;

    protected:

      /**
       * @brief - Definition of new props to define a block.
       */
      struct BProps: public Block::Props {
        float orientation;
        float rotation;
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

    protected:

      /**
       * @brief - The current orientation of this block. This
       *          defines the general direction where it can
       *          take an action immediately. A value of `0`
       *          indicates a block pointing along the `x`
       *          axis.
       */
      float m_orientation;

      /**
       * @brief - The rotation speed in radians per second for
       *          this block.
       */
      float m_rotationSpeed;
  };

  using BlockShPtr = std::shared_ptr<Block>;
}

# include "Block.hxx"

#endif    /* BLOCK_HH */
