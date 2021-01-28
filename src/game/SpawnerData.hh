#ifndef    SPAWNER_DATA_HH
# define   SPAWNER_DATA_HH

# include "Spawner.hh"

namespace tdef {
  namespace spawners {

    namespace easy {

      /**
       * @brief - Generate the spawner's data for the specified
       *          level.
       * @return - the generated data.
       */
      Processes
      generateData() noexcept;

      /**
       * @brief - Generate the spawner's properties for position provided
       *          in input.
       * @param p - the position of the spawner.
       * @return - the generated properties.
       */
      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept;

    }

    namespace normal {

      /**
       * @brief - Generate the spawner's data for the specified
       *          level.
       * @return - the generated data.
       */
      Processes
      generateData() noexcept;

      /**
       * @brief - Generate the spawner's properties for position provided
       *          in input.
       * @param p - the position of the spawner.
       * @return - the generated properties.
       */
      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept;

    }

    namespace hard {

      /**
       * @brief - Generate the spawner's data for the specified
       *          level.
       * @return - the generated data.
       */
      Processes
      generateData() noexcept;

      /**
       * @brief - Generate the spawner's properties for position provided
       *          in input.
       * @param p - the position of the spawner.
       * @return - the generated properties.
       */
      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept;

    }

  }
}

#endif    /* SPAWNER_DATA_HH */
