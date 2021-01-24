#ifndef    TOWER_HH
# define   TOWER_HH

# include <map>
# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"
# include "Mob.hh"

namespace tdef {
  namespace towers {

    /**
     * @brief - Convenience structure defining all props
     *          needed to perform the custom behaviors
     *          that can be assigned to a tower. This is
     *          only relevant for the target picking
     *          custom data.
     */
    struct PickData {
      utils::Point2f pos;
      float minRange;
      float maxRange;
    };

    /**
     * @brief - Defines a generic function signature that
     *          can be used by a tower to pick a new mob
     *          to target.
     */
    using TargetPicker = std::function<MobShPtr(StepInfo&, PickData&)>;

    /**
     * @brief - Convenience structure defining all props
     *          defining the data performed by a tower.
     */
    struct DamageData {
      // The main damage dealt by the tower. This defines
      // the raw data unshielded and can represent some
      // sort of poisoning in case the `pDuration` is not
      // a zero duration.
      float damage;

      // The accuracy of the damage: this value in the range
      // `[0; 1]` defines a chance for the damage to miss the
      // to apply its damage.
      float accuracy;

      // Represents the factor by which the speed of the
      // target of the damage should be multiplied. This
      // value should be in the range `[0; 1]` and is
      // a measure of the final speed of the target.
      float speed;

      // Defines the percentage by which the speed of the
      // target is reduced every second.
      float slowdown;

      // Defines the chances that the target of the damage
      // becomes stunned. This value is in the range `[0; 1]`
      // where `0` means no chance to be stunned and `1` a
      // 100% chance to be stunned.
      float stunProb;

      // Defines the likelihood of a critical hit for the
      // tower. This value is in the range `[0; 1]`.
      float critProb;

      // Defines the multiplier to apply to the damage in
      // case a critical hit is performed.
      float critMultiplier;

      // The duration of the freezing factor applied to the
      // target of the damage.
      utils::Duration fDuration;

      // The duration of the stunning effect applied to the
      // target of the damage.
      utils::Duration sDuration;

      // The duration of the poisoning effect applied
      // to the target.
      utils::Duration pDuration;
    };

    /**
     * @brief - Defines a generic function signature that
     *          can be used by a tower to apply damage to
     *          a mob.
     *          The return value should indicates whether
     *          the mob is still alive or has been damaged
     *          enough that it should be killed.
     */
    using DoDamage = std::function<bool(StepInfo&, MobShPtr, DamageData&)>;

    /**
     * @brief - Convenience structure defining the needed
     *          information to represent the custom info
     *          for a tower.
     *          This allows to define various towers that
     *          can specialize part of their behavior for
     *          example the way a mob is picked when the
     *          conditions are met to pick a new target
     *          or the way a mob is affected by the tower.
     */
    struct Data {
      TargetPicker pickMob;
      DoDamage damage;
    };

    /**
     * @brief - Convenience enumeration defining the types
     *          of towers available in the game.
     */
    enum class Type {
      Basic,
      Sniper,
      Cannon,
      Freezing,
      Venom,
      Splash,
      Blast,
      Multishot,
      Minigun,
      Antiair,
      Tesla,
      Missile,
    };

    /**
     * @brief - Converts the input type value to a string
     *          and to an "Unknown" value if the enum can
     *          not be interpreted.
     * @param t - the type to convert.
     * @return - the string representation of the enum.
     */
    std::string
    toString(const Type& t) noexcept;

    /**
     * @brief - Convenience enumeration defining the types
     *          of upgrades that a tower can define.
     */
    enum class Upgrade {
      Range,
      Damage,
      RotationSpeed,
      AttackSpeed,
      ProjectileSpeed,
      FreezingPower,
      FreezingSpeed,
      FreezingDuration,
      PoisonDuration,
      StunChance,
      StunDuration
    };

    /**
     * @brief - Converts the input upgrade value to a string
     *          and to an "Unknown" value if the enum can
     *          not be interpreted.
     * @param t - the upgrade to convert.
     * @return - the string representation of the enum.
     */
    std::string
    toString(const Upgrade& t) noexcept;

