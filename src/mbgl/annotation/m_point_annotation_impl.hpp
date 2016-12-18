#ifndef MBGL_M_POINT_ANNOTATION_IMPL
#define MBGL_M_POINT_ANNOTATION_IMPL

#include <mbgl/annotation/annotation.hpp>
#include <mbgl/annotation/m_point_annotation.hpp>
#include <mbgl/util/geo.hpp>

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wdeprecated-register"
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#pragma GCC diagnostic pop

namespace mbgl {

class AnnotationTileLayer;

class MPointAnnotationImpl {
public:
    using Map = std::map<AnnotationID, std::shared_ptr<MPointAnnotationImpl>>;
    using Tree = boost::geometry::index::rtree<std::shared_ptr<const MPointAnnotationImpl>, boost::geometry::index::rstar<16, 4>>;

    MPointAnnotationImpl(const AnnotationID, const MPointAnnotation&);

    void updateLayer(const TileID&, AnnotationTileLayer&) const;

    const AnnotationID id;
    const MPointAnnotation point;
};

} // namespace mbgl

// Tell Boost Geometry how to access a std::shared_ptr<mbgl::PointAnnotation> object.
namespace boost {
namespace geometry {
namespace index {

template <>
struct indexable<std::shared_ptr<const mbgl::MPointAnnotationImpl>> {
    using result_type = const mbgl::LatLng&;
    inline const mbgl::LatLng& operator()(const std::shared_ptr<const mbgl::MPointAnnotationImpl>& v) const {
        return v->point.position;
    }
};

} // end namespace index
} // end namespace geometry
} // end namespace boost

#endif
