#ifndef MBGL_MTN_POINT_ANNOTATION
#define MBGL_MTN_POINT_ANNOTATION

#include <mbgl/util/geo.hpp>

#include <string>

namespace mbgl {

class MTNPointAnnotation {
public:
    MTNPointAnnotation(const LatLng& position_, const std::string& type_ = "", const int idx_i_ = 0, const int nr_i_ = 0, const int selected_i_ = 0)
        : position(position_.wrapped()), type(type_), idx_i(idx_i_), nr_i(nr_i_), selected_i(selected_i_) {}

    const LatLng position;
    const std::string type;
    const int idx_i;
    const int nr_i;
    const int selected_i;
};

} // namespace mbgl

#endif
