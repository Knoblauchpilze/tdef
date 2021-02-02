#ifndef    STRONG_HH
# define   STRONG_HH

# include "Mob.hh"

namespace tdef {
  namespace mobs {

    namespace strong {

      /**
       * @brief - Generate the mob's properties for the level
       *          and position provided in input.
       * @param p - the position of the mob.
       * @param level - the level of the mob.
       * @return - the generated properties.
       */
      Mob::MProps
      generateProps(const utils::Point2f& p,
                    int level = 0) noexcept;

    }

  }
}

#endif    /* STRONG_HH */
