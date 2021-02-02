#ifndef    MOB_HH
# define   MOB_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "WorldElement.hh"
# include "Path.hh"

namespace tdef {
  namespace mobs {

    /**
     * @brief - Defines the possible types for a mob
     *          in the world.
     */
    enum class Type {
      Regular,
      Fast,
      Strong,
      Heli,
      Jet,
      Armored,
      Healer,
      Toxic,
      Icy,
      Fighter,
      Light,
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
     * @brief - Convenience structure regrouping the info
     *          needed to perform damage to a mob.
     */
    struct Damage {
      // The number of hit points to subtract for the full
      // effect of the damage. Note that this value applies
      // every second for the duration of the poisoning in
      // case the `pDuration` is set to a non-zero value.
      float hit;

      // The accuracy of the damage: this value in the range
      // `[0; 1]` defines a chance for the damage to miss the
      // to apply its damage.
      float accuracy;

      // A ratio in the range `[0; 1]` indicating how the
      // speed of the mob should be affected. A stun is
      // indicated by a value of `0`.
      float speed;

      // A measure in percent of the decrease in speed
      // that is applied each second. Using this and the
      // `speed` one can work out the time it will take
      // for the mob to actually reach the slowed speed.
      float sDecraseSpeed;

      // A value in the range `[0; 1]` indicating how
      // likely it is for the mob to be stunned.
      float stunProb;

      // Measure how long the mob will be affected by the
      // slowing effect.
      utils::Duration fDuration;

      // Measure how long the mob will be affected by the
      // stunning effect.
      utils::Duration sDuration;

      // Measure how long the mob will take damage from
      // the poisoning effect.
      utils::Duration pDuration;
    };

    /**
     * @brief - Convenience structure defining all props
     *          allowing a mob to defend itself.
     */
    struct DefenseData {
      // Defines the shield as a percentage of the total
      // health of the mob owning this data. This value
      // is in the range `[0; 1]`.
      float shield;

      // Defines the percentage of the incoming damage
      // that is reduced by a functional shield.
      float shieldEfficiency;

      // Defines the durability of the shield. Starts
      // with a value and decreases each time the shield
      // absorbs some damage until it gets destroyed.
      // Belongs to the range `[0; 1]`.
      float shieldDurability;

      // Whether or not the mob is poisonable.
      bool poisonable;

      // Whether or not the mob is slowable.
      bool slowable;

      // Whether or not the mob can be stunned. Separate
      // from the slowable property.
      bool stunnable;
    };

  }

  // Forward declaration of the block class to be able
  // to use it directly.
  class Block;
  using BlockShPtr = std::shared_ptr<Block>;

  class Mob: public WorldElement {
    public:

      /**
       * @brief - Definition of new props to define a mob.
       */
      struct MProps: public WorldElement::Props {
        float energy;
        float maxEnergy;
        float refill;

        // Defines the base speed of the mob without any
        // modifiers. This is the speed that is reached
        // by the mob given enough free time.
        float speed;

        // Defines the acceleration rate for this mob.
        // It is applied to get back up to seepd in case
        // it has been slowed down for some reasons.
        // This is defined as unit of speed per second.
        float acceleration;

        // Defines the arrival radius: when the mob is
        // closer to its target than this distance we
        // consider that it has arrived.
        float arrival;

        // Defines the damage that this mob hit with on
        // each attack.
        float attack;

        // Defines the cost of performing a single attack
        // for this mob.
        float attackCost;

        // The amount of gold that this mob yields when
        // killed.
        float bounty;

        // The amount of lives that this mob is worth if
        // it passes through a portal.
        float lives;

        // The amount of experience that this mob yields
        // when it is killed.
        float exp;

        // Defines the amount of shield this mob has as a
        // percentage of its total health. Typically if a
        // value of `0.5` is specified this means that the
        // shield is equal to `0.5 * health`.
        // Should be in the range `[0; 1]`.
        float shield;

        // Defines the amount of damage absorbed by the
        // shield as long as it has some durability. The
        // value is expressed as a percentage of the
        // incoming damage and is in the range `[0; 1]`.
        float shieldEfficiency;

        // Whether or not the mob can be poisoned.
        bool poisonable;

        // Whether or not the mob can be slowed.
        bool slowable;

        // Whether or not the mob can be stunned.
        bool stunnable;

        // The `type` defines a representation of the real
        // kind of the mob. If one is using the factory
        // mobs with the same type should have similar
        // properties but it's not guaranteed otherwise.
        mobs::Type type;
      };

