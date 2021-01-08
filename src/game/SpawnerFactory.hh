#ifndef    SPAWNER_FACTORY_HH
# define   SPAWNER_FACTORY_HH

# include "Spawner.hh"

namespace tdef {
  namespace spawners {

    /**
     * @brief - Generates default properties for a spawner.
     * @param p - the position of the spawner to generate.
     * @return - the generated props.
     */
    Spawner::SProps
    generateProps(const utils::Point2f& p);

  }
}

#endif    /* SPAWNER_FACTORY_HH */
