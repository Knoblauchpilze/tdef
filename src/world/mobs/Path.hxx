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
    Segment::normalize() {
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

  }

  inline
  bool
  Path::valid() const noexcept {
    return !m_segments.empty();
  }

  inline
  void
  Path::clear(const utils::Point2f& p) {
    // Assign home and current position.
    m_home = p;
    m_cur = p;

    // Reset segments.
    m_seg = -1;
    m_segments.clear();

    // Reset temporary passage points.
    m_cPoints.clear();
    addPassagePoint(p);
  }

  inline
  void
  Path::addPassagePoint(const utils::Point2f& p) {
    m_cPoints.push_back(p);
  }

  inline
  void
  Path::add(const utils::Point2f& p, float xD, float yD, float d) {
    path::Segment s = path::newSegment(p, xD, yD, d);
    m_segments.push_back(s);
    addPassagePoint(s.end);

    // Make the entity on the first segment.
    if (m_seg < 0) {
      m_seg = 0;
    }
  }

  inline
  void
  Path::add(const utils::Point2f& s, const utils::Point2f& t) {
    path::Segment se = path::newSegment(s, t);
    m_segments.push_back(se);
    addPassagePoint(se.end);

    // Make the entity on the first segment.
    if (m_seg < 0) {
      m_seg = 0;
    }
  }

  inline
  void
  Path::add(const utils::Point2f& p) {
    if (m_seg < 0) {
      // We want to make sure that we don't
      // register the home position once
      // again.
      if (m_home.x() != p.x() || m_home.y() != p.y()) {
        add(m_home, p);
      }
    }
    else {
      const path::Segment s = m_segments.back();
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
    int ss = static_cast<int>(m_segments.size());
    if (m_seg < 0 || m_seg >= ss) {
      return false;
    }

    return (m_seg < ss - 1) || utils::d(m_segments[m_seg].end, m_cur) > threshold;
  }

  inline
  utils::Point2f
  Path::cur() const noexcept {
    return m_cur;
  }

  inline
  utils::Point2f
  Path::currentTarget() const noexcept {
    int ss = static_cast<int>(m_segments.size());
    if (m_seg < 0 || m_seg >= ss) {
      return utils::Point2f();
    }

    return m_segments[m_seg].end;
  }

  inline
  utils::Point2f
  Path::target() const noexcept {
    if (m_segments.empty()) {
      return utils::Point2f();
    }

    return m_segments.back().end;
  }

  inline
  const std::vector<utils::Point2f>&
  Path::getPassagePoints() const noexcept {
    return m_cPoints;
  }

}

inline
std::ostream&
operator<<(std::ostream& out, const tdef::Path& p) noexcept {
  p << out;
  return out;
}

inline
std::istream&
operator>>(std::istream& in, tdef::Path& p) noexcept {
  p >> in;
  return in;
}

#endif    /* PATH_HXX */
