
/**
 * @brief - Attempt at a real *playable* tower defense
 *          game which aims at actually finishing the
 *          features and deliver a working product.
 *          Started on 13/12/2020.
 *          Finished on 20/02/2021.
 */

# include <core_utils/log/StdLogger.hh>
# include <core_utils/log/Locator.hh>
# include <core_utils/log/PrefixedLogger.hh>
# include <core_utils/CoreException.hh>
# include "AppDesc.hh"
# include "coordinates/TopViewFrame.hh"
# include "TDefApp.hh"

// TODO: Maybe in case of A* due a single path with
// the target as the starting point so that we can
// explore all the starting position (i.e. for mobs)
// at once and assign the built paths while we explore
// nodes.

int main(int /*argc*/, char** /*argv*/) {
  // Create the logger.
  utils::log::StdLogger raw;
  raw.setLevel(utils::log::Severity::DEBUG);
  utils::log::PrefixedLogger logger("tdef", "main");
  utils::log::Locator::provide(&raw);

  try {
    logger.notice("Starting application");

    tdef::CoordinateFrameShPtr cf = std::make_shared<tdef::TopViewFrame>(
      tdef::Viewport{olc::vf2d(-6.0f, -5.0f), olc::vf2d(20.0f, 15.0f)},
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
    logger.error("Caught internal exception while setting up application", e.what());
  }
  catch (const std::exception& e) {
    logger.error("Caught exception while setting up application", e.what());
  }
  catch (...) {
    logger.error("Unexpected error while setting up application");
  }

  // All is good.
  return EXIT_SUCCESS;
}
