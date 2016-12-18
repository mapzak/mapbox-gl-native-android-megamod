#include <mbgl/annotation/mtn_point_annotation_impl.hpp>
#include <mbgl/annotation/annotation_tile.hpp>

namespace mbgl {

MTNPointAnnotationImpl::MTNPointAnnotationImpl(const AnnotationID id_, const MTNPointAnnotation& point_)
: id(id_),
  point(point_) {
}

void MTNPointAnnotationImpl::updateLayer(const TileID& tileID, AnnotationTileLayer& layer) const {
    std::unordered_map<std::string, std::string> featureProperties;
    auto idx_s = std::to_string(point.idx_i);
    auto nr_s = std::to_string(point.nr_i);
    
    featureProperties.emplace("idx_s", idx_s);
    featureProperties.emplace("nr_s", nr_s);
    featureProperties.emplace("type_s", point.type);
    if(point.idx_i == 0) {
        featureProperties.emplace("first_s", "yes");
    } else {
        featureProperties.emplace("first_s", "no");
    }
    if(point.selected_i == 1) {
        featureProperties.emplace("selected_s", "yes");
    } else {
        featureProperties.emplace("selected_s", "no");
    }
    
    
    auto text = std::string("N/A");
    
    if(point.idx_i == 0) {
        text = "START";
    } else if(point.type == "waypoint") {
        text = std::to_string(point.nr_i);
    } else if(point.type == "pointofinterest") {
        text = "POI";
    }
    featureProperties.emplace("text", text);
    
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