    /**
     * @brief - Convenience structure to refer to a map of
     *          upgrades.
     */
    using Upgrades = std::map<Upgrade, int>;

  }

  class Tower: public Block {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct TProps: public Block::BProps {
        // The initial energy of the tower. This might allow
        // for a hit directly when the tower is built.
        float energy;

        // The total energy available for this tower. This
        // usually is equal to the `attackCost` as a simple
        // way to know how often a tower can fire.
        float maxEnergy;

        // How many energy units are refilled each second.
        // This value usually indicates how fast a tower
        // can fire two consecutive shots.
        float refill;

        // The minimum range below which the tower can't
        // fire on a target. This does not mean that existing
        // projectils can't reach this area.
        float minRange;

        // The maximum range above which a mob can't be
        // targeted by the tower.
        float maxRange;

        // The damage done by the tower.
        float damage;

        // The radius of the aoe for each projectile or hit
        // generated by the tower. The damage made to targets
        // inside the aoe is not configurable and depends on
        // the proximity with the center of the area.
        float aoeRadius;

        // The aiming speed of the tower. The value is not
        // directly expressing a duration: instead it is used
        // to derive a duration through the following formula:
        // `100 / aimSpeed seconds`.
        float aimSpeed;

        // The speed of the projectiles shot by this tower.
        // In case this value is negative we consider that
        // the tower shoots infinitely fast projectiles and
        // we deal damage to the target immediately.
        float projectileSpeed;

        // A value in the range `[0; 1]` defining the chance
        // to miss a hit where `0` means that the tower will
        // always miss and `1` that it will never miss.
        float accuracy;

        // The duration of any effect applied by the tower. It
        // usually refers to either a freezing or a poisoning
        // effect. This duration is expressed in milliseconds.
        float duration;

        // The angle in radians within which the tower is able
        // to shoot on a target. This allows the tower to not
        // be perfectly aligned and still shoot or defines the
        // angle with which multiple projectiles will be fired.
        float shootAngle;

        // The amount of projectiles fired at once by the tower.
        int projectiles;

        float acceleration;

        // Defines the percentage of reduction of the speed
        // of mobs. Value in the range `[0; 1]`.
        float freezePercent;

        // Defines the percentage of speed that is reduced
        // per second when an entity is within the range
        // of this tower. Value in the range `[0; 1]`.
        float freezeSpeed;

        // Defines a value in the range `[0; 1]` indicating
        // how likely it is for the tower to stun an enemy.
        float stunProb;

        // Defines a value in the range `[0; 1]` indicating
        // how likely it is for the tower to land a critical
        // hit. Critical hit do more damage.
        float critProb;

        // Defines a multiplier indicating how much damage
        // is dealt additionally to a regular hit when a
        // critical hit is performed.
        // This value is used to multiply the base damage
        // so a value larger than `1` indicates more damage
        // while a value smaller than `1` indicates less
        // damage.
        float critMultiplier;

        // Defines the amount of energy that each shot fired
        // by the tire cost. If this value is `0` it means
        // that the tower can shoot indefinitely often. In
        // any other case the speed at which shots are fired
        // will depend on the energy refill.
        float attackCost;

        // Defines the cost in resource for a tower. This
        // values scales with the level of the tower and
        // we only represent the initial value here.
        float cost;

        // The `type` defines a representation of the real
        // kind of the tower. If one is using the factory
        // towers with the same type should have similar
        // properties but it's not guaranteed otherwise.
        towers::Type type;

        // Defines the list of upgrades that can be applied
        // to this tower.
        std::vector<towers::Upgrade> upgrades;
      };

