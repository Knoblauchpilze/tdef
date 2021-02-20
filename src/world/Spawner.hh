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

    /**
     * @brief - Defines the possible types for a spawner. Each
     *          type has its own distribution of mob and some
     *          specific formula to make the health of the mobs
     *          spawned increase.
     */
    enum class Level {
      Easy,
      Normal,
      Hard
    };

    /**
     * @brief - Converts the input type value to a string
     *          and to an "Unknown" value if the enum can
     *          not be interpreted.
     * @param l - the type to convert.
     * @return - the string representation of the enum.
     */
    std::string
    toString(const Level& l) noexcept;

    /**
     * @brief - Convenience define for a function allowing to
     *          compute the health of a mob based on some level.
     */
    using WaveFunc = std::function<int(StepInfo&, int)>;

    /**
     * @brief - Convenience define for a function allowing to
     *          compute the health pool of a mob based on some
     *          level.
     */
    using HealthFunc = std::function<float(StepInfo&, int)>;

    /**
     * @brief - Convnience define for a function allowing to
     *          compute the bounty of a mob based on the health
     *          of the mob.
     */
    using BountyFunc = std::function<float(StepInfo&, float)>;

    /**
     * @brief - Convenience structure defining the needed
     *          information to represent the custom info
     *          for a spawner.
     *          This allows to perform customization of
     *          the behavior of a spawner without needing
     *          to instantiate a new class.
     */
    struct Processes {
      WaveFunc wave;
      HealthFunc health;
      BountyFunc bounty;
    };

  }

  class Spawner: public Block {
    public:

      /**
       * @brief - Definition of new props to define a spawner.
       */
      struct SProps: public Block::BProps {
        // The radius within which mobs can be spawned around
        // this object.
        float spawnRadius;

        // The cost to spawn a new wave.
        float threshold;

        // The initial reserve available for the spawner: if
        // it is less than `threshold` then the spawning will
        // be delayed.
        float reserve;

        // The amount of stock refilled each second. Defines
        // how fast a spawner can create new mob waves.
        float refill;

        // The distribution of mobs for this spawner.
        spawners::Distribution mobs;

        // The difficulty of the spawner. Allows to generate
        // the correct processes from this difficulty.
        spawners::Level difficulty;
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

      std::ostream&
      operator<<(std::ostream& out) const override;

      std::istream&
      operator>>(std::istream& in) override;

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
        * @brief - Convenience method allowing to handle the ops
        *          needed to generate a new wave for this spawner.
        * @param info - info about the current step of the world.
        */
      void
      generateWave(StepInfo& info);

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

      /**
       * @brief - Defines an experience level for the spawner.
       *          This is a measure of the number of waves that
       *          have already been generated for the spawner.
       */
      int m_exp;

      /**
       * @brief - The difficulty of this spawner.
       */
      spawners::Level m_difficulty;

      /**
       * @brief - Defines the custom processes attached to
       *          this spawner.
       */
      spawners::Processes m_processes;
  };

  using SpawnerShPtr = std::shared_ptr<Spawner>;
}

std::ostream&
operator<<(std::ostream& out, const tdef::Spawner& s) noexcept;

std::istream&
operator>>(std::istream& in, tdef::Spawner& s) noexcept;

# include "Spawner.hxx"

#endif    /* SPAWNER_HH */
