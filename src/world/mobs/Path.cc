
# include "Path.hh"
# include "StepInfo.hh"
# include "Locator.hh"
# include "AStar.hh"

namespace tdef {

  Path::Path() noexcept:
    utils::CoreObject("path"),

    m_home(),
    m_cur(),

    m_seg(-1),
    m_segments(),
    m_cPoints()
  {
    setService("path");
  }

  Path::Path(const utils::Point2f& p) noexcept:
    utils::CoreObject("path"),

    m_home(p),
    m_cur(p),

    m_seg(-1),
    m_segments(),
    m_cPoints()
  {
    setService("path");

    // Register the home position as a passage point.
    addPassagePoint(p);
  }

  void
  Path::advance(float speed, float elapsed, float threshold) {
    // In case we already arrived, do nothing.
    if (!enRoute(threshold)) {
      return;
    }

    float traveled = speed * elapsed;
    int ss = static_cast<int>(m_segments.size());

    // Attempt to complete the current path
    // segment until we either reach the end
    // of the path or don't travel enough to
    // do so.
    float dToEofS = utils::d(m_cur, m_segments[m_seg].end);

    while (traveled > dToEofS && m_seg < ss) {
      // Move along the path segment: as we
      // traveled far enough to complete the
      // path segment we can directly go to
      // the target point of the segment.
      m_cur.x() = m_segments[m_seg].end.x();
      m_cur.y() = m_segments[m_seg].end.y();

      // Finish this path segment and move to
      // the next one.
      traveled -= dToEofS;
      ++m_seg;

      // In order to prepare for the advance of
      // the segment, we will move to the start
      // of the next segment (so the one pointed
      // by `seg` now).
      // This should usually correspond to the
      // end of the previous segment but as it
      // is not guaranteed, we will ensure it.
      if (m_seg < ss) {
        m_cur.x() = m_segments[m_seg].start.x();
        m_cur.y() = m_segments[m_seg].start.y();
      }

      // Initialize the distance to the end
      // of the path segment.
      dToEofS = utils::d(m_cur, m_segments[m_seg].end);
    }

    // See whether we reached the end of the
    // path: if this is the case, the previous
    // algo ensured we were already at the end
    // of the path so we don't have nothing
    // else to do.
    if (m_seg == ss) {
      return;
    }

    // Advance on the path of the amount left.
    m_cur.x() += traveled * m_segments[m_seg].xD;
    m_cur.y() += traveled * m_segments[m_seg].yD;
  }

  bool
  Path::generatePathTo(LocatorShPtr frustum,
                       const utils::Point2f& p,
                       bool ignoreTargetObstruction,
                       float maxDistanceFromStart,
                       bool allowLog)
  {
    // Convert to path semantic: the starting point
    // of the path segment will be the end of the
    // registered list of intermediate points. It
    // can correspond to the home position in case
    // no segments are defined.
    utils::Point2f s = m_home;
    if (m_seg >= 0) {
      s = m_segments[m_segments.size() - 1].end;
    }

    float xDir, yDir, d;
    utils::toDirection(s, p, xDir, yDir, d);

    // Detect trivial case where the target is a
    // solid block and we're not supposed to be
    // ignoring it: in this case it does not make
    // sense to try to generate a path.
    if (frustum->obstructed(p) && !ignoreTargetObstruction) {
      return false;
    }

    // First, try to find a straight path to the
    // target: if this is possible it's cool.
    utils::Point2f obsP;
    std::vector<utils::Point2f> iPoints;

    bool obs = frustum->obstructed(s, xDir, yDir, d, iPoints, &obsP);
    float dx = std::abs(obsP.x() - p.x());
    float dy = std::abs(obsP.y() - p.y());
    bool obsWithinTarget = obs && (dx < 1.0f && dy < 1.0f);

    if (!obs || (obsWithinTarget && ignoreTargetObstruction)) {
      // Either there is no obstruction or there is one
      // but we ignore it. In case we could generate at
      // least one intermediate point between the start
      // and the end of the path, we need to register
      // it.
      if (!iPoints.empty()) {
        for (unsigned id = 0u ; id < iPoints.size() - 1 ; ++id) {
          m_cPoints.push_back(iPoints[id]);
        }
      }

      add(p);

      return true;
    }

    // Note that at this point we know:
    //  - that the target is not obstructed or we
    //    ignore it.
    //  - there is no straight path to the target.
    // So with this information in mind, we will
    // try to run a A* algorithm. We thought about
    // relying on a technique called the `obstacle
    // tracing` algorithm but it appears that it's
    // not really easy to implement and we prefer
    // to choose safety. It is not exactly what we
    // desired (the A*) as it means that each mob
    // has indeed infinite vision for now but
    // that's it.
    AStar alg(s, p, frustum);
    std::vector<utils::Point2f> steps;

    if (!alg.findPath(steps, maxDistanceFromStart, allowLog)) {
      return false;
    }

    // A path was found, register it.
    for (unsigned id = 0u ; id < steps.size() ; ++id) {
      add(steps[id]);
    }

    return true;
  }

}