      static
      TProps
      newProps(const utils::Point2f& p,
               const towers::Type& type = towers::Type::Basic,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new tower with the specified props.
       * @param props - the properties defining this tower.
       * @param desc - the description of the custom functions to
       *               use during the behavior of this tower.
       */
      Tower(const TProps& props,
            const towers::Data& desc);

      /**
       * @brief - Fetch the type for this tower.
       * @return - the type for this tower.
       */
      const towers::Type&
      getType() const noexcept;

      float
      getRange() const noexcept;

      float
      getAttack() const noexcept;

      /**
       * @brief - Fetch the status of upgrades for this tower.
       * @return - the state of upgrades.
       */
      towers::Upgrades
      getUpgrades() const noexcept;

      /**
       * @brief - Attempts to fetch the level for the provided
       *          upgrade for this tower. In case the tower is
       *          not providing the corresponding upgrade the
       *          returned value will be `0`.
       * @param upgrade - the type of upgrade for which the
       *                  level should be fetched.
       * @return - the level for this upgrade or `0` in case
       *           the upgrade is not available.
       */
      int
      getUpgradeLevel(const towers::Upgrade& upgrade) const noexcept;

      /**
       * @brief - Returns an aggregated number measuring the
       *          attack speed for this tower. The higher the
       *          value the faster the tower is able to attack.
       *          The value is `0` in case the tower can't do
       *          more than one attack and is homogenous to a
       *          duration.
       * @return - the attack speed for this tower.
       */
      float
      getAttackSpeed() const noexcept;

      float
      getProjectileSpeed() const noexcept;

      /**
       * @brief - Returns the percentage of reduction applied to
       *          the speed of mobs freezed by this tower.
       *          This value is expressed as a real percentage in
       *          the range `[0; 100]`.
       * @return - the freezing power of the tower.
       */
      float
      getFreezingPower() const noexcept;

      /**
       * @brief - Returns the percentage with which the speed
       *          of a mob freezed by this tower will decrease.
       *          This is expressed as a real percentage in the
       *          tange `[0; 100]`.
       * @return - the freezing speed as a percentage.
       */
      float
      getFreezingSpeed() const noexcept;

      utils::Duration
      getFreezingDuration() const noexcept;

      utils::Duration
      getPoisonDuration() const noexcept;

      /**
       * @brief - Returns the probability that a mob will get
       *          stunned by a hit of this tower. The value
       *          is expressed as a real percentage in the
       *          range `[0; 100]`.
       * @return - the stun probability for each hit of the
       *           tower as a percentage.
       */
      float
      getStunChance() const noexcept;

      utils::Duration
      getStunDuration() const noexcept;

      /**
       * @brief - Returns the aggregated cost of this tower,
       *          which corresponds to the total amount that
       *          has been spent to perform the upgrades and
       *          the initial buying cost.
       * @return - the total cost of the tower.
       */
      float
      getTotalCost() const noexcept;

      /**
       * @brief - Used to perform the upgrade of the tower to
       *          the provided level. In case the upgrade is
       *          not possible for this tower nothing will be
       *          changed.
       * @param upgrade - the upgrade to perform.
       * @param level - the level to which the corresponding
       *                properties should be upgraded.
       */
      void
      upgrade(const towers::Upgrade& upgrade,
              int level);

      void
      step(StepInfo& info) override;

      void
      pause(const utils::TimeStamp& t) override;

      void
      resume(const utils::TimeStamp& t) override;

    private:

      /**
       * @brief - Used to generate a damage data structure from the
       *          input properties describing a tower.
       * @param props - the properties to use to generate the damage
       *                data.
       * @return - the corresponding tower damage data.
       */
      static
      towers::DamageData
      fromProps(const TProps& props) noexcept;

      /**
       * @brief - Convenience method to allow grouping the projectile
       *          speed determination in a single method.
       * @param speed - the projectile speed to analyze.
       * @return - `true` if the input speed describes a projectile
       *           with infinite speed.
       */
      static
      bool
      hasInfiniteProjectileSpeed(float speed) noexcept;

      /**
       * @brief - This method is used to align the tower with the
       *          selected target. In case no target is defined
       *          yet the picking process will also be handled.
       * @param info - information to help the picking.
       * @return - `true` if the target could be picked and we
       *           are in such an orientation that a shot is
       *           possible.
       */
      bool
      pickAndAlignWithTarget(StepInfo& info);

      /**
       * @brief - Convenience method allowing to perform the attack
       *          of the current target given the properties of the
       *          tower.
       *          We don't verify whether the attack is possible
       *          given the current resource level and don't account
       *          for energy usage.
       * @param info - information to be able to spawn projectiles
       *               and generally handle the attack.
       * @return - the `return` value indicates whether or not the
       *           mob is still alive after the shot.
       */
      bool
      attack(StepInfo& info);

    private:

      /**
       * @brief - Defines a convenience structure to recall the
       *          status of an upgrade for this tower. This is
       *          used as a way to persist the upgrades made to
       *          the towers.
       */
      struct UpgradeData {
        towers::Upgrade type;
        int level;
      };

      /**
       * @brief - Defines a convenience structure regrouping the
       *          properties of the experience gained by this
       *          tower so far.
       */
      struct ExperienceData {
        // The total amount of experience gained by this tower.
        float exp;

        // The current level of the tower. Saved as a speed up
        // to avoid recomputing it from the raw experience when
        // needed.
        int level;
      };

      /**
       * @brief - The type of the tower. This is mostly used
       *          to quickly identify the tower but most of
       *          the precise behaviors are actually defined
       *          through the `m_process` attribute.
       */
      towers::Type m_type;

      /**
       * @brief - Defines the list of upgrades that are possible
       *          to improve this tower.
       */
      std::vector<UpgradeData> m_upgrades;

      /**
       * @brief - Defines the experience data for this tower.
       *          The experience is used to offset and improve
       *          the effectiveness of some processes.
       */
      ExperienceData m_exp;

      /**
       * @brief - An indication of the energy left for this
       *          tower to take actions.
       */
      float m_energy;

      /**
       * @brief - Measure the maximum energy that this tower
       *          can have. Above this value the energy won't
       *          be refilled anymore.
       */
      float m_maxEnergy;

      /**
       * @brief - Indication of how fast the energy pool of
       *          this tower can be refilled over time. A
       *          faster refill rate indicates a tower that
       *          can take more decisions.
       */
      float m_energyRefill;

      /**
       * @brief - Defines the energy cost of a single attack.
       *          This will consume some energy.
       */
      float m_attackCost;

      /**
       * @brief - Defines the minimum range of this tower.
       */
      float m_minRange;

      /**
       * @brief - Defines the maximum range of this tower.
       */
      float m_maxRange;

      /**
       * @brief - Defines the aoe radius for the projectiles
       *          fired by this tower.
       */
      float m_aoeRadius;

      /**
       * @brief - The definition of a cone within which a
       *          tower can shoot. When the angle between
       *          the target and the tower is less than
       *          this value a shot is possible. It is
       *          expressed in radians.
       */
      float m_shootAngle;

      /**
       * @brief - The speed at which projectiles are fired
       *          by this tower. In case this value is set
       *          to a negative number we consider that the
       *          projectile has indeed infinite speed and
       *          won't create the related objects.
       */
      float m_projectileSpeed;

      /**
       * @brief - The duration of the aiming process for the
       *          tower. It indicates an interval where the
       *          tower needs to be aligned with the target
       *          (or at least within the shooting angle) to
       *          somehow prepare the hit. It resets as soon
       *          as the target exits the aim cone.
       */
      float m_aimSpeed;

      /**
       * @brief - Defines the damage data for this tower.
       */
      towers::DamageData m_attack;

      /**
       * @brief - Defines the custom processes attached to
       *          this tower.
       */
      towers::Data m_processes;

      /**
       * @brief - The target for this tower. Until it is
       *          destroyed or somehow made unavailable
       *          we will keep locking and firing on it.
       */
      MobShPtr m_target;
  };

  using TowerShPtr = std::shared_ptr<Tower>;
}

# include "Tower.hxx"

#endif    /* TOWER_HH */
