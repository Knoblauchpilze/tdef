#ifndef    MOB_HH
# define   MOB_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "WorldElement.hh"
# include "Path.hh"

namespace tdef {

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
      };

      static
      MProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Creates a new mob with the specified props
       * @param props - the properties defining this mob.
       */
      Mob(const MProps& props);

      /**
       * @brief - Returns the current path followed by the entity.
       * @return - the current path followed by the entity.
       */
      const path::Path&
      getPath() const noexcept;

      /**
       * @brief - Convenience wrapper to determine whether this
       *          entity is still en route on its current path.
       * @return - `true` if the entity is still moving along
       *           its path.
       */
      bool
      isEnRoute() const noexcept;

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

    private:

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
  };

  using MobShPtr = std::shared_ptr<Mob>;
}

# include "Mob.hxx"

#endif    /* MOB_HH */