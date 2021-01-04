#ifndef    MOB_FACTORY_HH
# define   MOB_FACTORY_HH

# include "Mob.hh"

namespace tdef {
  namespace mobs {

    /**
     * @brief - Generates the properties for a mob given its
     *          type.
     * @param type - the type of the mob for which the props
     *               should be generated.
     * @param p - the position of the mob to generate.
     * @param level - the level for which the props should be
     *                generated.
     * @return - the generated props.
     */
    Mob::MProps
    generateProps(const Type& type,
                  const utils::Point2f& p,
                  int level = 0);

  }
}

#endif    /* MOB_FACTORY_HH */
