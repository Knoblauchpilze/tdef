
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
// TODO: Projectiles seem to keep being alive for some time.
// TODO: Somehow materialize the aiming speed.

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
      tdef::Viewport{olc::vf2d(0.0f, 0.0f), olc::vf2d(10.0f, 10.0f)},
      tdef::Viewport{olc::vf2d(10.0f, 50.0f), olc::vf2d(640.0f, 480.0f)},
      olc::vi2d(64, 64)
    );
    tdef::AppDesc ad = tdef::newDesc(
      olc::vi2d(640, 480),
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
