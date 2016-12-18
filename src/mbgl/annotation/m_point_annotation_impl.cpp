#include <mbgl/annotation/m_point_annotation_impl.hpp>
#include <mbgl/annotation/annotation_tile.hpp>

namespace mbgl {

MPointAnnotationImpl::MPointAnnotationImpl(const AnnotationID id_, const MPointAnnotation& point_)
: id(id_),
  point(point_) {
}

void MPointAnnotationImpl::updateLayer(const TileID& tileID, AnnotationTileLayer& layer) const {
    std::unordered_map<std::string, std::string> featureProperties;

    featureProperties.emplace("symbol", point.symbol);
    
    mbgl::ScreenCoordinate projected = point.position.project();
    projected *= 1 << tileID.z;
    projected.x = std::fmod(projected.x, 1);
    projected.y = std::fmod(projected.y, 1);
    projected *= GeometryTileFeature::defaultExtent;

    layer.features.emplace_back(
        std::make_shared<const AnnotationTileFeature>(FeatureType::Point,
                                                      GeometryCollection {{ {{ GeometryCoordinate { projected } }} }},
                                                      featureProperties));
}

} // namespace mbgl
