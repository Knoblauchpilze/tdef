#ifndef    TOWER_HH
# define   TOWER_HH

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
      float damage;

      float aoeRadius;
      float aoeDamage;

      float accuracy;

      float speed;
      utils::Duration sDuration;

      float poison;
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

  }

  class Tower: public Block {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct TProps: public Block::BProps {
        float energy;
        float maxEnergy;
        float refill;

        float minRange;
        float maxRange;

        float damage;
        float aoeRadius;
        float aoeDamage;

        float attackSpeed;
        float rotation;
        float aimSpeed;
        float projectileSpeed;
        float accuracy;

        float duration;
        float shootAngle;
        int projectiles;
        float acceleration;

        float freezePercent;
        float freezeSpeed;

        float attackCost;

        // The `type` defines a representation of the real
        // kind of the tower. If one is using the factory
        // towers with the same type should have similar
        // properties but it's not guaranteed otherwise.
        towers::Type type;
      };

      static
      TProps
      newProps(const utils::Point2f& p,
               const towers::Type& type = towers::Type::Basic,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param props - the properties defining this wall.
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

      /**
       * @brief - Implementation of the interface method to evolve
       *          this mob.
       */
      void
      step(StepInfo& info) override;

      /**
       * @brief - Implementation of the interface method to pause
       *          the internal processes for this mob. Note that
       *          it is mostly provided so that this class is not
       *          abstract as there are not really any process to
       *          pause at this time.
       * @param t - the timestamp at which the pause occur.
       */
      void
      pause(const utils::TimeStamp& t) override;

      /**
       * @brief - Implementation of the interface method to resume
       *          the internal processes for this mob.
       * @param t - the timestamp at which the resume occur.
       */
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

    private:

      /**
       * @brief - The type of the tower. This is mostly used
       *          to quickly identify the tower but most of
       *          the precise behaviors are actually defined
       *          through the `m_process` attribute.
       */
      towers::Type m_type;

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
       * @brief - Defines the damage data for this tower.
       */
      towers::DamageData m_attack;

      /**
       * @brief - Defines the custom processes attached to
       *          this tower.
       */
      towers::Data m_processes;
  };

  using TowerShPtr = std::shared_ptr<Tower>;
}

# include "Tower.hxx"

#endif    /* TOWER_HH */
