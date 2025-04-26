#ifndef META_HPP
#define META_HPP

/*
  Aggregates and labels function pointers to mutable members of engine classes.

  Example:
  Animation wants to access and write to path "crate12.sprite.region", seperated by periods.
  If it exists, it'll change the value using the registered setter. If anything's wrong,
  it's fine and will just report an error.

  These getters/setters are registered programmatically for each class in the Meta instance.


  metadb.register_class("Sprite")
  metadb.register_member("Position", &Sprite::get_position, &Sprite::set_position)
  metadb.register_member("Region", &Sprite::get_region, &Sprite::set_region)
  
 */

#include <any>

namespace Turbine {
struct MetaDB {
private:
    using m_getter = std::function<std::any(void*)>;
    using m_setter = std::function<void(void*, const std::any&)>;

    struct m_property {
        m_setter setter;
        m_getter getter; 
    };

    void register_class();
};
}

#endif
