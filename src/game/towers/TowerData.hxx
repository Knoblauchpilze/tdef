#ifndef    TOWER_DATA_HXX
# define   TOWER_DATA_HXX

# include "TowerData.hh"

namespace tdef {
  namespace towers {

    inline
    constexpr float
    freezePercentageToSpeedRatio(float freezePercentage) noexcept {
      return (100.0f - freezePercentage) / 100.0f;
    }

  }
}

#endif    /* TOWER_DATA_HH */
