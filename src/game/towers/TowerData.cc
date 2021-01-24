
# include "TowerData.hh"
# include "Tower.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace tdef {
  namespace towers {

    MobShPtr
    basicTargetPicking(StepInfo& info, PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.maxRange, nullptr);
    }

    bool
    basicDamaging(StepInfo& info, MobShPtr mob, Damage& data) {
      mobs::Damage d;
      d.hit = data.damage;

      d.accuracy = data.accuracy;

      d.speed = data.speed;
      d.sDecraseSpeed = data.slowdown;

      d.stunProb = data.stunProb;

      d.fDuration = data.fDuration;
      d.sDuration = data.sDuration;
      d.pDuration = data.pDuration;

      return mob->hit(info, d);
    }

  }
}
