#ifndef PARENT_HPP
#define PARENT_HPP

#include <KumaECS/ECS.hpp>

namespace StarBear {

struct Parent {
  KumaECS::EntitySet mChildren;
};

} // namespace StarBear

#endif
