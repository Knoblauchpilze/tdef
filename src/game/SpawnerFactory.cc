
# include "SpawnerFactory.hh"
# include "Mob.hh"

namespace tdef {
  namespace spawners {

    Spawner::SProps
    generateProps(const utils::Point2f& p) {
      spawners::Distribution d;

      d.push_back(
        spawners::DistItem{
          0.2f,
          mobs::Type::Regular
        }
      );
      d.push_back(
        spawners::DistItem{
          0.4f,
          mobs::Type::Fast
        }
      );
      d.push_back(
        spawners::DistItem{
          0.2f,
          mobs::Type::Icy
        }
      );
      d.push_back(
        spawners::DistItem{
          0.2f,
          mobs::Type::Strong
        }
      );

      Spawner::SProps pp = Spawner::newProps(p, d);

      pp.spawnRadius = 2.0f;

      pp.threshold = 1.0f;
      pp.reserve = 0.9f;
      pp.refill = 0.1f;

      pp.minWaveSize = 1;
      pp.maxWaveSize = 3;

      return pp;
    }

  }
}