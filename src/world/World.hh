#ifndef    WORLD_HH
# define   WORLD_HH

# include <vector>
# include <memory>
# include <fstream>
# include <core_utils/CoreObject.hh>
# include <core_utils/Signal.hh>
# include <core_utils/RNG.hh>
# include <maths_utils/Point2.hh>
# include "Mob.hh"
# include "Tower.hh"
# include "Projectile.hh"
# include "Block.hh"
# include "Locator.hh"

namespace tdef {

  namespace world {

    /**
     * @brief - Convenience enumeration defining the state for
     *          a cell. It represents the main type of elements
     *          handled by the world.
     */
    enum class State {
      Empty,
      Colony,
      Deposit,
      Worker,
      Warrior,
      Count
    };

    /**
     * @brief - Convenience enumeration defining the difficulty
     *          of a world: this has an impact on the number of
     *          spawners and walls that get generated.
     */
    enum class Difficulty {
      Easy,
      Normal,
      Hard
    };

  }

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new world with the specified seed.
       * @param seed - the seed to use for this world.
       */
      World(int seed);

      /**
       * @brief - Desctruction of the object.
       */
      ~World();

      /**
       * @brief - Used to return a locator object to allow easy
       *          access to elements of this world.
       * @return - a locator allowing to filter and efficiently
       *           access objects of this world.
       */
      LocatorShPtr
      locator() const noexcept;

      /**
       * @brief - Used to move one step ahead in time in this
       *          world, given that `tDelta` represents the
       *          duration of the last frame in seconds. The
       *          method is also given the controls as input
       *          so that it can be made available to entities.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      step(float tDelta);

      /**
       * @brief - Used to indicate that the world should be
       *          paused. Time based entities and actions
       *          should take actions to correctly resume at
       *          a later time.
       */
      void
      pause();

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       */
      void
      resume();

      /**
       * @brief - Used to perform the registration of this
       *          block assuming it is valid. No checks are
       *          performed t overify that the block will
       *          be valid.
       * @param block - the block to spawn.
       */
      void
      spawn(BlockShPtr block);

      /**
       * @brief - Used to force the update of elements to
       *          be deleted and delete them right away.
       */
      void
      forceDelete();

      /**
       * @brief - Used to reset the properties of this world
       *          so that it can be used to start a new game.
       * @param metadataSize - defines the size of the data
       *                       existing at the beginning of
       *                       the file and describing data
       *                       that is not relevant to the
       *                       world's data.
       * @param file - the name of the file from which the
       *               world's data should be loaded. In case
       *               the path is empty a new world will be
       *               generated.
       * @param difficulty - the difficulty of the world to
       *                     generate.
       *                     This value is ignored in case the
       *                     input file is valid, in which case
       *                     the difficulty is just assumed to
       *                     be described by the world's data.
       */
      void
      reset(unsigned metadataSize,
            const std::string& file = std::string(),
            const world::Difficulty& difficulty = world::Difficulty::Normal);

      /**
       * @brief - Used to save the content of the world to
       *          the file provided in input. This can be
       *          particulary useful to restore the game
       *          at a later point in time.
       * @param file - the name of the file to which this
       *               world should be serialized.
       */
      void
      save(const std::string& file) const;

    private:

      /**
       * @brief - Generate the base layout for this world
       *          with the specified difficulty.
       * @param difficulty - the difficulty of the world
       *                     to generate.
       */
      void
      generate(const world::Difficulty& difficulty);

      /**
       * @brief - Called after a world has been generated,
       *          either from a file or through automatic
       *          procedures to initialize the needed info
       *          about it.
       *          For now it mainly consists in creating
       *          the locator object.
       */
      void
      initialize();

      /**
       * @brief - Attempt to load a world from the file as
       *          specified in input.
       * @param file - the name of the file from which the
       *               world should be loaded.
       * @param metadataSize - defines the size of the data
       *                       existing at the beginning of
       *                       the file and describing data
       *                       that is not relevant to the
       *                       world's data.
       */
      void
      loadFromFile(const std::string& file,
                   unsigned metadataSize);

      /**
       * @brief - Used to perform an update of all elements
       *          still existing in the world in response to
       *          a world change.
       */
      void
      onWorldUpdate();

    private:

      /**
       * @brief - Convenience define to handle the dimension of a newly
       *          generated world.
       */
      static constexpr int sk_worldSize = 30;

      /**
       * @brief - The variation allowed when choosing the position of
       *          the spawner. Defined as a percentage of the world
       *          size.
       */
      static constexpr float sk_spawnerRingWidth = 0.05f;

      /**
       * @brief - The random number engine for this world: allows to
       *          make the simulation deterministic by gathering all
       *          randomness in a single place.
       */
      utils::RNG m_rng;

      /**
       * @brief - The list of blocks for this world.
       */
      std::vector<BlockShPtr> m_blocks;

      /**
       * @brief - The list of mobs available in this world.
       */
      std::vector<MobShPtr> m_mobs;

      /**
       * @brief - The list of projectiles waiting to do damage.
       */
      std::vector<ProjectileShPtr> m_projectiles;

      /**
       * @brief - Defines whether this world is paused (i.e.
       *          internal attributes of the mobs/blocks/etc
       *          have already been updated to reflect the
       *          pause status) or not. This allows to react
       *          to consecutive pause requests and prevent
       *          weird behaviors to occur.
       */
      bool m_paused;

      /**
       * @brief - The locator to use to organize objects and
       *          mobs based on their position.
       */
      LocatorShPtr m_loc;

    public:

      /**
       * @brief - Signal emitted whenever gold is earned during
       *          a simulation step for the world. The parameter
       *          corresponds to the amount of gold earned.
       */
      utils::Signal<float> onGoldEarned;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

# include "World.hxx"

#endif    /* WORLD_HH */
