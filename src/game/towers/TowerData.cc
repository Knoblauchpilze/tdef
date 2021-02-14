
# include "TowerData.hh"
# include "Tower.hh"
# include "StepInfo.hh"
# include "Locator.hh"

namespace tdef {
  namespace towers {

    std::vector<MobShPtr>
    basicTargetPicking(StepInfo& info, PickData& data) {
      MobShPtr m = info.frustum->getClosestMob(data.pos, data.maxRange, nullptr);

      if (m == nullptr) {
        return std::vector<MobShPtr>();
      }

      return std::vector<MobShPtr>{m};
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

    towers::Upgradable
    buildLinearUpgradable(float a,
                          float b,
                          float min) noexcept
    {
      return [a, b, min](int level) {
        return level == 0 ? min : a * level + b;
      };
    }

    towers::Upgradable
    buildQuadraticUpgradable(float a,
                             float b,
                             float c,
                             float min) noexcept
    {
      return [a, b, c, min](int level) {
        return level == 0 ? min : a * level * level + b * level + c;
      };
    }

    towers::Upgradable
    buildCubicUpgradable(float a,
                         float b,
                         float c,
                         float d,
                         float min) noexcept
    {
      return [a, b, c, d, min](int level) {
        return level == 0 ? min : a * level * level * level + b * level * level + c * level + d;
      };
    }

  }
}
