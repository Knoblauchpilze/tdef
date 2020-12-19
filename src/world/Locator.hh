#ifndef    LOCATOR_HH
# define   LOCATOR_HH

# include <memory>
# include <unordered_set>
# include <core_utils/CoreObject.hh>
# include "Block.hh"
# include "Mob.hh"

namespace tdef {

  namespace world {

    /**
     * @brief - Convenience structure allowing to define
     *          a status for identifiers when retrieving
     *          elements based on their visibility.
     */
    struct Filter {
      utils::Uuid id;
      bool include;
    };

    /**
     * @brief - Convenience wrapper allowing to access the
     *          properties of a block.
     */
    struct Block {
      utils::Point2f p;
      float radius;
      float health;
    };

    /**
     * @brief - Convenience wrapper allowing to access the
     *          properties of a mob.
     */
    struct Mob {
      utils::Point2f p;
      float radius;
      float health;
    };

    /**
     * @brief - Convenience enumeration defining the possible
     *          types for an item visible in the frustum of
     *          the locator.
     */
    enum class ItemType {
      Block,
      Mob,
    };

    /**
     * @brief - Convenience structure defining an entry in the
     *          visibility frustum returned by the locator. It
     *          defines the type of the object along with its
     *          index which can then be used to fetch it from
     *          the locator itself (and thus retrieve more info
     *          on it).
     */
    struct ItemEntry {
      ItemType type;
      int index;
    };

    /**
     * @brief - Convenience enumeration defining the possible
     *          sort algorithms to use.
     */
    enum class Sort {
      None,
      Distance,
      ZOrder
    };

  }

  class Locator: public utils::CoreObject {
    public:

      /**
       * @brief - Build a new locator for in-game elements. This
       *          class aims at providing efficient location and
       *          filtering method to detect specific entities
       *          or solid elements in the world.
       *          The details of the implementation do not need
       *          to be exposed to external users so it's hidden
       *          in here. One can use the methods to query the
       *          elements through the dedicated interface.
       * @param width - the width of the world in which elements
       *                are registered. Expressed in cells.
       * @param height - similar to `width` but for the height
       *                 of the world.
       * @param blocks - the list of tiles registered in the
       *                 world.
       * @param mobs - the list of mobs of the world.
       */
      Locator(int width,
              int height,
              const std::vector<BlockShPtr>& blocks,
              const std::vector<MobShPtr>& mobs);

      /**
       * @brief - Return the width of the world in cells.
       * @return - the number of cell in width for the
       *           world attached to this locator.
       */
      int
      w() const noexcept;

      /**
       * @brief - Return the height of the world in cells.
       * @return - the number of cell in height for the
       *           world attached to this locator.
       */
      int
      h() const noexcept;

      /**
       * @brief - Retrieve the tile at the specified index. Note
       *          that no checks are performed to verify that it
       *          is indeed possible to access to this tile: if
       *          it is not UB will arise.
       * @param id - index of the tile to access.
       * @return - the tile at the specified index.
       */
      world::Block
      block(int id) const noexcept;

      /**
       * @brief - Similar to the `solidTile` method but to get
       *          the entity at the specified index.
       * @param id - the index of the entity to get.
       * @return - the corresponding entity.
       */
      world::Mob
      mob(int id) const noexcept;

      /**
       * @brief - Used to indicate that the locator should be
       *          recomputing the spatially-optimized layout
       *          due probably to some motions.
       *          An internal system is used to detect items
       *          that have changed so it's somewhat safe to
       *          call this more often than needed.
       */
      void
      refresh();

