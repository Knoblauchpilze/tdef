
# include "SpawnerData.hh"

namespace tdef {
  namespace spawners {

    /**
     * @brief - Defines the ratio to apply to the canonical
     *          health for a mob to obtain the minimum pool
     *          for a given level.
     */
    constexpr float min_health_pool = 0.95f;

    /**
     * @brief - Defines the ratio to apply to the canonical
     *          health for a mob to obtain the maximum pool
     *          for a given level.
     */
    constexpr float max_health_pool = 1.05f;

    /**
     * @brief - Defines the minimum size of a wave.
     */
    constexpr int min_wave_size = 1;

    /**
     * @brief - Defines the maximum size of a wave.
     */
    constexpr int max_wave_size = 1;

    namespace easy {

      constexpr float constant = 0.f;

      Processes
      generateData() noexcept {
        Processes dd;

        dd.wave = [](StepInfo& info, int exp) {
          // The minimum and maximum wave size scales
          // linearily with the experience.
          float s = std::round(constant * exp);

          int min = min_wave_size + static_cast<int>(s);
          int max = max_wave_size + static_cast<int>(s);

          return info.rng.rndInt(min, max);
        };

        dd.health = [](StepInfo& info, int exp) {
          // The minimum and maximum mob health scales
          // linearily with the experience.
          float s = 0.5f * exp;

          float hMin = std::max(min_health_pool * s, 1.0f);
          float hMax = std::max(max_health_pool * s, 1.0f);

          return info.rng.rndFloat(hMin, hMax);
        };

        return dd;
      }

      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept {
        spawners::Distribution d = {
          spawners::DistItem{0.2f, mobs::Type::Regular},
          spawners::DistItem{0.4f, mobs::Type::Fast},
          spawners::DistItem{0.2f, mobs::Type::Icy},
          spawners::DistItem{0.2f, mobs::Type::Strong}
        };

        Spawner::SProps pp = Spawner::newProps(p, d);

        pp.spawnRadius = 2.0f;

        pp.threshold = 1.0f;
        pp.reserve = 0.9f;
        pp.refill = 0.05f;

        return pp;
      }

    }

    namespace normal {

      constexpr float constant = 0.f;

      Processes
      generateData() noexcept {
        Processes dd;

        dd.wave = [](StepInfo& info, int exp) {
          // The minimum and maximum wave size scales
          // quadratically with the experience.
          float s = std::round(constant * exp * exp);

          int min = min_wave_size + static_cast<int>(s);
          int max = max_wave_size + static_cast<int>(s);

          return info.rng.rndInt(min, max);
        };

        dd.health = [](StepInfo& info, int exp) {
          // The minimum and maximum mob health scales
          // linearily with the experience.
          float s = 0.5f * exp * exp;

          float hMin = std::max(min_health_pool * s, 1.0f);
          float hMax = std::max(max_health_pool * s, 1.0f);

          return info.rng.rndFloat(hMin, hMax);
        };

        return dd;
      }

      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept {
        spawners::Distribution d = {
          spawners::DistItem{0.2f, mobs::Type::Regular},
          spawners::DistItem{0.4f, mobs::Type::Fast},
          spawners::DistItem{0.2f, mobs::Type::Icy},
          spawners::DistItem{0.2f, mobs::Type::Strong}
        };

        Spawner::SProps pp = Spawner::newProps(p, d);

        pp.spawnRadius = 2.0f;

        pp.threshold = 1.0f;
        pp.reserve = 0.9f;
        pp.refill = 0.1f;

        return pp;
      }

    }

    namespace hard {

      constexpr float constant = 0.f;

      Processes
      generateData() noexcept {
        Processes dd;

        dd.wave = [](StepInfo& info, int exp) {
          // The minimum and maximum wave size scales
          // cubically with the experience.
          float s = std::round(constant * exp * exp * exp);

          int min = min_wave_size + static_cast<int>(s);
          int max = max_wave_size + static_cast<int>(s);

          return info.rng.rndInt(min, max);
        };

        dd.health = [](StepInfo& info, int exp) {
          // The minimum and maximum mob health scales
          // cubically with the experience.
          float s = 0.5f * exp * exp * exp;

          float hMin = std::max(min_health_pool * s, 1.0f);
          float hMax = std::max(max_health_pool * s, 1.0f);

          return info.rng.rndFloat(hMin, hMax);
        };

        return dd;
      }

      Spawner::SProps
      generateProps(const utils::Point2f& p) noexcept {
        spawners::Distribution d = {
          spawners::DistItem{0.2f, mobs::Type::Regular},
          spawners::DistItem{0.4f, mobs::Type::Fast},
          spawners::DistItem{0.2f, mobs::Type::Icy},
          spawners::DistItem{0.2f, mobs::Type::Strong}
        };

        Spawner::SProps pp = Spawner::newProps(p, d);

        pp.spawnRadius = 2.0f;

        pp.threshold = 1.0f;
        pp.reserve = 0.9f;
        pp.refill = 0.2f;

        return pp;
      }

    }

  }
}
