#ifndef    PROJECTILE_HH
# define   PROJECTILE_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include <core_utils/TimeUtils.hh>
# include "WorldElement.hh"
# include "Mob.hh"

namespace tdef {

  class Projectile: public WorldElement {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct PProps: public WorldElement::Props {
        float speed;

        float damage;
        float aoeRadius;

        float accuracy;

        float freezePercent;
        float freezeSpeed;

        // A value in the range `[0; 1]` indicating how
        // likely it is that this projectil stuns the
        // target.
        float stunProb;

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
       * @param mob - the mob which is targeted by this projectile.
       */
      Projectile(const PProps& props,
                 MobShPtr mob);

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
       * @brief - Defines the distance below which the projectile
       *          is considered to have reach its target.
       */
      static constexpr float sk_arrived = 0.0001f;

      /**
       * @brief - The target for this projectile. We will
       *          perform some position tracking so that
       *          the projectile follows its target until
       *          it hits it.
       */
      MobShPtr m_target;

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

# include "Projectile.hxx"

#endif    /* PROJECTILE_HH */
