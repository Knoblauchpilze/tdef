#ifndef    PROJECTILE_HH
# define   PROJECTILE_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include <core_utils/TimeUtils.hh>
# include "WorldElement.hh"
# include "Mob.hh"

namespace tdef {

  // Forward declaration of the Tower class.
  class Tower;

  class Projectile: public WorldElement {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct PProps: public WorldElement::Props {
        float speed;

        float damage;
        float aoeRadius;

        // A value in the range `[0; 1]` defining the chance
        // to miss a hit where `0` means that the tower will
        // always miss and `1` that it will never miss.
        float accuracy;

        // Defines the percentage of reduction of the speed
        // of mobs. Value in the range `[0; 1]`.
        float freezePercent;

        // Defines the percentage of speed that is reduced
        // per second when an entity is within the range
        // of this tower. Value in the range `[0; 1]`.
        float freezeSpeed;

        // A value in the range `[0; 1]` indicating how
        // likely it is that this projectil stuns the
        // target.
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

        // Duration of the freezing effect applied by the
        // projectile, expressed in milliseconds.
        utils::Duration freezeDuration;

        // Duration of the stunning effect applied by the
        // projectile, expressed in milliseconds.
        utils::Duration stunDuration;

        // Duration of the poisoning effect applied by the
        // projectile, expressed in milliseconds. Note that
        // in case the duration is not zero we will assign
        // the damage as poison damage.
        utils::Duration poisonDuration;
      };

      static
      PProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Creates a new projectile with the specified
       *          properties.
       * @param props - the properties defining this projectile.
       * @param owner - defines the tower that fired this projectile.
       *                It is used as a way to transmit the exp gain
       *                to the tower in case this projectile kills a
       *                mob.
       * @param mob - the mob which is targeted by this projectile.
       */
      Projectile(const PProps& props,
                 Tower* owner,
                 MobShPtr mob);

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
       * @brief - Used to update the tracked destination of the
       *          projectile from the target if any is defined.
       *          In case none is defined the destination is left
       *          unchanged.
       */
      void
      updateTrackedDestination();

    private:

      /**
       * @brief - Defines the distance below which the projectile
       *          is considered to have reach its target.
       */
      static constexpr float sk_arrived = 0.01f;

      /**
       * @brief - The target for this projectile. We will
       *          perform some position tracking so that
       *          the projectile follows its target until
       *          it hits it.
       */
      MobShPtr m_target;

      /**
       * @brief - The position followed by this projectile.
       *          It is updated at each step from the target
       *          in case it is available (to make the proj
       *          track an element) or used as is in case the
       *          target is not defined (typically after the
       *          projectile has been restored).
       */
      utils::Point2f m_dest;

      /**
       * @brief - The parent tower of this projectile: it
       *          corresponds to the tower that fired the
       *          projectile and is used to propagate any
       *          experience gain to it.
       */
      Tower* m_tower;

      /**
       * @brief - The travelling speed for this projectile.
       */
      float m_speed;

      /**
       * @brief - The main damage value for this projectile.
       *          Note that in case the poison duration is
       *          not null we will consider that the damage
       *          is actually applied over the duration of
       *          the poison where this value is dealt each
       *          second.
       */
      float m_damage;

      /**
       * @brief - The radius of the aoe where damage is
       *          applied when the projectile hits.
       */
      float m_aoeRadius;

      /**
       * @brief - The accuracy of this projectile in the
       *          range `[0; 1]`. It defines how likely
       *          this projectile is to miss its target.
       */
      float m_accuracy;

      /**
       * @brief - The percentage of speed reduction that is
       *          applied by this projectile.
       */
      float m_freezePercent;

      /**
       * @brief - The percentage by which the speed of the
       *          target is reduced upon touching this
       *          projectile. The freezing will be applied
       *          for the specified duration.
       */
      float m_freezeSpeed;

      /**
       * @brief - A value in the range `[0; 1]` indicating how
       *          likely it is for this projectile to stun its
       *          target.
       */
      float m_stunProb;

      /**
       * @brief - A value in the range `[0; 1]` defining how
       *          likely it is for this projectile to land a
       *          critical hit (dealing more damage).
       */
      float m_critProb;

      /**
       * @brief - A value defining the multiplier to compute
       *          the damage for a critical hit.
       */
      float m_critMultiplier;

      /**
       * @brief - The duration of the freeze effect for this
       *          projectile. Expressed in milliseconds.
       */
      utils::Duration m_freezeDuration;

      /**
       * @brief - The duration of the stun effect for this
       *          projectile. Expressed in milliseconds.
       */
      utils::Duration m_stunDuration;

      /**
       * @brief - The duration of the poisoning effect of
       *          this projectile expressed in milliseconds.
       */
      utils::Duration m_poisonDuration;
  };

  using ProjectileShPtr = std::shared_ptr<Projectile>;
}

std::ostream&
operator<<(std::ostream& out, const tdef::Projectile& p) noexcept;

std::istream&
operator>>(std::istream& in, tdef::Projectile& p) noexcept;

# include "Projectile.hxx"

#endif    /* PROJECTILE_HH */
