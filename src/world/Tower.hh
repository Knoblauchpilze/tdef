#ifndef    TOWER_HH
# define   TOWER_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"

namespace tdef {

  class Tower: public Block {
    public:

      /**
       * @brief - Definition of new props to define a tower.
       */
      struct TProps: public Block::BProps {
        float energy;
        float maxEnergy;
        float refill;

        float range;
        float attack;
        float attackCost;
      };

      static
      TProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param props - the properties defining this wall.
       */
      Tower(const TProps& props);

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
       * @brief - Defines the range of this tower.
       */
      float m_range;

      /**
       * @brief - Defines the attack value of this tower.
       */
      float m_attack;
  };

  using TowerShPtr = std::shared_ptr<Tower>;
}

# include "Tower.hxx"

#endif    /* TOWER_HH */
