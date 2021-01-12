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
        float aoeDamage;

        float accuracy;

        float freezePercent;
        utils::Duration freezeSpeed;
        utils::Duration freezeDuration;

        float poison;
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
      static constexpr const float sk_arrived = 0.0001f;

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
       */
      float m_damage;

      /**
       * @brief - The radius of the aoe where damage is
       *          applied when the projectile hits.
       */
      float m_aoeRadius;

      /**
       * @brief - The value of the aoe damage of the
       *          projectile.
       */
      float m_aoeDamage;

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
       * @brief - The speed of freezing for this projectile.
       *          Expressed in milliseconds.
       */
      utils::Duration m_freezeSpeed;

      /**
       * @brief - The duration of the freeze effect for this
       *          projectile. Expressed in milliseconds.
       */
      utils::Duration m_freezeDuration;

      /**
       * @brief - The poison damage expressed in units per
       *          second for this projectile. It will be
       *          applied for the whold duration of the
       *          effect.
       */
      float m_poison;

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
