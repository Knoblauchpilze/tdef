
# include "TowerFactory.hh"
# include "Locator.hh"

namespace tdef {

  Tower::TProps
  TowerFactory::generateBasicTowerProps(const utils::Point2f& p) noexcept {
    Tower::TProps pp = Tower::newProps(p, towers::Type::Regular);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.1f;

    pp.range = 3.0f;
    pp.attack = 0.2f;
    pp.attackCost = 0.55f;

    return pp;
  }

  Tower::TProps
  TowerFactory::generateSniperTowerProps(const utils::Point2f& p) noexcept {
    Tower::TProps pp = Tower::newProps(p, towers::Type::Snipe);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.05f;

    pp.range = 4.0f;
    pp.attack = 0.4f;
    pp.attackCost = 0.55f;

    return pp;
  }

  Tower::TProps
  TowerFactory::generateSlowTowerProps(const utils::Point2f& p) noexcept {
    Tower::TProps pp = Tower::newProps(p, towers::Type::Slow);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.01f;

    pp.range = 3.2f;
    pp.attack = 0.01f;
    pp.attackCost = 0.5f;

    return pp;
  }

  Tower::TProps
  TowerFactory::generateCannonTowerProps(const utils::Point2f& p) noexcept {
    Tower::TProps pp = Tower::newProps(p, towers::Type::Cannon);

    pp.energy = 0.5f;
    pp.maxEnergy = 1.0f;
    pp.refill = 0.1f;

    pp.range = 2.5f;
    pp.attack = 0.25f;
    pp.attackCost = 0.55f;

    return pp;
  }

  towers::Data
  TowerFactory::generateBasicTowerData() noexcept {
    towers::Data dd;

    dd.pickMob = [](StepInfo& info, towers::PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.range, nullptr);
    };

    dd.damage = [](StepInfo& /*info*/, MobShPtr mob, towers::DamageData& data) {
      return mob->damage(data.attack);
    };

    return dd;
  }

  towers::Data
  TowerFactory::generateSniperTowerData() noexcept {
    towers::Data dd;

    dd.pickMob = [](StepInfo& info, towers::PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.range, nullptr);
    };

    dd.damage = [](StepInfo& /*info*/, MobShPtr mob, towers::DamageData& data) {
      return mob->damage(data.attack);
    };

    return dd;
  }

  towers::Data
  TowerFactory::generateSlowTowerData() noexcept {
    towers::Data dd;

    dd.pickMob = [](StepInfo& info, towers::PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.range, nullptr);
    };

    dd.damage = [](StepInfo& /*info*/, MobShPtr mob, towers::DamageData& data) {
      return mob->damage(data.attack);
    };

    return dd;
  }

  towers::Data
  TowerFactory::generateCannonTowerData() noexcept {
    towers::Data dd;

    dd.pickMob = [](StepInfo& info, towers::PickData& data) {
      return info.frustum->getClosestMob(data.pos, data.range, nullptr);
    };

    dd.damage = [](StepInfo& /*info*/, MobShPtr mob, towers::DamageData& data) {
      return mob->damage(data.attack);
    };

    return dd;
  }

}
