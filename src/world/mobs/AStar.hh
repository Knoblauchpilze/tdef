#ifndef    ASTAR_HH
# define   ASTAR_HH

# include <core_utils/CoreObject.hh>
# include <unordered_map>
# include <maths_utils/Point2.hh>
# include "Locator.hh"

namespace tdef {

  class AStar: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new A* capable object allowing to
       *          go from the starting point `s` to the end
       *          point `e` using the world described by the
       *          locator.
       * @param s - the starting position.
       * @param e - the end position.
       * @param loc - the locator describing the world.
       */
      AStar(const utils::Point2f& s,
            const utils::Point2f& e,
            LocatorShPtr loc);

      /**
       * @brief - Used to generate the path from the start
       *          position to the end and return the path
       *          that was built in the output argument.
       *          In case no path can be found, the return
       *          value will indicate so.
       * @param path - the path generated to reach the two
       *               end points of this finder.
       * @param radius - a float value indicating how far
       *                 from the starting point the path
       *                 is allowed to go and still be
       *                 declared valid. Any path that is
       *                 led to go farther from the start
       *                 is considered not realistic for
       *                 the entity to follow.
       * @param allowLog - `true` if the process should be
       *                   logged.
       * @return - `true` if a path could be find.
       */
      bool
      findPath(std::vector<utils::Point2f>& path,
               float radius = -1.0f,
               bool allowLog = false) const noexcept;

    private:

      /**
       * @brief - Used to reconstruct the path stored in
       *          the object assuming that we found a valid
       *          path from the ending point.
       * @param parents - a description of the explored nodes
       *                  with their `parent` (i.e. the node
       *                  that was explored just before).
       * @param path - output vector which will contain the
       *               reconstructed path.
       * @param allowLog - `true` if the process should be
       *                   logged.
       * @return - `true` if the path could be reconstructed.
       */
      bool
      reconstructPath(const std::unordered_map<int, int>& parents,
                      std::vector<utils::Point2f>& path,
                      bool allowLog) const noexcept;

      /**
       * @brief - Used to perform a smoothing of the input path
       *          to reduce the amount of sharp turns that it
       *          contains.
       * @param path - the path to smooth out. Note that the
       *               smoothened path will be returned directly
       *               in this output argument.
       * @param allowLog - `true` if the process should be
       *                   logged.
       */
      void
      smoothPath(std::vector<utils::Point2f>& path, bool allowLog) const noexcept;

    private:

      /**
       * @brief - The starting point of the algorithm.
       */
      utils::Point2f m_start;

      /**
       * @brief - The end point of the algorithm.
       */
      utils::Point2f m_end;

      /**
       * @brief - A locator object allowing to determine
       *          whether a location is obstructed.
       */
      LocatorShPtr m_loc;
  };

}

#endif    /* ASTAR_HH */
