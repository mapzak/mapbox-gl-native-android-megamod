#ifndef MBGL_MAP_MAP
#define MBGL_MAP_MAP

#include <mbgl/util/chrono.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/map/update.hpp>
#include <mbgl/map/mode.hpp>
#include <mbgl/util/geo.hpp>
#include <mbgl/util/noncopyable.hpp>
#include <mbgl/util/vec.hpp>
#include <mbgl/annotation/annotation.hpp>
#include <mbgl/style/types.hpp>

#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace mbgl {

class FileSource;
class View;
class MapData;
class MapContext;
class SpriteImage;
class Transform;
class PointAnnotation;
// modologica BEGIN
class MMRShipPointAnnotation;
class MMRMyShipPointAnnotation;
class MTNPointAnnotation;
class MPointAnnotation;
// modologica END
class ShapeAnnotation;
struct CameraOptions;
struct AnimationOptions;

namespace util {
template <class T> class Thread;
} // namespace util

class Map : private util::noncopyable {
    friend class View;

public:
    explicit Map(View&, FileSource&,
                 MapMode mapMode = MapMode::Continuous,
                 GLContextMode contextMode = GLContextMode::Unique,
                 ConstrainMode constrainMode = ConstrainMode::HeightOnly);
    ~Map();

    // Pauses the render thread. The render thread will stop running but will not be terminated and will not lose state until resumed.
    void pause();
    bool isPaused();

    // Resumes a paused render thread
    void resume();

    // Register a callback that will get called (on the render thread) when all resources have
    // been loaded and a complete render occurs.
    using StillImageCallback = std::function<void (std::exception_ptr, PremultipliedImage&&)>;
    void renderStill(StillImageCallback callback);

    // Triggers a synchronous render.
    void renderSync();

    // Notifies the Map thread that the state has changed and an update might be necessary.
    void update(Update update);

    // Styling
    void addClass(const std::string&);
    void removeClass(const std::string&);
    bool hasClass(const std::string&) const;
    void setClasses(const std::vector<std::string>&);
    std::vector<std::string> getClasses() const;

    void setDefaultFadeDuration(const Duration&);
    Duration getDefaultFadeDuration() const;

    void setDefaultTransitionDuration(const Duration&);
    Duration getDefaultTransitionDuration() const;

    void setDefaultTransitionDelay(const Duration&);
    Duration getDefaultTransitionDelay() const;

    void setStyleURL(const std::string& url);
    void setStyleJSON(const std::string& json, const std::string& base = "");
    std::string getStyleURL() const;
    std::string getStyleJSON() const;

    // Transition
    void cancelTransitions();
    void setGestureInProgress(bool);
    bool isGestureInProgress() const;
    bool isRotating() const;
    bool isScaling() const;
    bool isPanning() const;

    // Camera
    void jumpTo(const CameraOptions&);
    void easeTo(const CameraOptions&, const AnimationOptions&);
    void flyTo(const CameraOptions&, const AnimationOptions&);

    // Position
    void moveBy(const ScreenCoordinate&, const Duration& = Duration::zero());
    void setLatLng(const LatLng&, const ScreenCoordinate&, const Duration& = Duration::zero());
    void setLatLng(const LatLng&, const EdgeInsets&, const Duration& = Duration::zero());
    void setLatLng(const LatLng&, const Duration& = Duration::zero());
    LatLng getLatLng(const EdgeInsets& = {}) const;
    void resetPosition(const EdgeInsets& = {});

    // Scale
    void scaleBy(double ds, const ScreenCoordinate& = { NAN, NAN }, const Duration& = Duration::zero());
    void setScale(double scale, const ScreenCoordinate& = { NAN, NAN }, const Duration& = Duration::zero());
    double getScale() const;
    void setZoom(double zoom, const Duration& = Duration::zero());
    void setZoom(double zoom, const EdgeInsets&, const Duration& = Duration::zero());
    double getZoom() const;
    void setLatLngZoom(const LatLng&, double zoom, const Duration& = Duration::zero());
    void setLatLngZoom(const LatLng&, double zoom, const EdgeInsets&, const Duration& = Duration::zero());
    CameraOptions cameraForLatLngBounds(const LatLngBounds&, const EdgeInsets&);
    CameraOptions cameraForLatLngs(const std::vector<LatLng>&, const EdgeInsets&);
    void resetZoom();
    void setMinZoom(const double minZoom);
    double getMinZoom() const;
    void setMaxZoom(const double maxZoom);
    double getMaxZoom() const;

