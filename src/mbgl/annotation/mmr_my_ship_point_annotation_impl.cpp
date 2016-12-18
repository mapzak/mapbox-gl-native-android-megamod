#include <mbgl/annotation/mmr_my_ship_point_annotation_impl.hpp>
#include <mbgl/annotation/annotation_tile.hpp>

namespace mbgl {

MMRMyShipPointAnnotationImpl::MMRMyShipPointAnnotationImpl(const AnnotationID id_, const MMRMyShipPointAnnotation& ship_)
: id(id_),
  ship(ship_) {
}

void MMRMyShipPointAnnotationImpl::updateLayer(const TileID& tileID, AnnotationTileLayer& layer) const {
    std::unordered_map<std::string, std::string> featureProperties;
    
    if(ship.type.empty()) {
        featureProperties.emplace("sprite", std::string("mmr-ship-unspecified-moving-100"));
    } else {

        bool moving = false;
        auto moving_str = moving ? std::string("-moving"): std::string("-moored");
        
        auto sprite = (std::string("mmr-ship-") + ship.type + moving_str + std::string("-100"));

        featureProperties.emplace("sprite", sprite);
    }
    
    featureProperties.emplace("rot_i", std::to_string(ship.rot_i));
    featureProperties.emplace("sog_i", std::to_string(ship.sog_i));
    featureProperties.emplace("cog_i", std::to_string(ship.cog_i));
    
    featureProperties.emplace("rotation_degrees_i", std::to_string(int(ship.cog_i / 10.0 / 10.0)));
    
    mbgl::ScreenCoordinate projected = ship.position.project();
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
