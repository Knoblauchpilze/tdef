#ifndef    PORTAL_HH
# define   PORTAL_HH

# include <memory>
# include <maths_utils/Point2.hh>
# include "Block.hh"

namespace tdef {

  class Portal: public Block {
    public:

      /**
       * @brief - Definition of new props to define a portal.
       */
      struct PProps: public Block::BProps {
        float lives;
      };

      static
      PProps
      newProps(const utils::Point2f& p,
               const utils::Uuid& owner = utils::Uuid()) noexcept;

      /**
       * @brief - Defines a new spawner with the specified props.
       * @param props - the properties defining this wall.
       */
      Portal(const PProps& props);

      float
      getLives() const noexcept;

      /**
       * @brief - Method to handle the breaching of the portal
       *          by an entity. This allows to specialize what
       *          needs to be done in this case.
       * @param lives - the number of lives that this portal
       *                has been breached to.
       */
      void
      breach(float lives);

      std::ostream&
      operator<<(std::ostream& out) const override;

      std::istream&
      operator>>(std::istream& in) override;

    private:

      /**
       * @brief - The number of lives left for this portal. It is
       *          mostly related to the number of mobs that can
       *          still pass in this portal before it becomes a
       *          dead portal (i.e. inactivated).
       */
      float m_lives;
  };

  using PortalShPtr = std::shared_ptr<Portal>;
}

std::ostream&
operator<<(std::ostream& out, const tdef::Portal& p) noexcept;

std::istream&
operator>>(std::istream& in, tdef::Portal& p) noexcept;

# include "Portal.hxx"

#endif    /* PORTAL_HH */
