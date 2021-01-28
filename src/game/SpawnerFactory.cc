
# include "SpawnerFactory.hh"
# include "SpawnerData.hh"

namespace tdef {
  namespace spawners {

    Processes
    generateData(const Level& lvl) {
      switch (lvl) {
        case Level::Easy:
          return easy::generateData();
        case Level::Hard:
          return hard::generateData();
        case Level::Normal:
        default:
          // Assume default is normal difficulty.
          return normal::generateData();
      }
    }

    Spawner::SProps
    generateProps(const utils::Point2f& p,
                  const Level& lvl)
    {
      switch (lvl) {
        case Level::Easy:
          return easy::generateProps(p);
        case Level::Hard:
          return hard::generateProps(p);
        case Level::Normal:
        default:
          // Assume default is normal difficulty.
          return normal::generateProps(p);
      }
    }

  }
}