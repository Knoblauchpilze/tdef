#ifndef    STEP_INFO_HH
# define   STEP_INFO_HH

# include <vector>
# include <memory>
# include <core_utils/RNG.hh>
# include <core_utils/TimeUtils.hh>

namespace tdef {

  class Mob;
  using MobShPtr = std::shared_ptr<Mob>;

  /**
   * @enum  - Convenience structure regrouping all variables
   *          needed to perform the advancement of one step
   *          of a world object. It includes a RNG, info on
   *          the dimensions of the world, etc.
   */
  struct StepInfo {
    utils::RNG& rng;

    utils::TimeStamp moment;
    float elapsed;

    std::vector<MobShPtr> mSpawned;
    std::vector<Mob*> mRemoved;

    void
    spawnMob(MobShPtr m);

    void
    removeMob(Mob* m);
  };

}

#endif    /* STEP_INFO_HH */