      /**
       * @brief - Allow to determine whether a specific loc
       *          is obstructed (meaning that a block already
       *          exists here) or not.
       * @param x - the abscissa to check for occupancy.
       * @param y - the ordinate to check for occupancy.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(float x, float y) const noexcept;

      /**
       * @brief - Allow to determine whether a specific loc
       *          is obstructed (meaning that a block already
       *          exists here) or not.
       * @param p - the position to check for occupancy.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(const utils::Point2f& p) const noexcept;

      /**
       * @brief - Determine whether the path defined by the
       *          input coordinate and the direction has any
       *          obstructing elements on it within `d` cells
       *          of the starting position.
       *          Note that we don't consider the first cell
       *          to be a valid candidate for obstruction.
       * @param p - the starting position of the path which
       *            needs to be checked for obstruction.
       * @param xDir - the direction of the ray along the `x`
       *               axis.
       * @param yDir - the direction of the ray along the `y`
       *               axis.
       * @param d - the distance to check for obstruction.
       * @param cPoints - the list of points considered in
       *                  the obsctruction process.
       * @param obs - if not `null` will output the position
       *               of the first obstruction. Only relevant
       *               if the return value is `true`.
       * @param sample - the sampling distance to use when we
       *                 discretly check intermediate points
       *                 on the line joining the start to the
       *                 end of the path. Smaller values can
       *                 be used to detect smaller obstacles
       *                 but also increase the compute time.
       * @param allowLog - `true` if logs are enabled.
       * @return - `true` if the path is obstructed by any
       *           solid element.
       */
      bool
      obstructed(utils::Point2f p,
                 float xDir,
                 float yDir,
                 float d,
                 std::vector<utils::Point2f>& cPoints,
                 utils::Point2f* obs = nullptr,
                 float sample = 0.05f,
                 bool allowLog = false) const noexcept;

      /**
       * @brief - Very similar to the above method. Checks
       *          for obstruction in the line joining the
       *          `p` and `e` endpoints.
       * @param p - the starting point of the segment to
       *            check for obstructions.
       * @param e - the end point of the segment.
       * @param cPoints - the list of points considered in
       *                  the obsctruction process.
       * @param obs - if not `null` will output the position
       *               of the first obstruction. Only relevant
       *               if the return value is `true`.
       * @param sample - the sampling interval on the line.
       * @param allowLog - `true` if logs are enabled.
       * @return - `true` if the line is obstructed.
       */
      bool
      obstructed(utils::Point2f p,
                 utils::Point2f e,
                 std::vector<utils::Point2f>& cPoints,
                 utils::Point2f* obs = nullptr,
                 float sample = 0.05f,
                 bool allowLog = false) const noexcept;

      /**
       * @brief - Return the list of items that are visible
       *          in the view frustum defined by the AABB
       *          from the input values.
       *          All items are included (blocks, entities
       *          and VFXs) and the user can request items
       *          to be sorted.
       * @param xMin - the abscissa of the top left corner
       *               of the view frustum.
       * @param yMin - the ordinate of the top left corner
       *               of the view frustum.
       * @param xMax - the abscissa of the bottom right
       *               corner of the view frustum.
       * @param yMax - the ordinate of the bottom right
       *               corner of the view frustum.
       * @param type - the type of elements to consider. If
       *               this value is set to `null` all the
       *               item types will be included.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @param sort - the algorithm to use when performing the
       *               sorting operation (none by default).
       * @return - the list of items that are visible in the
       *           input view frustum.
       */
      std::vector<world::ItemEntry>
      getVisible(float xMin,
                 float yMin,
                 float xMax,
                 float yMax,
                 const world::ItemType* type = nullptr,
                 const world::Filter* filters = nullptr,
                 world::Sort sort = world::Sort::None) const noexcept;

      /**
       * @brief - Retrieve the list of elements with a type
       *          as defined in input or all the elements if
       *          no type is provided.
       *          The area in which elements should be fetched
       *          is described through a circle with a center
       *          and a radius.
       * @param p - the position of the point from which a set
       *            of visible elements should be found.
       * @param r - the radius of the area to search for. If
       *            this value is negative there's no limits
       *            to the distance between the element and the
       *            input position.
       * @param type - the type of elements to consider. If
       *               this value is set to `null` all the
       *               item types will be included.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @param sort - the algorithm to use when performing the
       *               sorting operation (none by default).
       * @return - the list of elements corresponding in the
       *           specified area.
       */
      std::vector<world::ItemEntry>
      getVisible(const utils::Point2f& p,
                 float r,
                 const world::ItemType* type = nullptr,
                 const world::Filter* filter = nullptr,
                 world::Sort sort = world::Sort::None) const noexcept;

