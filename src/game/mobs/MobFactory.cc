
# include "MobFactory.hh"
# include "Armored.hh"
# include "Fast.hh"
# include "Fighter.hh"
# include "Healer.hh"
# include "Heli.hh"
# include "Icy.hh"
# include "Jet.hh"
# include "Light.hh"
# include "Regular.hh"
# include "Strong.hh"
# include "Toxic.hh"

namespace tdef {
  namespace mobs {

    Mob::MProps
    generateProps(const Type& type,
                  const utils::Point2f& p,
                  int level)
    {
      switch (type) {
        case Type::Fast:
          return fast::generateProps(p, level);
        case Type::Strong:
          return strong::generateProps(p, level);
        case Type::Heli:
          return heli::generateProps(p, level);
        case Type::Jet:
          return jet::generateProps(p, level);
        case Type::Armored:
          return armored::generateProps(p, level);
        case Type::Healer:
          return healer::generateProps(p, level);
        case Type::Toxic:
          return toxic::generateProps(p, level);
        case Type::Icy:
          return icy::generateProps(p, level);
        case Type::Fighter:
          return fighter::generateProps(p, level);
        case Type::Light:
          return light::generateProps(p, level);
        case Type::Regular:
        default:
          // Assume default is regular mob.
          return regular::generateProps(p, level);
      }
    }

  }
}