    // Rotation
    void rotateBy(const ScreenCoordinate& first, const ScreenCoordinate& second, const Duration& = Duration::zero());
    void setBearing(double degrees, const Duration& = Duration::zero());
    void setBearing(double degrees, const ScreenCoordinate&, const Duration& = Duration::zero());
    void setBearing(double degrees, const EdgeInsets&, const Duration& = Duration::zero());
    double getBearing() const;
    void resetNorth(const Duration& = Milliseconds(500));
    void resetNorth(const EdgeInsets&, const Duration& = Milliseconds(500));

    // Pitch
    void setPitch(double pitch, const Duration& = Duration::zero());
    void setPitch(double pitch, const ScreenCoordinate&, const Duration& = Duration::zero());
    double getPitch() const;

    // North Orientation
    void setNorthOrientation(NorthOrientation);
    NorthOrientation getNorthOrientation() const;
    
    // Constrain mode
    void setConstrainMode(ConstrainMode);
    ConstrainMode getConstrainMode() const;

    // Size
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    // Projection
    double getMetersPerPixelAtLatitude(double lat, double zoom) const;
    ProjectedMeters projectedMetersForLatLng(const LatLng&) const;
    LatLng latLngForProjectedMeters(const ProjectedMeters&) const;
    ScreenCoordinate pixelForLatLng(const LatLng&) const;
    LatLng latLngForPixel(const ScreenCoordinate&) const;

    // Annotations
    void addAnnotationIcon(const std::string&, std::shared_ptr<const SpriteImage>);
    void removeAnnotationIcon(const std::string&);
    double getTopOffsetPixelsForAnnotationIcon(const std::string&);

    AnnotationID addPointAnnotation(const PointAnnotation&);
    AnnotationIDs addPointAnnotations(const std::vector<PointAnnotation>&);
    // modologica BEGIN
    AnnotationID addMMRShipPointAnnotation(const MMRShipPointAnnotation&);
    AnnotationIDs addMMRShipPointAnnotations(const std::vector<MMRShipPointAnnotation>&);
    void updateMMRShipPointAnnotation(AnnotationID, const MMRShipPointAnnotation&);
    void updateAnnotations();
    
    AnnotationID addMMRMyShipPointAnnotation(const MMRMyShipPointAnnotation&);
    AnnotationIDs addMMRMyShipPointAnnotations(const std::vector<MMRMyShipPointAnnotation>&);
    void updateMMRMyShipPointAnnotation(AnnotationID, const MMRMyShipPointAnnotation&);

    AnnotationID addMTNPointAnnotation(const MTNPointAnnotation&);
    AnnotationIDs addMTNPointAnnotations(const std::vector<MTNPointAnnotation>&);
    void updateMTNPointAnnotation(AnnotationID, const MTNPointAnnotation&);

    AnnotationID addMPointAnnotation(const MPointAnnotation&);
    AnnotationIDs addMPointAnnotations(const std::vector<MPointAnnotation>&);
    void updateMPointAnnotation(AnnotationID, const MPointAnnotation&);
    // modologica END
    AnnotationID addShapeAnnotation(const ShapeAnnotation&);
    AnnotationIDs addShapeAnnotations(const std::vector<ShapeAnnotation>&);

    void updatePointAnnotation(AnnotationID, const PointAnnotation&);

    void removeAnnotation(AnnotationID);
    void removeAnnotations(const AnnotationIDs&);

    AnnotationIDs getPointAnnotationsInBounds(const LatLngBounds&);

    void addCustomLayer(const std::string& id,
                        CustomLayerInitializeFunction,
                        CustomLayerRenderFunction,
                        CustomLayerDeinitializeFunction,
                        void* context,
                        const char* before = nullptr);
    void removeCustomLayer(const std::string& id);

    // Memory
    void setSourceTileCacheSize(size_t);
    void onLowMemory();

    // Debug
    void setDebug(MapDebugOptions);
    void cycleDebugOptions();
    MapDebugOptions getDebug() const;

    bool isFullyLoaded() const;
    void dumpDebugLogs() const;

private:
    View& view;
    const std::unique_ptr<Transform> transform;
    const std::unique_ptr<util::Thread<MapContext>> context;
    MapData* data;

    enum class RenderState {
        never,
        partial,
        fully
    };

    RenderState renderState = RenderState::never;
};

} // namespace mbgl

#endif