      static
      MProps
      newProps(const utils::Point2f& p,
               const mobs::Type& type = mobs::Type::Regular,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Creates a new mob with the specified props
       * @param props - the properties defining this mob.
       */
      Mob(const MProps& props);

      /**
       * @brief - Fetch the type for this mob.
       * @return - the type for this mob.
       */
      const mobs::Type&
      getType() const noexcept;

      /**
       * @brief - Returns the current path followed by the entity.
       * @return - the current path followed by the entity.
       */
      const Path&
      getPath() const noexcept;

      float
      getBounty() const noexcept;

      float
      getExpReward() const noexcept;

      float
      getSpeed() const noexcept;

      /**
       * @brief - Convenience wrapper to determine whether this
       *          entity is still en route on its current path.
       * @return - `true` if the entity is still moving along
       *           its path.
       */
      bool
      isEnRoute() const noexcept;

      /**
       * @brief - Used to interpret the damage structure provided
       *          in input and to apply it to the mob. The mob is
       *          able to mitigate some of it.
       *          In case the mob is damaged to the point where it
       *          reaches `0` hp, we will mark it for deletion.
       * @param info - the information to generate random numbers
       *               and get an idea of the time frame for which
       *               damage is applied.
       * @param d - the damage to apply.
       * @return - `true` if the mob is still alive.
       */
      bool
      hit(StepInfo& info,
          const mobs::Damage& d);

      std::ostream&
      operator<<(std::ostream& out) const override;

      std::istream&
      operator>>(std::istream& in) override;

      void
      init(StepInfo& info) override;

      void
      step(StepInfo& info) override;

      void
      pause(const utils::TimeStamp& t) override;

      void
      resume(const utils::TimeStamp& t) override;

      void
      destroy(StepInfo& info) override;

      void
      worldUpdate(LocatorShPtr loc) override;

    private:

      /**
       * @brief - Used to generate a defense data structure from the
       *          input properties describing a mob.
       * @param props - the properties to use to generate the defense
       *                data.
       * @return - the corresponding mob defense data.
       */
      static
      mobs::DefenseData
      fromProps(const MProps& props) noexcept;

      /**
       * @brief - Used to apply the hit damage described by the
       *          input structure to this mob. The mob will use
       *          its shielding data to mitigate the damage.
       * @param info - info about the damage to apply.
       * @param d - the description of the damage to apply.
       */
      void
      applyDamage(StepInfo& info,
                  const mobs::Damage& d);

      /**
       * @brief - Used to apply the freezing damage described by the
       *          input structure to this mob. In case the mob can't
       *          be freezed nothing will happen.
       * @param info - info about the damage to apply.
       * @param d - the description of the damage to apply.
       */
      void
      applyFreezing(StepInfo& info,
                    const mobs::Damage& d);

      /**
       * @brief - Used to apply the stunning damage described by the
       *          input structure to this mob. In case the mob can't
       *          be stunned nothing will happend.
       * @param info - info about the damage to apply.
       * @param d - the description of the damage to apply.
       */
      void
      applyStunning(StepInfo& info,
                    const mobs::Damage& d);

      /**
       * @brief - Used to apply the poison damage described by the
       *          input structure to this mob. In case the mob is
       *          not poisonable nothing will happen.
       * @param info - info about the damage to apply.
       * @param d - the description of the damage to apply.
       */
      void
      applyPoison(StepInfo& info,
                  const mobs::Damage& d);

      /**
       * @brief - Used every frame to update the speed of the mob
       *          based on the current effects applied to it. The
       *          aim of this method is to have a usable speed at
       *          the end of the function.
       * @param info - information about the elapsed step.
       */
      void
      updateSpeed(StepInfo& info);

      /**
       * @brief - Used every frame to update the health of the
       *          mob based on the damage it's taking from applied
       *          effects.
       * @param info - information about the elapsed step.
       */
      void
      updateHealth(StepInfo& info);

      /**
       * @brief - Attemps to locate a portal and generate a path
       *          to go there. If this works, the path will be
       *          assigned and the behavior will be set to portal
       *          seeking.
       * @param loc - a locator allowing to search elements in
       *              the world.
       * @param path - the path to generate. Will be cleared
       *               before being used.
       * @return - `true` if a path could be generated.
       */
      bool
      locatePortal(LocatorShPtr loc,
                   Path& path);

      /**
       * @brief - Used to trigger and apply the behavior which
       *          consists in finding the closest wall or tower
       *          and go to it to destroy it.
       *          This is usually used when the portal can not
       *          be located.
       * @param loc - a locator allowing to search elements in
       *              the world.
       * @param path - the path to generate. Will be cleared
       *               before being used.
       * @return - `true` if a path could be generated.
       */
      bool
      destroyDefenses(LocatorShPtr loc,
                      Path& path);

    private:

      /**
       * @brief - Convenience structure regrouping the props
       *          needed to manage the speed of a mob. It is
       *          mainly used to hold the freezing effect to
       *          apply on top of the regular speed.
       */
      struct SpeedData {
        // The base speed of the mob: this is the speed it
        // will reach without any modifiers.
        float bSpeed;

        // The current speed of the mob. This includes the
        // current modifiers (like freezing and stun) that
        // might be applied.
        float speed;

        // Defines the timestamp at which the freezing has
        // begun for this mob.
        utils::TimeStamp tFreeze;

        // Defines the duration of the freezing effect. The
        // mob will start to accelerate after the timestamp
        // reaches this value.
        utils::Duration fDuration;

        // Defines the speed to reach if the freezing effect
        // lasts for long enough. If it were to last forever
        // the `speed` would reach this value.
        // Note that this is expressed in percentage of the
        // base speed so it is in the range `[0; 1]`.
        float fSpeed;

        // Defines the speed at which the mob is slowed down
        // during the duration of the freezing effect. Note
        // that depending on the duration it might mean that
        // the mob won't ever reach `fSpeed`. This is based
        // on a percentage value, indicating how much of the
        // current speed is lost each second.
        // So a value of `0.12` indicates that each second
        // the speed will be decreased by 12%. This value is
        // in the range `[0; 1]`.
        float sDecrease;

        // Defines the speed at which the mob is accelerating
        // when the speed is slower than the desired one. As
        // for the `sDecrease` it is expressed as a percentage
        // of the speed that is gained each second.
        float sIncrease;

        // Defines the timestamp when this mob was stunned.
        utils::TimeStamp tStun;

        // Defines the duration of the stun effect. When the
        // current timestamp will reach this value the mob
        // will start to accelerate back to the expected
        // speed given the greezing effect.
        utils::Duration sDuration;
      };

      /**
       * @brief - Convenience structure regrouping the props
       *          needed to manage the poisoning status for
       *          this mob.
       *          We handle stacking and poisoning duration.
       */
      struct PoisonData {
        // The amount of damage per second that occur due to
        // the poisoning effect.
        float damage;

        // Defines a counter indicating how many times this
        // mob has been posioned. The more stack there are,
        // the less efficient they will be (relatively, as
        // in any new stack will always deal more damage but
        // not as much as the first stack).
        int stack;

        // Defines the timestamp when this mob was first hit
        // by a poison effect. The duration starts ticking
        // from this point.
        utils::TimeStamp tPoison;

        // Defines the duration of the poisoning effect. If
        // the current timestamp reaches this point the mob
        // will no longer be poisoned.
        utils::Duration pDuration;
      };

      /**
       * @brief - Convenience enumeration defining the modes
       *          available for the mob: this describes the
       *          possible behaviors that can be adopted by
       *          the mob.
       */
      enum class Behavior {
        None,
        PortalSeeker,
        WallBreaker
      };

      /**
       * @brief - A factor describing how the durability of
       *          the shield is affected by each hit.
       */
      static constexpr float sk_shieldHitDurab = 0.01f;

      /**
       * @brief - The maximum number of poison stacks to be
       *          applied to a mob.
       */
      static constexpr int sk_poisonStacksLimit = 5;

      /**
       * @brief - The maximum distance from the mob that the
       *          path finding routines can reach before we
       *          declare that it is impossible to find.
       *          This will be used to trigger the wall or
       *          tower breaking behavior.
       */
      static constexpr float sk_maxPathFindingDistance = 25.0f;

      /**
       * @brief - The type of the mob. This is mostly used
       *          to quickly identify the mob but most of
       *          the precise behaviors are actually defined
       *          through custom processes attached for the
       *          attack behavior.
       */
      mobs::Type m_type;

      /**
       * @brief - An indication of the energy left for this
       *          mob to take actions.
       */
      float m_energy;

      /**
       * @brief - Measure the maximum energy that this mob can
       *          have. Above this value the energy won't be
       *          refilled anymore.
       */
      float m_maxEnergy;

      /**
       * @brief - Indication of how fast the energy pool of
       *          this entity can be refilled over time. A
       *          faster refill rate indicates an entity that
       *          can take more decisions.
       */
      float m_energyRefill;

      /**
       * @brief - The behavior currently adopted by the mob.
       */
      Behavior m_behavior;

      /**
       * @brief - The energy cost of each attack for this mob.
       *          This value in combination with the energy is
       *          defining how fast the mob can attack.
       */
      float m_attackCost;

      /**
       * @brief - Defines the damage value that this mob is
       *          dealing when attacking.
       */
      float m_attack;

      /**
       * @brief - Defines the arrival radius for this mob when
       *          approaching its destination.
       */
      float m_rArrival;

      /**
       * @brief - The current path followed by this mob.
       */
      Path m_path;

      /**
       * @brief - The bounty provided by this mob if killed.
       */
      float m_bounty;

      /**
       * @brief - The cost to let this mob pass through a portal.
       */
      float m_cost;

      /**
       * @brief - The amount of experience given by this mob
       *          if killed.
       */
      float m_exp;

      /**
       * @brief - The defense properties of this mob. Used to
       *          somewhat mitugate any incoming damage.
       */
      mobs::DefenseData m_defense;

      /**
       * @brief - Defines the common handler to regroup all the
       *          properties needed to manage the speed of this
       *          mob.
       */
      SpeedData m_speed;

      /**
       * @brief - Defines the common handler to regroup all the
       *          properties needed to manage the poisoning of
       *          this mob.
       */
      PoisonData m_poison;

      /**
       * @brief - The target for this mob. Until it is reached
       *          or somehow made unavailable we will try to
                  reach it.
       */
      BlockShPtr m_target;
  };

  using MobShPtr = std::shared_ptr<Mob>;
}

std::ostream&
operator<<(std::ostream& out, const tdef::Mob& m) noexcept;

std::istream&
operator>>(std::istream& in, tdef::Mob& m) noexcept;

# include "Mob.hxx"

#endif    /* MOB_HH */
