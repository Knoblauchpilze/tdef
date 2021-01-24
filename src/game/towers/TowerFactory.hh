#ifndef    TOWER_FACTORY_HH
# define   TOWER_FACTORY_HH

# include "Tower.hh"

namespace tdef {
  namespace towers {

    /**
     * @brief - Generates the data for a tower given its type.
     * @param type - the type of the tower for which the data
     *               should be generated.
     * @param level - the level for which the data should be
     *                generated.
     * @return - the generated data.
     */
    Processes
    generateData(const Type& type,
                 int level = 0);

    /**
     * @brief - Generates the properties for a tower given its
     *          type.
     * @param type - the type of the tower for which the props
     *               should be generated.
     * @param p - the position of the tower to generate.
     * @param level - the level for which the props should be
     *                generated.
     * @return - the generated props.
     */
    Tower::TProps
    generateProps(const Type& type,
                  const utils::Point2f& p,
                  int level = 0);

    /**
     * @brief - Returns the gold cost of a specific tower with
     *          the specified level.
     * @param type - the type of the tower.
     * @param level - the level for which the cost should be
     *                returned.
     * @return - the gold cost for the level of the tower.
     */
    float
    getCost(const Type& type,
            int level = 0);

    /**
     * @brief - Returns the gold cost of a specific tower with
     *          the specified level and upgrade type.
     * @param type - the type of the tower.
     * @param upgrade - the type of upgrade to perform.
     * @param level - the level for which the cost should be
     *                returned.
     * @return - the gold cost for the level upgrade.
     */
    float
    getUpgradeCost(const Type& type,
                   const Upgrade& upgrade,
                   int level = 0);

  }
}

#endif    /* TOWER_FACTORY_HH */
