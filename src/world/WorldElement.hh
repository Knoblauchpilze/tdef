#ifndef    WORLD_ELEMENT_HH
# define   WORLD_ELEMENT_HH

# include <memory>
# include <core_utils/TimeUtils.hh>
# include <core_utils/CoreObject.hh>
# include <core_utils/Uuid.hh>
# include <maths_utils/Point2.hh>
# include "StepInfo.hh"

namespace tdef {

  class WorldElement: public utils::CoreObject {
    public:

      /**
       * @brief - Retrieve the position of this element.
       * @return - the position for this element.
       */
      const utils::Point2f&
      getPos() const noexcept;

      /**
       * @brief - Interrogate the internal identifier for the
       *          owner of this entity and return `true` if
       *          it is valid.
       * @return - `true` if the owner is valid.
       */
      bool
      isOwned() const noexcept;

      /**
       * @brief - Return a reference to the identifier of the
       *          owner of this element. Note that the uuid
       *          might be invalid if the `isOwned` returns
       *          `true`.
       * @return - the identifier of the owner of this elem.
       */
      const utils::Uuid&
      getOwner() const noexcept;

      /**
       * @brief - Return the radius for this entity.
       * @return - the radius for this entity in blocks.
       */
      float
      getRadius() const noexcept;

      /**
       * @brief - Fetch the current health for this world
       *          element. This value corresponds to the
       *          total health pool.
       * @return - the current health.
       */
      float
      getHealth() const noexcept;

      /**
       * @brief - Retrieve the ratio of current health over
       *          total health for this element. This value
       *          is in the range `[0; 1]` and allows to
       *          measure how healthy this element it.
       * @return - a ratio measuring the health of this item.
       */
      float
      getHealthRatio() const noexcept;

      /**
       * @brief - Return `true` in case this element is marked
       *          for deletion and will probably be removed in
       *          the next execution of the influences.
       * @return - `true` if the block is marked for deletion.
       */
      bool
      isDead() const noexcept;

      /**
       * @brief - Defines whether this element has already been
       *          marked for deletion.
       * @return - `true` if the element has been deleted.
       */
      bool
      isDeleted() const noexcept;

      /**
       * @brief - Used to hit the element with the specified
       *          amount of damage. We clamp the amount of
       *          health to `0` and the return value tells
       *          whether or not the element is still alive.
       *          Note that in case the `hit` is negative
       *          it will correspond to some healing done
       *          to the element.
       * @param hit - the amount of damage to apply to the
       *              element.
       */
      bool
      damage(float hit);

      /**
       * @brief - Interface method caled before the first
       *          execution of this element with the info
       *          on the step at the moment of the spawn.
       *          It allows to initialize some values.
       * @param info - the info of the step.
       */
      virtual void
      init(StepInfo& info) = 0;

      /**
       * @brief - Interface method allowing for a world element
       *          to evolve based on its surroundings. We use a
       *          struct gathering the current state of the
       *          world to take decisions based on it.
       *          Inheriting classes can subclass this method
       *          to provide custom behavior.
       * @param info - the information to get data on the step.
       */
      virtual void
      step(StepInfo& info) = 0;

      /**
       * @brief - Interface method allowing to pause this world
       *          element: all internal behavior that depend on
       *          time should be stopped so that they can be
       *          later resumed.
       * @param t - the timestamp at which the pause occur.
       */
      virtual void
      pause(const utils::TimeStamp& t) = 0;

      /**
       * @brief - Opposite operation to the `pause`, this method
       *          is called to indicate that internal processes
       *          for this world element can be resumed.
       * @param t - the timestamp at which the resume occur.
       */
      virtual void
      resume(const utils::TimeStamp& t) = 0;

      /**
       * @brief - Interface method called before an element is
       *          removed from the game.
       * @param info - the information about the step.
       */
      virtual void
      destroy(StepInfo& info) = 0;

      /**
       * @brief - Interface method called whenever a change in
       *          the world occurs that might lead the element
       *          to reconsider some of the choices made.
       *          This typically include a new block spawned
       *          (which might change the pathes).
       * @param loc - information about the current step of
       *               the game.
       */
      virtual void
      worldUpdate(LocatorShPtr loc) = 0;

    protected:

      /**
       * @brief - Convenience structure to regroup the info
       *          needed to create a world element. It is
       *          meant as a way to reduce the number of
       *          arguments provided to the constructor of
       *          this class.
       */
      struct Props {
        utils::Point2f pos;
        float radius;

        float health;

        utils::Uuid owner;

        /**
         * @brief - Used to make the compiler consider this
         *          structure as polymorphic and thus allow
         *          dynamic_cast operator on derived elems.
         */
        virtual ~Props();
      };

      static
      void
      assignProps(Props& pp,
                  const utils::Point2f& p,
                  const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Build a new world element with the specified
       *          position and name.
       * @param props - the properties to use to define this
       *                world element.
       * @param name - the name of the element.
       */
      WorldElement(const Props& props,
                   const std::string& name);

      /**
       * @brief - Used to define a new owner for this element.
       * @param uuid - the identifier of the new owner of the
       *               element.
       */
      void
      setOwner(const utils::Uuid& uuid);

      /**
       * @brief - Mark this item for deletion based on the
       *          status provided in input.
       * @param toDelete - `true` if the item should be
       *                   marked for deletion.
       */
      void
      markForDeletion(bool toDelete);

    protected:

      /**
       * @brief - The identifier of the owner of this element.
       *          It is used to make sure that elements can
       *          cooperate and identify each other in the
       *          simulation.
       */
      utils::Uuid m_owner;

      /**
       * @brief - The home position of this mob. Used when the
       *          entity needs to come back home.
       */
      utils::Point2f m_pos;

      /**
       * @brief - Define the size of the entity expressed in
       *          blocks. This value is guaranteed to be at
       *          least `0`.
       */
      float m_radius;

      /**
       * @brief - The total health pool for this element. It is
       *          used as a reminder of how much health in total
       *          the element had, before anything happened to
       *          it.
       */
      float m_totalHealth;

      /**
       * @brief - The current health pool for this element.
       */
      float m_health;

      /**
       * @brief - Defines that this world element has been marked
       *          for deletion.
       */
      bool m_deleted;
  };

  using WorldElementShPtr = std::shared_ptr<WorldElement>;
}

# include "WorldElement.hxx"

#endif    /* WORLD_ELEMENT_HH */
