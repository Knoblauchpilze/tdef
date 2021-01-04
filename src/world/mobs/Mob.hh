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
      // effect of the damage.
      float hit;

      // A ratio in the range `[0; 1]` indicating how the
      // speed of the mob should be affected. A stun is
      // indicated by a value of `0`.
      float speed;

      // Measure how long the mob will be affected by the
      // slowing effect.
      utils::Duration sDuration;

      // A value indicating how much damage the mob should
      // take every second from being poisoned.
      float poison;

      // Measure how long the mob will take damage from
      // the poisoning effect.
      utils::Duration pDuration;
    };

    /**
     * @brief - Convenience structure defining all props
     *          allowing a mob to defend itself.
     */
    struct DefenseData {
      float shield;

      bool poisonable;
      bool slowable;
      bool stunnable;
    };

  }

  class Mob: public WorldElement {
    public:

      /**
       * @brief - Definition of new props to define a mob.
       */
      struct MProps: public WorldElement::Props {
        float energy;
        float maxEnergy;
        float refill;

        float speed;
        float arrival;

        float bounty;
        float lives;

        float shield;

        bool poisonable;
        bool slowable;
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
      const path::Path&
      getPath() const noexcept;

      float
      getBounty() const noexcept;

      float
      getCost() const noexcept;

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
       * @param d - the damage to apply.
       * @return - `true` if the mob is still alive.
       */
      bool
      damage(const mobs::Damage& d);

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

    private:

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
       * @brief - Defines the arrival radius for this mob when
       *          approaching its destination.
       */
      float m_rArrival;

      /**
       * @brief - Speed of the entity in cells per second.
       */
      float m_speed;

      /**
       * @brief - The current path followed by this mob.
       */
      path::Path m_path;

      /**
       * @brief - The bounty provided by this mob if killed.
       */
      float m_bounty;

      /**
       * @brief - The cost to let this mob pass through a portal.
       */
      float m_cost;

      /**
       * @brief - The defense properties of this mob. Used to
       *          somewhat mitugate any incoming damage.
       */
      mobs::DefenseData m_defense;
  };

  using MobShPtr = std::shared_ptr<Mob>;
}

# include "Mob.hxx"

#endif    /* MOB_HH */
