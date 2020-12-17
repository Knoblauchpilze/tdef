#ifndef    MOB_HH
# define   MOB_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "WorldElement.hh"

namespace tdef {

  class Mob: public WorldElement {
    public:

      /**
       * @brief - Creates a new mob with the specified position.
       * @param pos - the position of this mob.
       * @param energy - the initial energy for this mob.
       * @param maxEnergy - the maximum energy for this mob.
       * @param refill - the amount of energy refilled at each
       *                 second.
       * @param owner - the owner of the mob.
       */
      Mob(const utils::Point2f& pos,
          float energy,
          float maxEnergy,
          float refill,
          const utils::Uuid& owner = utils::Uuid());

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
       * @brief - An indication of the energy left for this
       *          mob to take actions. This include spawning
       *          new pheromones for example.
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
  };

  using MobShPtr = std::shared_ptr<Mob>;
}

# include "Mob.hxx"

#endif    /* MOB_HH */
