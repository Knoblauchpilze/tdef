#ifndef    WORLD_HH
# define   WORLD_HH

# include <memory>
# include <fstream>
# include <core_utils/CoreObject.hh>
# include <core_utils/RNG.hh>

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
       * @brief - Create a new world with the specified dimensions.
       * @param seed - the seed to use for this world.
       * @param width - the width of the world in cells.
       * @param height - the height of the window in cells.
       */
      World(int seed, int width = 8, int height = 6);

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

      unsigned
      w() const noexcept;

      unsigned
      h() const noexcept;

      /**
       * @brief- Fetch the state of a cell assuming it exists.
       *         In case the coordinates are outside of the
       *         world the `Empty` value is returned.
       * @param x - the abscissa of the cell to get.
       * @param t - the ordinate of the cell to get.
       * @param valid- `true` in case the coordinates are valid.
       * @return - the state of the cell or empty if the coords
       *           are invalid.
       */
      world::State
      cell(int x, int y, bool& valid) const noexcept;

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

      /**
       * @brief - Used to parse the dimensions of the world
       *          from the file laoded in the stream.
       *         Throws an error in case an error occurs.
       * @param in - the stream describing the level file.
       */
      void
      loadDimensions(std::ifstream& in);

    private:

      /**
       * @brief - Defines the block size for each individual
       *          regions of the world.
       */
      static constexpr int sk_regionSize = 100;

      /**
       * @brief - A convenience structur representing a block
       *          (or a region) containing some information
       *          about the world. Allows to speed up empty
       *          regions by simulating them quickly.
       */
      struct Region {
        int x;
        int y;

        world::State cells[sk_regionSize * sk_regionSize];
      };

      /**
       * @brief - Width of the world in cells.
       */
      int m_w;

      /**
       * @brief - Height of the world in cells.
       */
      int m_h;

      /**
       * @brief - The random number engine for this world: allows to
       *          make tthe simulation deterministic by gathering all
       *          randomness in a single place.
       */
      utils::RNG m_rng;

      /**
       * @brief - Defines the list of regions created for this world.
       */
      std::vector<Region> m_regions;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

# include "World.hxx"

#endif    /* WORLD_HH */
