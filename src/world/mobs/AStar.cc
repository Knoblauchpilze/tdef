
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

    /**
     * @brief - Use a basic string transformation to do
     *          a conversion of the coordinates for this
     *          node to a single string.
     * @return - the mapped string for this node.
     */
    std::string
    hash() const noexcept;

    /**
     * @brief - Used to invert the hash provided as input
     *          to a set of coordinates. As we hash any
     *          non integer coordinates to the same value
     *          we will consider that the point lies at
     *          the center of the cell.
     * @param hash - the value to invert.
     * @return - the normalized coordinates computed from
     *           the hash.
     */
    static
    utils::Point2f
    invertHash(const std::string& hash) noexcept;
  };

  inline
  bool
  Node::contains(const utils::Point2f& m) const noexcept {
    // We want to compare the floating point values
    // by considering that they belong to the cell
    // right before the one it is in.
    // So for example:
    // `(1.2, 2.7)` -> `(1, 2)`.
    // `(1.7, 2.2)` -> `(1, 2)`.
    // But also:
    // `(-0.2, -0.7)` -> `(-1, -1)`
    // `(-0.9, -1.1)` -> `(-1, -2)`.
    //
    // At first, we thought about just casting the
    // values to `int`. But it fails for negative
    // values (as `-0.9` would get converted to a
    // `-0`).
    // The `round` approach also does not work for
    // positive values (as `2.7` will be converted
    // to `3`).
    // What should work is to subtract `1` in the
    // negative case so that the conversion moves
    // the cell to the current one *away* from `0`
    // and not *towards* `0`.
    float px = p.x();
    float py = p.y();

    if (px < 0.0f) {
      px -= 1.0f;
    }
    if (py < 0.0f) {
      py -= 1.0f;
    }

    float mx = m.x();
    float my = m.y();

    if (mx < 0.0f) {
      mx -= 1.0f;
    }
    if (my < 0.0f) {
      my -= 1.0f;
    }

    // Now convert the cell values and compare
    // them to check for inclusion.
    int iPX = static_cast<int>(px);
    int iPY = static_cast<int>(py);

    int iMX = static_cast<int>(mx);
    int iMY = static_cast<int>(my);

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
  std::string
  Node::hash() const noexcept {
    // We first tried to use the Cantor pairing
    // function but as usual the fact that the
    // generated keys are usually quite large
    // was not really suited to our needs: as
    // we can have large coordinates in large
    // worlds the hashing should stay bounded.
    // The string approach (by basically just
    // concatenating the two coordinates) has
    // the advantage to work no matter the
    // dimensions of the world.
    int k1 = static_cast<int>(std::floor(p.x()));
    int k2 = static_cast<int>(std::floor(p.y()));

    return std::to_string(k1) + "#" + std::to_string(k2);
  }

  inline
  utils::Point2f
  Node::invertHash(const std::string& hash) noexcept {
    // Deconcatenate the input hash and offset
    // with half a cell the produced coords.
    std::size_t id = hash.find('#');

    if (id == std::string::npos) {
      // This is very weird, assume a default
      // node.
      return utils::Point2f(0.5f, 0.5f);
    }

    std::string xStr = hash.substr(0, id);
    std::string yStr = hash.substr(id + 1);

    float x = 0.5f + std::stof(xStr);
    float y = 0.5f + std::stof(yStr);

    return utils::Point2f(x, y);
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

    using AssociationMap = std::unordered_map<std::string, int>;

    // The `cameFrom[i]` defines the index of its parent
    // node, i.e. the node we were traversing when this
    // node was encountered.
    std::unordered_map<std::string, std::string> cameFrom;
    AssociationMap associations;

    associations[init.hash()] = 0;

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

        // Attempt to reconstruct the path.
        if (!reconstructPath(cameFrom, out, allowLog)) {
          return false;
        }

        // Smooth out the sharp turns that might have
        // been produced by the A*.
        smoothPath(out, allowLog);

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

        return valid;
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
        // and prevent the registration of the node if it
        // is not valid in the sense defined in the above
        // section.
        if ((id == NorthEast && !validNE) ||
            (id == SouthEast && !validSE) ||
            (id == SouthWest && !validSW) ||
            (id == NorthWest && !validNW))
        {
          continue;
        }

        // Make sure that we don't consider nodes farther
        // away from the source as defined by the user.
        if (radius > 0.0f && utils::d(m_start, neighbor.p) >= radius) {
          continue;
        }

        AssociationMap::iterator it = associations.find(neighbor.hash());

        if (it == associations.end() || neighbor.c < nodes[it->second].c) {
          // This path to neighbor is better than any previous one.
          cameFrom[neighbor.hash()] = current.hash();

          if (it != associations.end()) {
            if (allowLog) {
              log(
                "Updating " + std::to_string(neighbor.p.x()) + "x" + std::to_string(neighbor.p.y()) +
                " from c " + std::to_string(nodes[it->second].c) + ", " + std::to_string(nodes[it->second].h) +
                " (f: " + std::to_string(nodes[it->second].c + nodes[it->second].h) + "," +
                " parent: " + cameFrom[nodes[it->second].hash()] + ")" +
                " to c: " + std::to_string(neighbor.c) + " h: " + std::to_string(neighbor.h) +
                " (f: " + std::to_string(neighbor.c + neighbor.h) + "," +
                " parent is " + current.hash() + ")",
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
                " parent is " + current.hash() + ")",
                utils::Level::Verbose
              );
            }

            openNodes.push_back(nodes.size());
            associations[neighbor.hash()] = nodes.size();
            nodes.push_back(neighbor);
          }
        }
      }
    }

    // We couldn't reach the goal, the algorithm failed.
    return false;
  }

  bool
  AStar::reconstructPath(const std::unordered_map<std::string, std::string>& parents,
                         std::vector<utils::Point2f>& path,
                         bool allowLog) const noexcept
  {
    std::vector<utils::Point2f> out;

    Node n{m_end, 0.0f, 0.0f};
    std::string h = n.hash();
    std::unordered_map<std::string, std::string>::const_iterator it = parents.find(h);

    while (it != parents.cend()) {
      n.p = Node::invertHash(h);

      if (allowLog) {
        log(
          "Registering point " + std::to_string(n.p.x()) + "x" + std::to_string(n.p.y()) +
          " with hash " + h +
          ", parent is " + it->second,
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
    std::string sh = n.hash();

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
    while (id < path.size() - 1u) {
      utils::Point2f c = path[id + 1u];

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
            " is unobstructed (obs: " + std::to_string(obs) + ", cont: " + std::to_string(end.contains(o)) + ")",
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
