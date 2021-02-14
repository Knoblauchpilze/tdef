
/**
 * @brief - Attempt at a real *playable* tower defense
 *          game which aims at actually finishing the
 *          features and deliver a working product.
 *          Started on 13/12/2020.
 */

# include <core_utils/StdLogger.hh>
# include <core_utils/LoggerLocator.hh>
# include <core_utils/CoreException.hh>
# include "AppDesc.hh"
# include "coordinates/TopViewFrame.hh"
# include "TDefApp.hh"

// TODO: Some mobs seem to have really high speed.
// TODO: Custom behaviors for towers.
// TODO: Maybe in case of A* due a single path with
// the target as the starting point so that we can
// explore all the starting position (i.e. for mobs)
// at once and assign the built paths while we explore
// nodes.
// TODO: Handle application of AttackSpeed.
// TODO: Handle creation of non-constant upgradable
// for tower props:
//  - X pp.minRange
//  - V pp.maxRange
//  - X pp.damage
//  - X pp.aoeRadius
//  - X pp.rotationSpeed
//  - X pp.aimSpeed
//  - V pp.projectileSpeed
//  - X pp.accuracy
//  - X pp.duration
//  - X pp.shootAngle
//  - X pp.projectiles
//  - X pp.acceleration
//  - X pp.freezePercent
//  - X pp.freezeSpeed
//  - X pp.stunProb
//  - X pp.critProb
//  - X pp.critMultiplier

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::StdLogger logger;
  utils::LoggerLocator::provide(&logger);

  logger.setLevel(utils::Level::Debug);

  const std::string service("tdef");
  const std::string module("main");

  try {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Notice,
      std::string("Starting application"),
      module,
      service
    );

    tdef::CoordinateFrameShPtr cf = std::make_shared<tdef::TopViewFrame>(
      tdef::Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(20.0f, 15.0f)},
      tdef::Viewport{olc::vf2d(10.0f, 50.0f), olc::vf2d(800.0f, 600.0f)},
      olc::vi2d(64, 64)
    );
    tdef::AppDesc ad = tdef::newDesc(
      olc::vi2d(800, 600),
      cf,
      "tower defense"
    );
    tdef::TDefApp demo(ad);

    demo.Start();
  }
  catch (const utils::CoreException& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught internal exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (const std::exception& e) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Caught exception while setting up application"),
      module,
      service,
      e.what()
    );
  }
  catch (...) {
    utils::LoggerLocator::getLogger().logMessage(
      utils::Level::Critical,
      std::string("Unexpected error while setting up application"),
      module,
      service
    );
  }

  // All is good.
  return EXIT_SUCCESS;
}
