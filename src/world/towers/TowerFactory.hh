#ifndef    TOWER_FACTORY_HH
# define   TOWER_FACTORY_HH

# include <core_utils/Uuid.hh>
# include "Tower.hh"

namespace tdef {

  class TowerFactory {
    public:

      static
      Tower::TProps
      generateBasicTowerProps(const utils::Point2f& p) noexcept;

      static
      Tower::TProps
      generateSnipeTowerProps(const utils::Point2f& p) noexcept;

      static
      Tower::TProps
      generateSlowTowerProps(const utils::Point2f& p) noexcept;

      static
      Tower::TProps
      generateCannonTowerProps(const utils::Point2f& p) noexcept;

      static
      towers::Data
      generateBasicTowerData() noexcept;

      static
      towers::Data
      generateSnipeTowerData() noexcept;

      static
      towers::Data
      generateSlowTowerData() noexcept;

      static
      towers::Data
      generateCannonTowerData() noexcept;

    private:
  };

}

#endif    /* TOWER_FACTORY_HH */
