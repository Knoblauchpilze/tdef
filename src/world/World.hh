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

  }

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new world with the specified seed.
       * @param seed - the seed to use for this world.
       */
      World(int seed);

      /**
       * @brief - Build a new world from the file specified in input.
       *          The file should describe a valid level.
       * @param seed - the seed to use for this world.
       * @param file - the name of the file from which the world's
       *               data should be loaded.
       */
      World(int seed,
            const std::string& file);

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
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      pause(float tDelta);

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      resume(float tDelta);

      /**
       * @brief - Used to perform the registration of this
       *          block assuming it is valid. No checks are
       *          performed t overify that the block will
       *          be valid.
       * @param block - the block to spawn.
       */
      void
      spawn(BlockShPtr block);

    private:

      /**
       * @brief - Generate the base layout for this world.
       */
      void
      generate();

      /**
       * @brief - Attempt to load a world from the file as
       *          specified in input.
       * @param file - the name of the file from which the
       *               world should be loaded.
       */
      void
      loadFromFile(const std::string& file);

    private:

      /**
       * @brief - The random number engine for this world: allows to
       *          make tthe simulation deterministic by gathering all
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
