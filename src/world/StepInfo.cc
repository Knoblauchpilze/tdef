
# include "StepInfo.hh"

namespace tdef {

  void
  StepInfo::spawnMob(MobShPtr m) {
    mSpawned.push_back(m);
  }

  void
  StepInfo::removeMob(Mob* m) {
    mRemoved.push_back(m);
  }

}
