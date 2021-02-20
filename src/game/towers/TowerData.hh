#ifndef    TOWER_DATA_HH
# define   TOWER_DATA_HH

# include <vector>
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
     * @brief - Used to convert a freeze percentage to the
     *          corresponding speed ratio which describes
     *          the freezing as a percentage of the base
     *          speed.
     *          This is used to convert between a human
     *          readable value and the internal engine
     *          syntax.
     *          The percentage is assumed to be in the range
     *          `[0; 100]`.
     * @param freezePercentage - the percentage to convert.
     * @return - the corresponding speed ratio.
     */
    constexpr float
    freezePercentageToSpeedRatio(float freezePercentage) noexcept;

    /**
     * @brief - Basic target picking method which just pick
     *          the closest mob from a position.
     * @param info - the data to use to pick a target.
     * @param data - the data to use to perform picking.
     * @return - the picked mobs.
     */
    std::vector<MobShPtr>
    basicTargetPicking(StepInfo& info, PickData& data);

    /**
     * @brief - Target picking method which picks all the
     *          mobs visible in a particular radius.
     * @param info - the data to use to pick a target.
     * @param data - the data to use to perform picking.
     * @return - the picked mobs.
     */
    std::vector<MobShPtr>
    multipleTargetPicking(StepInfo& info, PickData& data);

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
    basicDamaging(StepInfo& info, MobShPtr mob, Damage& data);

    /**
     * @brief - Generate an upgradable which has a linear relation
     *          between the level and the output value. The minimum
     *          value is clamped to be at least the provided one.
     * @param a - the linear coefficient of the level.
     * @param b - origin intercept of the linear function.
     * @param min - an additional parameter allowing to clamp the
     *              returned value to be at excatly this one if the
     *              level is `0`.
     * @return - the created upgradable.
     */
    towers::Upgradable
    buildLinearUpgradable(float a,
                          float b,
                          float min) noexcept;

    /**
     * @brief - Generate an upgradable which has a quadratic relation
     *          between the level and the output value. The minimum
     *          value is clamped to be at least the provided one.
     * @param a - the quadratic coefficient of the level.
     * @param b - the linear coefficient of the level.
     * @param c - origin intercept of the quadratic function.
     * @param min - an additional parameter allowing to clamp the
     *              returned value to be at excatly this one if the
     *              level is `0`.
     * @return - the created upgradable.
     */
    towers::Upgradable
    buildQuadraticUpgradable(float a,
                             float b,
                             float c,
                             float min) noexcept;

    /**
     * @brief - Generate an upgradable which has a quadratic relation
     *          between the level and the output value. The minimum
     *          value is clamped to be at least the provided one.
     * @param a - the cubic coefficient of the level.
     * @param b - the quadratic coefficient of the level.
     * @param c - the linear coefficient of the level.
     * @param d - origin intercept of the quadratic function.
     * @param min - an additional parameter allowing to clamp the
     *              returned value to be at excatly this one if the
     *              level is `0`.
     * @return - the created upgradable.
     */
    towers::Upgradable
    buildCubicUpgradable(float a,
                         float b,
                         float c,
                         float d,
                         float min) noexcept;

  }
}

# include "TowerData.hxx"

#endif    /* TOWER_DATA_HH */
