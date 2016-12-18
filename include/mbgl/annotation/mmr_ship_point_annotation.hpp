#ifndef MBGL_MMR_SHIP_POINT_ANNOTATION
#define MBGL_MMR_SHIP_POINT_ANNOTATION

#include <mbgl/util/geo.hpp>

#include <string>

namespace mbgl {

class MMRShipPointAnnotation {
public:
    MMRShipPointAnnotation(const LatLng& position_, const std::string& type_ = "", const int rot_i_ = 0, const int sog_i_ = 0, const int cog_i_ = 0)
        : position(position_.wrapped()), type(type_), rot_i(rot_i_), sog_i(sog_i_), cog_i(cog_i_) {}

    const LatLng position;
    const std::string type;
    const int rot_i;
    const int sog_i;
    const int cog_i;
};

} // namespace mbgl

#endif
