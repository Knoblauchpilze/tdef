#ifndef    SPAWNER_FACTORY_HH
# define   SPAWNER_FACTORY_HH

# include "Spawner.hh"

namespace tdef {
  namespace spawners {

    /**
     * @brief - Generates the data for a spawner given its
                level.
     * @param lvl - the level for which the data should be
     *              generated.
     * @return - the generated data.
     */
    Processes
    generateData(const Level& lvl);

    /**
     * @brief - Generates default properties for a spawner.
     * @param p - the position of the spawner to generate.
     * @param lvl - the level of the spawner.
     * @return - the generated props.
     */
    Spawner::SProps
    generateProps(const utils::Point2f& p,
                  const Level& lvl = Level::Normal);

  }
}

#endif    /* SPAWNER_FACTORY_HH */
