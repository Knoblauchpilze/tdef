
# include "AStar.hh"
# include <deque>
# include <iterator>

namespace {

  /**
   * @brief - Enumeration describing the position of a node
   *          relatively to its neighbots.
   */
  enum Neighbor {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
    Count
  };

  /**
   * @brief - Convenience structure to define an opened node.
   */
  struct Node {
    utils::Point2f p;
    float c;
    float h;

    bool
    contains(const utils::Point2f& p) const noexcept;

    std::vector<Node>
    generateNeighbors(const utils::Point2f& target) const noexcept;

    int
    hash(int offset) const noexcept;

    static
    utils::Point2f
    invertHash(int hash, int offset) noexcept;
  };

  inline
  bool
  Node::contains(const utils::Point2f& m) const noexcept {
    int iPX = static_cast<int>(p.x());
    int iPY = static_cast<int>(p.y());

    int iMX = static_cast<int>(m.x());
    int iMY = static_cast<int>(m.y());

    return iPX == iMX && iPY == iMY;
  }

  inline
  std::vector<Node>
  Node::generateNeighbors(const utils::Point2f& target) const noexcept {
    std::vector<Node> neighbors(Count);

    utils::Point2f np;

    float bx = std::floor(p.x()) + 0.5f;
    float by = std::floor(p.y()) + 0.5f;

    // Right neighbor.
    np.x() = bx + 1.0f; np.y() = by;
    neighbors[East] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Top right neighbor.
    np.x() = bx + 1.0f; np.y() = by + 1.0f;
    neighbors[NorthEast] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Up neighbor.
    np.x() = bx; np.y() = by + 1.0f;
    neighbors[North] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Top left neighbor.
    np.x() = bx - 1.0f; np.y() = by + 1.0f;
    neighbors[NorthWest] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Left neighbor.
    np.x() = bx - 1.0f; np.y() = by;
    neighbors[West] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Bottom left neighbor.
    np.x() = bx - 1.0f; np.y() = by - 1.0f;
    neighbors[SouthWest] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Down neighbor.
    np.x() = bx; np.y() = by - 1.0f;
    neighbors[South] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    // Bottom right neighbor.
    np.x() = bx + 1.0f; np.y() = by - 1.0f;
    neighbors[SouthEast] = Node{
      np,
      c + utils::d(p, np),
      utils::d(np, target)
    };

    return neighbors;
  }

  inline
  int
  Node::hash(int offset) const noexcept {
    return static_cast<int>(std::floor(p.y())) * offset + static_cast<int>(std::floor(p.x()));
  }

  inline
  utils::Point2f
  Node::invertHash(int hash, int offset) noexcept {
    utils::Point2f p;

    p.x() = 0.5f + hash % offset;
    p.y() = 0.5f + hash / offset;

    return p;
  }

}

namespace tdef {

  AStar::AStar(const utils::Point2f& s,
               const utils::Point2f& e,
               LocatorShPtr loc):
    utils::CoreObject("algo"),

    m_start(s),
    m_end(e),

    m_loc(loc)
  {
    setService("astar");
  }

