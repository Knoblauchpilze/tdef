#ifndef    APP_DESC_HH
# define   APP_DESC_HH

# include <string>
# include "olcEngine.hh"
# include "coordinates/CoordinateFrame.hh"

namespace tdef {

  /**
   * @brief - Convenience description of settings to define
   *          the canvas used by an app. This allows to be
   *          concise and easily add info if needed.
   */
  struct AppDesc {
    olc::vi2d dims;
    olc::vi2d pixRatio;

    std::string name;

    CoordinateFrameShPtr frame;
  };

  /**
   * @brief - Creates a default app description.
   * @param dims - the dimensions of the application's main
   *               canvas in pixels.
   * @param cf - the coordinate frame to associate to the
   *             application.
   * @param name - the name of the application.
   * @return - the default description of an app.
   */
  AppDesc
  newDesc(const olc::vi2d& dims,
          CoordinateFrameShPtr cf,
          const std::string& name = "pge_app") noexcept;

}

# include "AppDesc.hxx"

#endif    /* APP_DESC_HH */
