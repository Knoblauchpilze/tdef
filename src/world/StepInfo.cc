
# include "StepInfo.hh"

namespace tdef {

  void
  StepInfo::spawnMob(MobShPtr m) {
    mSpawned.push_back(m);
  }

  void
  StepInfo::spawnProjectile(ProjectileShPtr p) {
    pSpawned.push_back(p);
  }

}
