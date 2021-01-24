#ifndef    ANTIAIR_HH
# define   ANTIAIR_HH

# include "Tower.hh"

namespace tdef {
  namespace towers {

    namespace antiair {

      /**
       * @brief - Generate the tower's data for the specified
       *          level.
       * @param level - the level for which the data should be
       *                generated.
       * @return - the generated data.
       */
      Processes
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

      /**
       * @brief - Returns the cost to upgrade the property of
       *          the tower to the specified level.
       * @param type - the type of upgrade.
       * @param level - the level to upgrade.
       * @return - the gold cost for this upgrade.
       */
      float
      getUpgradeCost(const Upgrade& upgrade,
                     int level = 0) noexcept;

    }

  }
}

#endif    /* ANTIAIR_HH */
