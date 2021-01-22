#ifndef    TOWER_DATA_HH
# define   TOWER_DATA_HH

# include "Tower.hh"
# include "Mob.hh"

namespace tdef {
  namespace towers {

    /**
     * @brief - Defines a projectile with infinite speed.
     */
    constexpr float infinite_projectile_speed = -1.0f;

    /**
     * @brief - Defines a tower with infinite aim speed.
     */
    constexpr float infinite_aim_speed = 0.0f;

    /**
     * @brief - Defines a tower with infinite attack speed.
     */
    constexpr float infinite_attack_speed = 0.0f;

    /**
     * @brief - Defines a tower with no speed decrease on
     *          mobs.
     */
    constexpr float no_speed_decrease = 0.0f;

    /**
     * @brief - Basic target picking method which just pick
     *          the closest mob from a position.
     * @param info - the data to use to pick a target.
     * @param data - the data to use to perform picking.
     * @return - the picked mob.
     */
    MobShPtr
    basicTargetPicking(StepInfo& info, PickData& data);

    /**
     * @brief - Basic damaging function which just applies
     *          the damaga data to the mob.
     * @param info - information to pick random numbers for
     *               damage application.
     * @param mob - the mob to which damage should be applied.
     * @param data - the damage data to apply.
     * @return - `true` if the mob is still alive.
     */
    bool
    basicDamaging(StepInfo& info, MobShPtr mob, DamageData& data);

  }
}

#endif    /* TOWER_DATA_HH */
