#ifndef    PATH_HH
# define   PATH_HH

# include <vector>
# include <memory>
# include <maths_utils/Point2.hh>

namespace tdef {
  // Forward declaration of the `Locator` class.
  class Locator;
  using LocatorShPtr = std::shared_ptr<Locator>;

  namespace path {

    /**
     * @brief - Convenience structure defining a path segment with
     *          its endpoints (a starting point and a target) and
     *          a direction.
     */
    struct Segment {
      utils::Point2f start;
      utils::Point2f end;

      float xD, yD;

      /**
       * @brief - Computes the length of this path in cells.
       * @return - the length of the path in cells.
       */
      float
      length() const noexcept;

      /**
       * @brief - Used to make sure that the starting and end
       *          postion of this segment are consistent with
       *          the dimensions of the world.
       */
      void
      normalize();
    };

    /**
     * @brief - Define a complete path, composed of one or
     *          more segments.
     */
    struct Path {
      utils::Point2f home;

      utils::Point2f cur;
      int seg;

      std::vector<Segment> segments;

      std::vector<utils::Point2f> cPoints;

      /**
       * @brief - Used to determine whether this path is
       *          valid. We consider a path valid if it
       *          is composed of at least one segment.
       * @return - `true` if this path is valid.
       */
      bool
      valid() const noexcept;

      /**
       * @brief - Reset all information defined in this path
       *          and assume the entity is starting from the
       *          input position.
       * @param p - the position with which to clear the path.
       */
      void
      clear(const utils::Point2f& p);

      /**
       * @brief - Add the specified passage point in the list
       *          without changing it otherwise.
       * @param p - the passage point to add to the path.
       */
      void
      addPassagePoint(const utils::Point2f& p);

      /**
       * @brief - Add a new segment to this path with the
       *          specified information.
       * @param p - the starting point of the path segment.
       * @param xD - the abscissa of the direction of the
       *             new segment.
       * @param yD - the ordinate of the direction of the
       *             new segment.
       * @param d - the length of the path segment.
       */
      void
      add(const utils::Point2f& p, float xD, float yD, float d);

      /**
       * @brief - Other variant to register a new segment
       *          on this path.
       * @param s - the starting point of the path segment.
       * @param t - the target of the path segment.
       */
      void
      add(const utils::Point2f& s, const utils::Point2f& t);

      /**
       * @brief - Add a new path segment starting from the
       *          last position reached by the path and with
       *          the corresponding end position.
       *          In case no previous path segment exists
       *          the starting position of the path is used.
       * @param p - the target point of the path segment to
       *            create.
       */
      void
      add(const utils::Point2f& p);

      /**
       * @brief - Determine whether the current position on
       *          the path means that we arrived or not. It
       *          is only precise if the user actually makes
       *          the path-follower advance on the path by
       *          calling `advance`.
       * @param threshold - define the threshold below which
       *                    the path-follower is considered
       *                    to have arrived.
       * @return - `true` in case the path-follower has not
       *           arrived yet.
       */
      bool
      enRoute(float threshold) const noexcept;

      /**
       * @brief - Used to retrieve the closest target on the
       *          path: this refers to the endpoint of the
       *          segment currently followed by the entity.
       * @return - the next target on the path.
       */
      utils::Point2f
      currentTarget() const noexcept;

      /**
       * @brief - Used to fetch the final target of the path
       *          after all segments will have been traversed.
       * @return - the target of the path.
       */
      utils::Point2f
      target() const noexcept;

      /**
       * @brief - Used to advance on this path assuming the
       *          path follower is moving at `speed` along
       *          the path and that `elapsed` seconds have
       *          passed since the last actualization.
       * @param speed - the speef of the entity following
       *                the path.
       * @param elapsed - the duration elapsed since the
       *                  last actualization in seconds.
       * @param threshold - a threshold to consider that a
       *                    path segment is finished.
       */
      void
      advance(float speed, float elapsed, float threshold);

      /**
       * @brief - Used to generate a path to the target specified
       *          by the `x` and `y` coordinates and add needed
       *          segments to the path. The start of the path is
       *          assumed to be the current end of the path.
       * @param frustum - allowing to detect obstruction to reach
       *                  the target.
       * @param p - the point to generate a path to.
       * @param ignoreTargetObstruction - set this value to `true`
       *                                  in case obstruction of
       *                                  the target should be
       *                                  ignored. This should be
       *                                  the case for example when
       *                                  the target is a solid
       *                                  block.
       * @param maxDistanceFromStart - the maximum distance to which
       *                               the path can wander from the
       *                               starting point. Any path that
       *                               goes farther than this value
       *                               will be considered invalid.
       * @param allowLog - `true` if the A* process should produce
       *                   logs and information.
       * @return - `true` if we could generate a path to the
       *           target and `false` otherwise.
       */
      bool
      generatePathTo(LocatorShPtr frustum,
                     const utils::Point2f& p,
                     bool ignoreTargetObstruction,
                     float maxDistanceFromStart = -1.0f,
                     bool allowLog = false);
    };

    /**
     * @brief - Create a new path segment starting at the
     *          specified location and with the direction
     *          provided in input.
     *          The final position of the segment is given
     *          by measuring `d` units along the direction.
     * @param p - the starting position of the path segment.
     * @param xD - the abscissa for the direction.
     * @param yD - the ordinate for the direction.
     * @param d - the length of this path segment.
     * @return - the created path segment.
     */
    Segment
    newSegment(const utils::Point2f& p, float xD, float yD, float d) noexcept;

    /**
     * @brief - Create a new segment from a starting location
     *          and an end position.
       * @param s - the starting point of the path segment.
       * @param t - the target of the path segment.
     * @return - the created path segment.
     */
    Segment
    newSegment(const utils::Point2f& s, const utils::Point2f& t) noexcept;

    /**
     * @brief - Create a new path with the input position
     *          as default.
     * @param p - the input position of the path.
     * @return - the created path.
     */
    Path
    newPath(const utils::Point2f& p) noexcept;

  }
}

# include "Path.hxx"

#endif    /* PATH_HH */