  bool
  AStar::findPath(std::vector<utils::Point2f>& path,
                  float radius,
                  bool allowLog) const noexcept
  {
    // The code for this algorithm has been taken from the
    // below link:
    // https://en.wikipedia.org/wiki/A*_search_algorithm
    std::vector<utils::Point2f> out;
    path.clear();

    // The list of nodes that are currently being explored.
    std::vector<Node> nodes;
    std::deque<int> openNodes;
    Node init{m_start, 0.0f, utils::d(m_start, m_end)};
    nodes.push_back(init);
    openNodes.push_back(0);

    if (allowLog) {
      log(
        "Starting a* at " + std::to_string(m_start.x()) + "x" + std::to_string(m_start.y()) +
        " to reach " + std::to_string(m_end.x()) + "x" + std::to_string(m_end.y()),
        utils::Level::Verbose
      );
    }

    using AssociationMap = std::unordered_map<int, int>;

    // The `cameFrom[i]` defines the index of its parent
    // node, i.e. the node we were traversing when this
    // node was encountered.
    AssociationMap cameFrom;
    AssociationMap associations;

    associations[init.hash(m_loc->w())] = 0;

    // Common lambdas to handle sorting and distance
    // computation from a point to another.
    auto cmp = [&nodes](int lhs, int rhs) {
      const Node& nlhs = nodes[lhs];
      const Node& nrhs = nodes[rhs];
      return nlhs.c + nlhs.h < nrhs.c + nrhs.h;
    };

    while (!openNodes.empty()) {
      // Sort the open list to fetch nodes with smallest
      // `c + h` value.
      std::sort(openNodes.begin(), openNodes.end(), cmp);
      Node current = nodes[openNodes.front()];
      openNodes.pop_front();

      if (allowLog) {
        log(
          "Picked node " + std::to_string(current.p.x()) + "x" + std::to_string(current.p.y()) +
          " with c " + std::to_string(current.c) +
          " h is " + std::to_string(current.h) +
          " (nodes: " + std::to_string(openNodes.size()) + ")",
          utils::Level::Verbose
        );
      }

      // In case we reached the goal, stop there.
      if (current.contains(m_end)) {
        if (allowLog) {
          log(
            "Found path to " + std::to_string(m_end.x()) + "x" + std::to_string(m_end.y()) +
            " with c " + std::to_string(current.c) + ", h " + std::to_string(current.h),
            utils::Level::Verbose
          );
        }

        bool found = reconstructPath(cameFrom, m_loc->w(), out, allowLog);
        if (found) {
          // Smooth out the sharp turns that might have
          // been produced by the A*.
          smoothPath(out, allowLog);
        }

        // Check whether the path goes beyong the input
        // limit at any point: if this is the case we
        // will prevent it from being returned as we do
        // not consider it valid.
        bool valid = true;

        // In case the `radius` is negative, consider
        // that there's no limit.
        if (radius >= 0.0f) {
          unsigned id = 0u;

          while (id < out.size() && valid) {
            valid = (utils::d(m_start, out[id]) < radius);

            if (!valid && allowLog) {
              log(
                "Distance from start " + std::to_string(m_start.x()) + "x" + std::to_string(m_start.y()) +
                " to point " + std::to_string(id) + "/" + std::to_string(out.size()) +
                " " + std::to_string(out[id].x()) + "x" + std::to_string(out[id].y()) +
                " is " + std::to_string(utils::d(m_start, out[id])) +
                ", limit is " + std::to_string(radius)
              );
            }

            ++id;
          }
        }

        // Copy the path if it is valid.
        if (valid) {
          std::swap(path, out);
        }

        return found && valid;
      }

      // Also, consider the node if it is not obstructed
      // in a less obvious way as below:
      //
      // +----+----+
      // | Ob |  E |
      // |    |    |
      // +----+----+
      // |    | Ob |
      // | S  |    |
      // +----+----+
      //
      // This situation can only happen for the diagonal
      // neighbors (so either SE, SW, NE, NW).
      // In order to avoid entities going through the wall
      // we will also forbid the following situation:
      //
      // +----+----+
      // | Ob |  E |
      // |    |    |
      // +----+----+
      // |    |    |
      // | S  |    |
      // +----+----+
      //
      // Basically here we we consider the `E` neighbor
      // as valid we will reach cases where we are not
      // able to prevent the entity from going through
      // the `Ob` so we will just not allow it.
      // We will first determine before processing the
      // neighbors and check the status for each one.
      std::vector<Node> neighbors = current.generateNeighbors(m_end);

      float bx = std::floor(current.p.x()) + 0.5f;
      float by = std::floor(current.p.y()) + 0.5f;

      bool obsE = m_loc->obstructed(utils::Point2f(bx + 1.0f, by));
      bool obsN = m_loc->obstructed(utils::Point2f(bx, by + 1.0f));
      bool obsW = m_loc->obstructed(utils::Point2f(bx - 1.0f, by));
      bool obsS = m_loc->obstructed(utils::Point2f(bx, by - 1.0f));

      bool validNE = (!obsN && !obsE);
      bool validNW = (!obsN && !obsW);
      bool validSW = (!obsS && !obsW);
      bool validSE = (!obsS && !obsE);

      for (unsigned id = 0u ; id < neighbors.size() ; ++id) {
        Node& neighbor = neighbors[id];

        // Only consider the node if it is not obstructed.
        if (m_loc->obstructed(neighbor.p) && !neighbor.contains(m_end)) {
          continue;
        }

        // Use the pre-computed diagonal neighbors status
        // and prevent the registration of the it if it
        // is not valid in the sense defined in the above
        // section.
        if ((id == NorthEast && !validNE) ||
            (id == SouthEast && !validSE) ||
            (id == SouthWest && !validSW) ||
            (id == NorthWest && !validNW))
        {
          continue;
        }

        // Prevent neighbors outside of the map.
        if (neighbor.p.x() < 0.0f || neighbor.p.x() >= m_loc->w() ||
            neighbor.p.y() < 0.0f || neighbor.p.y() >= m_loc->h())
        {
          continue;
        }

        AssociationMap::iterator it = associations.find(neighbor.hash(m_loc->w()));

        if (it == associations.end() || neighbor.c < nodes[it->second].c) {
          // This path to neighbor is better than any previous one.
          cameFrom[neighbor.hash(m_loc->w())] = current.hash(m_loc->w());

          if (it != associations.end()) {
            if (allowLog) {
              log(
                "Updating " + std::to_string(neighbor.p.x()) + "x" + std::to_string(neighbor.p.y()) +
                " from c " + std::to_string(nodes[it->second].c) + ", " + std::to_string(nodes[it->second].h) +
                " (f: " + std::to_string(nodes[it->second].c + nodes[it->second].h) + "," +
                " parent: " + std::to_string(cameFrom[nodes[it->second].hash(m_loc->w())]) + ")" +
                " to c: " + std::to_string(neighbor.c) + " h: " + std::to_string(neighbor.h) +
                " (f: " + std::to_string(neighbor.c + neighbor.h) + "," +
                " parent is " + std::to_string(current.hash(m_loc->w())) + ")",
                utils::Level::Verbose
              );
            }

            nodes[it->second].c = neighbor.c;
          }
          else {
            if (allowLog) {
              log(
                "Registering " + std::to_string(neighbor.p.x()) + "x" + std::to_string(neighbor.p.y()) +
                " with c: " + std::to_string(neighbor.c) + " h: " + std::to_string(neighbor.h) +
                " (f: " + std::to_string(neighbor.c + neighbor.h) + "," +
                " parent is " + std::to_string(current.hash(m_loc->w())) + ")",
                utils::Level::Verbose
              );
            }

            openNodes.push_back(nodes.size());
            associations[neighbor.hash(m_loc->w())] = nodes.size();
            nodes.push_back(neighbor);
          }
        }
      }
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstructPath(const std::unordered_map<int, int>& parents,
                         int offset,
                         std::vector<utils::Point2f>& path,
                         bool allowLog) const noexcept
  {
    std::vector<utils::Point2f> out;

    Node n{m_end, 0.0f, 0.0f};
    int h = n.hash(offset);
    std::unordered_map<int, int>::const_iterator it = parents.find(h);

    while (it != parents.cend()) {
      n.p = Node::invertHash(h, offset);

      if (allowLog) {
        log(
          "Registering point " + std::to_string(n.p.x()) + "x" + std::to_string(n.p.y()) +
          " with hash " + std::to_string(h) +
          ", parent is " + std::to_string(it->second),
          utils::Level::Verbose
        );
      }

      if (n.contains(m_end)) {
        out.push_back(m_end);
      }
      else {
        out.push_back(n.p);
      }
      h = it->second;
      it = parents.find(h);
    }

    // Make sure that we reached the starting point.
    // If this is the case we can copy the path we
    // just built to the output argument.
    n.p = m_start;
    int sh = n.hash(offset);

    if (sh == h) {
      // We need to reverse the path as we've built
      // it from the end.
      for (std::vector<utils::Point2f>::const_reverse_iterator it = out.crbegin() ;
           it != out.crend() ;
           ++it)
      {
        path.push_back(*it);
      }

      // We also need to straighten the first segment
      // of the path: indeed we never check that the
      // path between the starting location and the
      // first cell is unobstructed: if this is the
      // case, we need to add the center of the cell
      // containing the starting location as an
      // intermediate position as we know the path
      // from there to the first segment will be
      // valid.
      utils::Point2f pObs(-1.0f, -1.0f);
      std::vector<utils::Point2f> dummy;

      if (allowLog) {
        log(
          "Checking obstruction between " +
          std::to_string(m_start.x()) + "x" + std::to_string(m_start.y()) +
          " and " +
          std::to_string(path[0].x()) + "x" + std::to_string(path[0].y()),
          utils::Level::Verbose
        );
      }

      if (m_loc->obstructed(m_start, path[0], dummy, &pObs, 0.005f)) {
        utils::Point2f ip(
          0.5f + static_cast<int>(std::floor(m_start.x())),
          0.5f + static_cast<int>(std::floor(m_start.y()))
        );

        if (allowLog) {
          log(
            "Registering point " + std::to_string(ip.x()) + "x" + std::to_string(ip.y()) +
            " as path from " + std::to_string(m_start.x()) + "x" + std::to_string(m_start.y()) +
            " to " + std::to_string(path[0].x()) + "x" + std::to_string(path[0].y()) +
            " is obstructed",
            utils::Level::Verbose
          );
        }

        path.insert(path.begin(), ip);
      }
    }

    return sh == h;
  }

  void
  AStar::smoothPath(std::vector<utils::Point2f>& path, bool allowLog) const noexcept {
    // The basic idea is taken from this very interesting
    // article found in Gamasutra:
    // https://www.gamasutra.com/view/feature/131505/toward_more_realistic_pathfinding.php?page=2
    // The idea is to remove progressively intermediate
    // positions on the path as long as the next step can
    // be reached with a straight line from the previous
    // point.
    // The starting point of the path is already the first
    // element in the `path` so we can start from there.
    // We will eliminate the trivial case where the path
    // is emtpy right away: this corresponds to all paths
    // that don't have at least 3 elements (and thus where
    // no simplification can occur).
    if (path.size() < 2u) {
      return;
    }

    std::vector<utils::Point2f> out;
    utils::Point2f p = m_start;
    Node end{m_end, 0.0f, 0.0f};

    unsigned id = 0u;
    std::vector<utils::Point2f> dummy;

    // Simplify the whole path.
    int count = 0;
    while (id < path.size() - 1u && count < 10) {
      utils::Point2f c = path[id + 1u];
      Node n{c, 0.0f, 0.0f};

      // Check whether the starting position and the
      // current point can be joined by a straight
      // line without obstructions. Note that we will
      // ignore obstructions in the target.
      utils::Point2f o;
      bool obs = m_loc->obstructed(p, c, dummy, &o, 0.005f, allowLog);
      if (!obs || end.contains(o)) {
        // The path can be reached in a straight line,
        // we can remove the current point.
        if (allowLog) {
          log(
            "Simplified point " + std::to_string(path[id].x()) + "x" + std::to_string(path[id].y()) +
            " as path from " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
            " to " + std::to_string(c.x()) + "x" + std::to_string(c.y()) +
            " is unobstructed (obs: " + std::to_string(obs) + ", cont: " + std::to_string(n.contains(m_end)) + ")",
            utils::Level::Verbose
          );
        }
        ++id;
      }
      else {
        // Can't reach the point from the current start.
        // This segment cannot be simplified further.
        if (allowLog) {
          log(
            "Can't simplify path from " + std::to_string(p.x()) + "x" + std::to_string(p.y()) +
            " to point " + std::to_string(c.x()) + "x" + std::to_string(c.y()) +
            " (id: " + std::to_string(id) + ", s: " + std::to_string(path.size()) + ")" +
            " registering " + std::to_string(p.x()) + "x" + std::to_string(p.y()),
            utils::Level::Verbose
          );
        }
        out.push_back(p);
        p = path[id];
        ++id;
      }

      ++count;
    }

    // Register the last points.
    out.push_back(p);
    out.push_back(path.back());

    // Swap the simplified path with the input argument.
    path.swap(out);

    if (allowLog) {
      for (unsigned id = 0u ; id < path.size() ; ++id) {
        log(
          "Point " + std::to_string(id) + "/" + std::to_string(path.size()) +
          " at " + std::to_string(path[id].x()) + "x" + std::to_string(path[id].y()),
            utils::Level::Verbose
        );
      }
    }
  }

}
