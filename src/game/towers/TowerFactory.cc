
# include "TowerFactory.hh"
# include "Basic.hh"
# include "Sniper.hh"
# include "Cannon.hh"
# include "Freezing.hh"
# include "Venom.hh"
# include "Splash.hh"
# include "Blast.hh"
# include "Multishot.hh"
# include "Minigun.hh"
# include "Antiair.hh"
# include "Tesla.hh"
# include "Missile.hh"

namespace tdef {
  namespace towers {

    Processes
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

    float
    getCost(const Type& type,
            int level)
    {
      switch (type) {
        case Type::Sniper:
          return sniper::getCost(level);
        case Type::Cannon:
          return cannon::getCost(level);
        case Type::Freezing:
          return freezing::getCost(level);
        case Type::Venom:
          return venom::getCost(level);
        case Type::Splash:
          return splash::getCost(level);
        case Type::Blast:
          return blast::getCost(level);
        case Type::Multishot:
          return multishot::getCost(level);
        case Type::Minigun:
          return minigun::getCost(level);
        case Type::Antiair:
          return antiair::getCost(level);
        case Type::Tesla:
          return tesla::getCost(level);
        case Type::Missile:
          return missile::getCost(level);
        case Type::Basic:
        default:
          // Assume default is basic tower.
          return basic::getCost(level);
      }
    }

    float
    getUpgradeCost(const Type& type,
                   const Upgrade& upgrade,
                   int level)
    {
      switch (type) {
        case Type::Sniper:
          return sniper::getUpgradeCost(upgrade, level);
        case Type::Cannon:
          return cannon::getUpgradeCost(upgrade, level);
        case Type::Freezing:
          return freezing::getUpgradeCost(upgrade, level);
        case Type::Venom:
          return venom::getUpgradeCost(upgrade, level);
        case Type::Splash:
          return splash::getUpgradeCost(upgrade, level);
        case Type::Blast:
          return blast::getUpgradeCost(upgrade, level);
        case Type::Multishot:
          return multishot::getUpgradeCost(upgrade, level);
        case Type::Minigun:
          return minigun::getUpgradeCost(upgrade, level);
        case Type::Antiair:
          return antiair::getUpgradeCost(upgrade, level);
        case Type::Tesla:
          return tesla::getUpgradeCost(upgrade, level);
        case Type::Missile:
          return missile::getUpgradeCost(upgrade, level);
        case Type::Basic:
        default:
          // Assume default is basic tower.
          return basic::getUpgradeCost(upgrade, level);
      }
    }

  }
}
