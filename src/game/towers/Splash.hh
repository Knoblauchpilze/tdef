#ifndef    SPLASH_HH
# define   SPLASH_HH

# include "Tower.hh"

namespace tdef {
  namespace towers {

    namespace splash {

      /**
       * @brief - Generate the tower's data for the specified
       *          level.
       * @param level - the level for which the data should be
       *                generated.
       * @return - the generated data.
       */
      Data
      generateData(int level = 0) noexcept;

      /**
       * @brief - Generate the tower's properties for the level
       *          and position provided in input.
       * @param p - the position of the tower.
       * @param level - the level of the tower.
       * @return - the generated properties.
       */
      Tower::TProps
      generateProps(const utils::Point2f& p,
                    int level = 0) noexcept;

      /**
       * @brief - Returns the cost to build the tower for the
       *          specified level.
       * @param level - the level of the tower.
       * @return - the gold cost for this level.
       */
      float
      getCost(int level = 0) noexcept;

    }

  }
}

#endif    /* SPLASH_HH */