      /**
       * @brief - Similar to the `getVisible` method but
       *          only returns the closest element from
       *          the available items.
       *          In case there are no elements to return
       *          the output will have a negative index.
       * @param p - the position of the position from which
       *            an element should be found.
       * @param type - the type of the element to search
       *               for.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @return - the corresponding element's description
       *           or a negative index if none can be
       *           found.
       */
      world::ItemEntry
      getClosest(const utils::Point2f& p,
                 const world::ItemType& type,
                 const world::Filter& filter) const noexcept;

      /**
       * @brief - Specialization of the `getVisible` method
       *          to fetch visible blocks in a certain area
       *          and return this as a list of blocks.
       * @param p - the position of the center of the area to
       *            consider.
       * @param r - the radius of the area to consider.
       * @param filters - include a description of a uuid and
       *                  whether or not theyit should be used
       *                  and considered when fetching items.
       * @param sort - the algorithm to use when performing the
       *               sorting operation (none by default).
       * @return - the list of blocks.
       */
      std::vector<BlockShPtr>
      getVisibleBlocks(const utils::Point2f& p,
                       float r,
                       const world::Filter* filter = nullptr,
                       world::Sort sort = world::Sort::None) const noexcept;

      /**
       * @brief - Specialization of the `getVisible` method
       *          to fetch visible entities in a certain area
       *          and return this as a list of entities.
       * @param p - the position of the center of the area to
       *            consider.
       * @param r - the radius of the area to consider.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @param sort - the algorithm to use when performing the
       *               sorting operation (none by default).
       * @return - the list of entities.
       */
      std::vector<MobShPtr>
      getVisibleMobs(const utils::Point2f& p,
                     float r,
                     const world::Filter* filter = nullptr,
                     world::Sort sort = world::Sort::None) const noexcept;

      /**
       * @brief - Similar to the `getVisible` but only returns
       *          the closest block from the total visible list.
       * @param p - the position of the center of the area to
       *            consider.
       * @param r - a limit for the radius for entities to be
       *            considered.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @return - the list of blocks.
       */
      BlockShPtr
      getClosestBlock(const utils::Point2f& p,
                      float r = -1.0f,
                      const world::Filter* filter = nullptr) const noexcept;

      /**
       * @brief - Similar to the `getVisible` but only returns
       *          the closest entity from the total visible list.
       * @param p - the position of the center of the area to
       *            consider.
       * @param r - a limit for the radius for entities to be
       *            considered.
       * @param filters - include a description of a uuid and
       *                  whether or not it should be used
       *                  and considered when fetching items.
       * @return - the list of entities.
       */
      MobShPtr
      getClosestMob(const utils::Point2f& p,
                    float r = -1.0f,
                    const world::Filter* filter = nullptr) const noexcept;

    private:

      /**
       * @brief - Used to initialize the internal variables to
       *          compute information about the data stored in
       *          this world.
       */
      void
      initialize();

    private:

      /**
       * @brief - Define a convenience structure to perform the
       *          sorting of tiles and entities.
       */
      struct SortEntry {
        utils::Point2f p;
        unsigned id;
      };

      /**
       * @brief - The width of the world in cells.
       */
      int m_w;

      /**
       * @brief - The height of the world in cells.
       */
      int m_h;

      /**
       * @brief - The elements registered in the world.
       */
      const std::vector<BlockShPtr>& m_blocks;
      const std::vector<MobShPtr>& m_mobs;

      /**
       * @brief - A map referencing the unique indices for
       *          blocks in the world. It is build as the
       *          object is constructed (or refreshed) and
       *          allows to quickly access to the position
       *          where a block exists.
       */
      std::unordered_set<int> m_blocksIDs;
  };

  using LocatorShPtr = std::shared_ptr<Locator>;
}

# include "Locator.hxx"

#endif    /* LOCATOR_HH */
