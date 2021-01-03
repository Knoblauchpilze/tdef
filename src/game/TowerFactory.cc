
# include "TowerFactory.hh"
# include "TowerData.hh"

namespace tdef {
  namespace towers {

    Data
    generateData(const Type& type, int level) {
      switch (type) {
        case Type::Sniper:
          return sniper::generateData(level);
        case Type::Cannon:
          return cannon::generateData(level);
        case Type::Freezing:
          return freezing::generateData(level);
        case Type::Venom:
          return venom::generateData(level);
        case Type::Splash:
          return splash::generateData(level);
        case Type::Blast:
          return blast::generateData(level);
        case Type::Multishot:
          return multishot::generateData(level);
        case Type::Minigun:
          return minigun::generateData(level);
        case Type::Antiair:
          return antiair::generateData(level);
        case Type::Tesla:
          return tesla::generateData(level);
        case Type::Missile:
          return missile::generateData(level);
        case Type::Basic:
        default:
          // Assume default is basic tower.
          return basic::generateData(level);
      }
    }

    Tower::TProps
    generateProps(const Type& type,
                  const utils::Point2f& p,
                  int level)
    {
      switch (type) {
        case Type::Sniper:
          return sniper::generateProps(p, level);
        case Type::Cannon:
          return cannon::generateProps(p, level);
        case Type::Freezing:
          return freezing::generateProps(p, level);
        case Type::Venom:
          return venom::generateProps(p, level);
        case Type::Splash:
          return splash::generateProps(p, level);
        case Type::Blast:
          return blast::generateProps(p, level);
        case Type::Multishot:
          return multishot::generateProps(p, level);
        case Type::Minigun:
          return minigun::generateProps(p, level);
        case Type::Antiair:
          return antiair::generateProps(p, level);
        case Type::Tesla:
          return tesla::generateProps(p, level);
        case Type::Missile:
          return missile::generateProps(p, level);
        case Type::Basic:
        default:
          // Assume default is basic tower.
          return basic::generateProps(p, level);
      }
    }

  }
}
