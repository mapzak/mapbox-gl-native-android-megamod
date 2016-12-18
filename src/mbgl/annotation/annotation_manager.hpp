#ifndef MBGL_ANNOTATION_MANAGER
#define MBGL_ANNOTATION_MANAGER

#include <mbgl/annotation/annotation.hpp>
// modologica BEGIN
#include <mbgl/annotation/mmr_ship_point_annotation_impl.hpp>
#include <mbgl/annotation/mmr_my_ship_point_annotation_impl.hpp>
#include <mbgl/annotation/mtn_point_annotation_impl.hpp>
#include <mbgl/annotation/m_point_annotation_impl.hpp>
// modologica END
#include <mbgl/annotation/point_annotation_impl.hpp>
#include <mbgl/annotation/shape_annotation_impl.hpp>
#include <mbgl/sprite/sprite_store.hpp>
#include <mbgl/sprite/sprite_atlas.hpp>
#include <mbgl/util/geo.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <string>
#include <vector>
#include <set>

namespace mbgl {

// modologica BEGIN
class MMRShipPointAnnotation;
class MMRMyShipPointAnnotation;
class MTNPointAnnotation;
class MPointAnnotation;
// modologica END
class PointAnnotation;
class ShapeAnnotation;
class AnnotationTile;
class AnnotationTileMonitor;
class Style;

class AnnotationManager : private util::noncopyable {
public:
    AnnotationManager(float pixelRatio);
    ~AnnotationManager();
    // modologica BEGIN
    AnnotationIDs addMMRShipPointAnnotations(const std::vector<MMRShipPointAnnotation>&, const uint8_t maxZoom);
    void updateMMRShipPointAnnotation(const AnnotationID&, const MMRShipPointAnnotation&, const uint8_t maxZoom);
    AnnotationIDs addMMRMyShipPointAnnotations(const std::vector<MMRMyShipPointAnnotation>&, const uint8_t maxZoom);
    void updateMMRMyShipPointAnnotation(const AnnotationID&, const MMRMyShipPointAnnotation&, const uint8_t maxZoom);
    AnnotationIDs addMTNPointAnnotations(const std::vector<MTNPointAnnotation>&, const uint8_t maxZoom);
    void updateMTNPointAnnotation(const AnnotationID&, const MTNPointAnnotation&, const uint8_t maxZoom);
    AnnotationIDs addMPointAnnotations(const std::vector<MPointAnnotation>&, const uint8_t maxZoom);
    void updateMPointAnnotation(const AnnotationID&, const MPointAnnotation&, const uint8_t maxZoom);
    // modologica END

    AnnotationIDs addPointAnnotations(const std::vector<PointAnnotation>&, const uint8_t maxZoom);
    AnnotationIDs addShapeAnnotations(const std::vector<ShapeAnnotation>&, const uint8_t maxZoom);
    void updatePointAnnotation(const AnnotationID&, const PointAnnotation&, const uint8_t maxZoom);
    void removeAnnotations(const AnnotationIDs&);

    AnnotationIDs getPointAnnotationsInBounds(const LatLngBounds&) const;

    void addIcon(const std::string& name, std::shared_ptr<const SpriteImage>);
    void removeIcon(const std::string& name);
    double getTopOffsetPixelsForIcon(const std::string& name);
    SpriteAtlas& getSpriteAtlas() { return spriteAtlas; }

    void updateStyle(Style&);

    void addTileMonitor(AnnotationTileMonitor&);
    void removeTileMonitor(AnnotationTileMonitor&);

    static const std::string SourceID;
    static const std::string PointLayerID;
    // modologica BEGIN
    static const std::string MMRShipPointSourceID;
    static const std::string MMRShipPointLayerID;
    static const std::string MMRMyShipPointSourceID;
    static const std::string MMRMyShipPointLayerID;
    static const std::string MTNPointSourceID;
    static const std::string MTNPointLayerID;
    static const std::string MPointSourceID;
    static const std::string MPointLayerID;
    // modologica END

private:
    std::unique_ptr<AnnotationTile> getTile(const TileID&);

    AnnotationID nextID = 0;
    // modologica BEGIN
    MMRShipPointAnnotationImpl::Tree mmrShipPointTree;
    MMRShipPointAnnotationImpl::Map mmrShipPointAnnotations;
    MMRMyShipPointAnnotationImpl::Tree mmrMyShipPointTree;
    MMRMyShipPointAnnotationImpl::Map mmrMyShipPointAnnotations;
    MTNPointAnnotationImpl::Tree mtnPointTree;
    MTNPointAnnotationImpl::Map mtnPointAnnotations;
    MPointAnnotationImpl::Tree mPointTree;
    MPointAnnotationImpl::Map mPointAnnotations;
    // modologica END
    PointAnnotationImpl::Tree pointTree;
    PointAnnotationImpl::Map pointAnnotations;
    ShapeAnnotationImpl::Map shapeAnnotations;
    std::vector<std::string> obsoleteShapeAnnotationLayers;
    std::set<AnnotationTileMonitor*> monitors;

    SpriteStore spriteStore;
    SpriteAtlas spriteAtlas;
};

} // namespace mbgl

#endif
