#ifndef MBGL_M_POINT_ANNOTATION
#define MBGL_M_POINT_ANNOTATION

#include <mbgl/util/geo.hpp>

#include <string>

namespace mbgl {

class MPointAnnotation {
public:
    MPointAnnotation(const LatLng& position_, const std::string& symbol_ = "")
        : position(position_.wrapped()), symbol(symbol_) {}

    const LatLng position;
    const std::string symbol;
};

} // namespace mbgl

#endif
