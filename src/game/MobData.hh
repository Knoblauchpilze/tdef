#ifndef    MOB_DATA_HH
# define   MOB_DATA_HH

# include "Mob.hh"

namespace tdef {
  namespace mobs {

    namespace regular {

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

    namespace fast {

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

    namespace heli {

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

    namespace jet {

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

    namespace armored {

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

    namespace healer {

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

    namespace toxic {

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

    namespace icy {

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

    namespace fighter {

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

    namespace light {

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

#endif    /* MOB_DATA_HH */
