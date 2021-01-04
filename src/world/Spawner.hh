#ifndef    SPAWNER_HH
# define   SPAWNER_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"
# include "Mob.hh"

namespace tdef {
  namespace spawners {

    /**
     * @brief - Convenience structure allowing to register a
     *          single item of a mob distribution.
     */
    struct DistItem {
      // `prob` defines the probability for the item.
      float prob;

      // `mob` defines the type of mob for this item.
      mobs::Type mob;
    };

    /**
     * @brief - Defines a distribution of mobs which allows
     *          to spawn various type of mobs with different
     *          probabilities for a spawner.
     */
    using Distribution = std::vector<DistItem>;

  }

  class Spawner: public Block {
    public:

      /**
       * @brief - Definition of new props to define a spawner.
       */
      struct SProps: public Block::BProps {
        float spawnRadius;

        float threshold;
        float reserve;
        float refill;

        spawners::Distribution mobs;
      };

      static
      SProps
      newProps(const utils::Point2f& p,
               const spawners::Distribution& dist = spawners::Distribution{{1.0f, mobs::Type::Regular}},
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param props - the properties to define this spawner.
       */
      Spawner(const SProps& props);

      void
      step(StepInfo& info) override;

      /**
       * @brief - Used to change the amount of the resource that
       *          is managed by this spawner by the specified
       *          value. It can be either used to refill this
       *          portal or to deplete even more the stock.
       *          In case the amount is too large to be removed
       *          from the internal stock, the operation is not
       *          performed unless the `force` boolean says so.
       *          We just guarantee that the stock does not go
       *          below `0`.
       * @param delta - the amount of resource to add or deplete.
       * @param force - `true` if the refill operation should
       *                always be successful.
       * @return - the value that was actually added/removed in
       *           the deposit.
       */
      float
      refill(float delta, bool force = false);

    protected:

      /**
       * @brief - Create an entity conform to the specifications
       *          of this spawner and return it as a pointer.
       * @param info - information about the step.
       * @return - a pointer to the created entity.
       */
      MobShPtr
      spawn(StepInfo& info) noexcept;

    private:

      /**
       * @brief - The distribution of mobs attached to this
       *          spawner. Will be polled when a new mob is
       *          to be spawned so that we can randomize
       *          each wave.
       */
      spawners::Distribution m_distribution;

      /**
       * @brief - The radius around this spawner where a mob
       *          can be spawned. A value of `0` indicates
       *          that mobs should be spawn inside the portal.
       *          This value is clamped to at least `0`.
       */
      float m_spawnRadius;

      /**
       * @brief - The current stock of resource available in the
       *          spawner. Based on the cost of a single entity
       *          it can be enough to spawn a new one.
       */
      float m_stock;

      /**
       * @brief - The cost of a single entity. This amount is
       *          removed from the `m_stock` each time a new
       *          entity is spawned.
       */
      float m_threshold;

      /**
       * @brief - Base refill rate for this spawner. This value
       *          is added to the `m_stock` assuming it is the
       *          amount of stock that is refilled every second.
       */
      float m_refill;
  };

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */
