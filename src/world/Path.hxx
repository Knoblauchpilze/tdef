#ifndef    PATH_HXX
# define   PATH_HXX

# include "Path.hh"
# include <maths_utils/LocationUtils.hh>

namespace tdef {
  namespace path {

    inline
    float
    Segment::length() const noexcept {
      return std::sqrt(
        (end.x() - start.x()) * (end.x() - start.x()) +
        (end.y() - start.y()) * (end.y() - start.y())
      );
    }

    inline
    void
    Segment::normalize(const StepInfo& /*info*/) {
      // Update direction for this segment.
      xD = end.x() - start.x();
      yD = end.y() - start.y();

      float d = length();
      if (d > 0.0001f) {
        xD /= d;
        yD /= d;
      }
    }

    inline
    bool
    Path::valid() const noexcept {
      return !segments.empty();
    }

    inline
    void
    Path::clear(const utils::Point2f& p, bool force) {
      // Assign home and current position.
      home = p;
      cur = p;

      // Reset segments.
      seg = -1;
      segments.clear();

      // Reset temporary passage points.
      cPoints.clear();
      addPassagePoint(p);

      forced = force;
    }

    inline
    void
    Path::addPassagePoint(const utils::Point2f& p) {
      cPoints.push_back(p);
    }

    inline
    void
    Path::add(const utils::Point2f& p, float xD, float yD, float d) {
      Segment s = newSegment(p, xD, yD, d);
      segments.push_back(s);
      addPassagePoint(s.end);

      // Make the entity on the first segment.
      if (seg < 0) {
        seg = 0;
      }
    }

    inline
    void
    Path::add(const utils::Point2f& s, const utils::Point2f& t) {
      Segment se = newSegment(s, t);
      segments.push_back(se);
      addPassagePoint(se.end);

      // Make the entity on the first segment.
      if (seg < 0) {
        seg = 0;
      }
    }

    inline
    void
    Path::add(const utils::Point2f& p) {
      if (seg < 0) {
        // We want to make sure that we don't
        // register the home position once
        // again.
        if (home.x() != p.x() || home.y() != p.y()) {
          add(home, p);
        }
      }
      else {
        const Segment s = segments.back();
        add(s.end, p);
      }
    }

    inline
    bool
    Path::enRoute(float threshold) const noexcept {
      // In case the identifier does not describe a
      // valid path segment, assume we did arrive.
      // Similarly if no segments are registered we
      // consider that we already arrived.
      int ss = static_cast<int>(segments.size());
      if (seg < 0 || seg >= ss) {
        return false;
      }

      return (seg < ss - 1) || utils::d(segments[seg].end, cur) > threshold;
    }

    inline
    utils::Point2f
    Path::currentTarget() const noexcept {
      int ss = static_cast<int>(segments.size());
      if (seg < 0 || seg >= ss) {
        return utils::Point2f();
      }

      return segments[seg].end;
    }

    inline
    Segment
    newSegment(const utils::Point2f& p, float xD, float yD, float d) noexcept {
      Segment s;

      s.start = p;
      s.xD = xD;
      s.yD = yD;

      s.end.x() = s.start.x() + d * s.xD;
      s.end.y() = s.start.y() + d * s.yD;

      return s;
    }

    inline
    Segment
    newSegment(const utils::Point2f& s, const utils::Point2f& t) noexcept {
      Segment se;

      se.start = s;
      se.end = t;

      se.xD = se.end.x() - se.start.x();
      se.yD = se.end.y() - se.start.y();

      float d = se.length();
      if (d > 0.0001f) {
        se.xD /= d;
        se.yD /= d;
      }

      return se;
    }

    inline
    Path
    newPath(const utils::Point2f& p) noexcept {
      Path pa;

      pa.home = p;
      pa.cur = p;

      pa.seg = -1;
      pa.addPassagePoint(p);

      pa.forced = false;

      return pa;
    }

  }
}

#endif    /* PATH_HXX */
