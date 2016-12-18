#include <mbgl/annotation/annotation_manager.hpp>
#include <mbgl/annotation/annotation_tile.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/layer/symbol_layer.hpp>

#include <boost/function_output_iterator.hpp>

// modologica BEGIN
#include <boost/range/irange.hpp>
#include <mbgl/style/style_parser.hpp>
// modologica END

namespace mbgl {

const std::string AnnotationManager::SourceID = "com.mapbox.annotations";
const std::string AnnotationManager::PointLayerID = "com.mapbox.annotations.points";
// modologica BEGIN
const std::string AnnotationManager::MPointSourceID = "com.mapbox.annotations.mpoints";
const std::string AnnotationManager::MPointLayerID = "com.mapbox.annotations.mpoints";
const std::string AnnotationManager::MTNPointSourceID = "com.mapbox.annotations.mtnpoints";
const std::string AnnotationManager::MTNPointLayerID = "com.mapbox.annotations.mtnpoints";
const std::string AnnotationManager::MMRShipPointSourceID = "com.mapbox.annotations.mmrshippoints";
const std::string AnnotationManager::MMRShipPointLayerID = "com.mapbox.annotations.mmrshippoints";
const std::string AnnotationManager::MMRMyShipPointSourceID = "com.mapbox.annotations.mmrmyshippoints";
const std::string AnnotationManager::MMRMyShipPointLayerID = "com.mapbox.annotations.mmrmyshippoints";
// modologica END

AnnotationManager::AnnotationManager(float pixelRatio)
    : spriteStore(pixelRatio),
      spriteAtlas(1024, 1024, pixelRatio, spriteStore) {
}

AnnotationManager::~AnnotationManager() = default;

// modologica BEGIN
AnnotationIDs
AnnotationManager::addMMRShipPointAnnotations(const std::vector<MMRShipPointAnnotation>& points, const uint8_t) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(points.size());

    for (const auto& point: points) {
        const uint32_t annotationID = nextID++;
        auto annotation = std::make_shared<MMRShipPointAnnotationImpl>(annotationID, point);
        mmrShipPointTree.insert(annotation);
        mmrShipPointAnnotations.emplace(annotationID, annotation);
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

void AnnotationManager::updateMMRShipPointAnnotation(const AnnotationID& id, const MMRShipPointAnnotation& point, const uint8_t) {
    auto foundAnnotation = mmrShipPointAnnotations.find(id);
    if (foundAnnotation != mmrShipPointAnnotations.end()) {
        auto updatedAnnotation = std::make_shared<MMRShipPointAnnotationImpl>(id, point);
        mmrShipPointTree.remove(foundAnnotation->second);
        mmrShipPointTree.insert(updatedAnnotation);
        foundAnnotation->second = updatedAnnotation;
    }
}

AnnotationIDs
AnnotationManager::addMMRMyShipPointAnnotations(const std::vector<MMRMyShipPointAnnotation>& points, const uint8_t) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(points.size());

    for (const auto& point: points) {
        const uint32_t annotationID = nextID++;
        auto annotation = std::make_shared<MMRMyShipPointAnnotationImpl>(annotationID, point);
        mmrMyShipPointTree.insert(annotation);
        mmrMyShipPointAnnotations.emplace(annotationID, annotation);
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

void AnnotationManager::updateMMRMyShipPointAnnotation(const AnnotationID& id, const MMRMyShipPointAnnotation& point, const uint8_t) {
    auto foundAnnotation = mmrMyShipPointAnnotations.find(id);
    if (foundAnnotation != mmrMyShipPointAnnotations.end()) {
        auto updatedAnnotation = std::make_shared<MMRMyShipPointAnnotationImpl>(id, point);
        mmrMyShipPointTree.remove(foundAnnotation->second);
        mmrMyShipPointTree.insert(updatedAnnotation);
        foundAnnotation->second = updatedAnnotation;
    }
}

AnnotationIDs
AnnotationManager::addMTNPointAnnotations(const std::vector<MTNPointAnnotation>& points, const uint8_t) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(points.size());

    for (const auto& point: points) {
        const uint32_t annotationID = nextID++;
        auto annotation = std::make_shared<MTNPointAnnotationImpl>(annotationID, point);
        mtnPointTree.insert(annotation);
        mtnPointAnnotations.emplace(annotationID, annotation);
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

void AnnotationManager::updateMTNPointAnnotation(const AnnotationID& id, const MTNPointAnnotation& point, const uint8_t) {
    auto foundAnnotation = mtnPointAnnotations.find(id);
    if (foundAnnotation != mtnPointAnnotations.end()) {
        auto updatedAnnotation = std::make_shared<MTNPointAnnotationImpl>(id, point);
        mtnPointTree.remove(foundAnnotation->second);
        mtnPointTree.insert(updatedAnnotation);
        foundAnnotation->second = updatedAnnotation;
    }
}

AnnotationIDs
AnnotationManager::addMPointAnnotations(const std::vector<MPointAnnotation>& points, const uint8_t) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(points.size());

    for (const auto& point: points) {
        const uint32_t annotationID = nextID++;
        auto annotation = std::make_shared<MPointAnnotationImpl>(annotationID, point);
        mPointTree.insert(annotation);
        mPointAnnotations.emplace(annotationID, annotation);
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

void AnnotationManager::updateMPointAnnotation(const AnnotationID& id, const MPointAnnotation& point, const uint8_t) {
    auto foundAnnotation = mPointAnnotations.find(id);
    if (foundAnnotation != mPointAnnotations.end()) {
        auto updatedAnnotation = std::make_shared<MPointAnnotationImpl>(id, point);
        mPointTree.remove(foundAnnotation->second);
        mPointTree.insert(updatedAnnotation);
        foundAnnotation->second = updatedAnnotation;
    }
}
// modologica END
AnnotationIDs
AnnotationManager::addPointAnnotations(const std::vector<PointAnnotation>& points, const uint8_t) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(points.size());

    for (const auto& point : points) {
        const uint32_t annotationID = nextID++;
        auto annotation = std::make_shared<PointAnnotationImpl>(annotationID, point);
        pointTree.insert(annotation);
        pointAnnotations.emplace(annotationID, annotation);
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

AnnotationIDs
AnnotationManager::addShapeAnnotations(const std::vector<ShapeAnnotation>& shapes, const uint8_t maxZoom) {
    AnnotationIDs annotationIDs;
    annotationIDs.reserve(shapes.size());

    for (const auto& shape : shapes) {
        const uint32_t annotationID = nextID++;
        shapeAnnotations.emplace(annotationID,
            std::make_unique<ShapeAnnotationImpl>(annotationID, shape, maxZoom));
        annotationIDs.push_back(annotationID);
    }

    return annotationIDs;
}

void AnnotationManager::updatePointAnnotation(const AnnotationID& id, const PointAnnotation& point, const uint8_t) {
    auto foundAnnotation = pointAnnotations.find(id);
    if (foundAnnotation != pointAnnotations.end()) {
        auto updatedAnnotation = std::make_shared<PointAnnotationImpl>(id, point);
        pointTree.remove(foundAnnotation->second);
        pointTree.insert(updatedAnnotation);
        foundAnnotation->second = updatedAnnotation;
    }
}

void AnnotationManager::removeAnnotations(const AnnotationIDs& ids) {
    for (const auto& id : ids) {
        if (pointAnnotations.find(id) != pointAnnotations.end()) {
            pointTree.remove(pointAnnotations.at(id));
            pointAnnotations.erase(id);
        } else if (shapeAnnotations.find(id) != shapeAnnotations.end()) {
            obsoleteShapeAnnotationLayers.push_back(shapeAnnotations.at(id)->layerID);
            shapeAnnotations.erase(id);
        }
        // modologica BEGIN
        else if (mmrShipPointAnnotations.find(id) != mmrShipPointAnnotations.end()) {
            mmrShipPointTree.remove(mmrShipPointAnnotations.at(id));
            mmrShipPointAnnotations.erase(id);
        } else if (mmrMyShipPointAnnotations.find(id) != mmrMyShipPointAnnotations.end()) {
            mmrMyShipPointTree.remove(mmrMyShipPointAnnotations.at(id));
            mmrMyShipPointAnnotations.erase(id);
        } else if (mtnPointAnnotations.find(id) != mtnPointAnnotations.end()) {
            mtnPointTree.remove(mtnPointAnnotations.at(id));
            mtnPointAnnotations.erase(id);
        } else if (mPointAnnotations.find(id) != mPointAnnotations.end()) {
            mPointTree.remove(mPointAnnotations.at(id));
            mPointAnnotations.erase(id);
        }
        // modologica END
    }
}

AnnotationIDs AnnotationManager::getPointAnnotationsInBounds(const LatLngBounds& bounds) const {
    AnnotationIDs result;

    pointTree.query(boost::geometry::index::intersects(bounds),
        boost::make_function_output_iterator([&](const auto& val){
            result.push_back(val->id);
        }));
    // modologica BEGIN
    mmrShipPointTree.query(boost::geometry::index::intersects(bounds),
                    boost::make_function_output_iterator([&](const auto& val){
        result.push_back(val->id);
    }));

    mmrMyShipPointTree.query(boost::geometry::index::intersects(bounds),
                           boost::make_function_output_iterator([&](const auto& val){
        result.push_back(val->id);
    }));

    mtnPointTree.query(boost::geometry::index::intersects(bounds),
                           boost::make_function_output_iterator([&](const auto& val){
        result.push_back(val->id);
    }));

    mPointTree.query(boost::geometry::index::intersects(bounds),
                           boost::make_function_output_iterator([&](const auto& val){
        result.push_back(val->id);
    }));
    // modologica END
    return result;
}

std::unique_ptr<AnnotationTile> AnnotationManager::getTile(const TileID& tileID) {
    if (pointAnnotations.empty() && shapeAnnotations.empty()
        // modologica BEGIN
        && mmrShipPointAnnotations.empty()
        && mmrMyShipPointAnnotations.empty()
        && mtnPointAnnotations.empty()
        && mPointAnnotations.empty()
        // modologica END
        )
        return nullptr;

    auto tile = std::make_unique<AnnotationTile>();

    AnnotationTileLayer& pointLayer = *tile->layers.emplace(
        PointLayerID,
        std::make_unique<AnnotationTileLayer>()).first->second;

    LatLngBounds tileBounds(tileID);

    pointTree.query(boost::geometry::index::intersects(tileBounds),
        boost::make_function_output_iterator([&](const auto& val){
            val->updateLayer(tileID, pointLayer);
        }));
    // modologica BEGIN
    AnnotationTileLayer& mmrShipPointLayer = *tile->layers.emplace(MMRShipPointLayerID,
                                                            std::make_unique<AnnotationTileLayer>()).first->second;

    AnnotationTileLayer& mmrMyShipPointLayer = *tile->layers.emplace(MMRMyShipPointLayerID,
                                                                   std::make_unique<AnnotationTileLayer>()).first->second;

    AnnotationTileLayer& mtnPointLayer = *tile->layers.emplace(MTNPointLayerID,
                                                                   std::make_unique<AnnotationTileLayer>()).first->second;

    AnnotationTileLayer& mPointLayer = *tile->layers.emplace(MPointLayerID,
                                                                   std::make_unique<AnnotationTileLayer>()).first->second;

    mmrShipPointTree.query(boost::geometry::index::intersects(tileBounds),
                               boost::make_function_output_iterator([&](const auto& val){
        val->updateLayer(tileID, mmrShipPointLayer);
    }));

    mmrMyShipPointTree.query(boost::geometry::index::intersects(tileBounds),
                                 boost::make_function_output_iterator([&](const auto& val){
        val->updateLayer(tileID, mmrMyShipPointLayer);
    }));

    mtnPointTree.query(boost::geometry::index::intersects(tileBounds),
                           boost::make_function_output_iterator([&](const auto& val){
        val->updateLayer(tileID, mtnPointLayer);
    }));

    mPointTree.query(boost::geometry::index::intersects(tileBounds),
                           boost::make_function_output_iterator([&](const auto& val){
        val->updateLayer(tileID, mPointLayer);
    }));
    // modologica END

    for (const auto& shape : shapeAnnotations) {
        shape.second->updateTile(tileID, *tile);
    }

    return tile;
}

void AnnotationManager::updateStyle(Style& style) {
    // Create annotation source, point layer, and point bucket
    // modologica BEGIN
    if (!style.getSource(SourceID) && !style.use_m) {
    // modologica END
        std::unique_ptr<Source> source = std::make_unique<Source>(SourceType::Annotations, SourceID, "", util::tileSize, std::make_unique<SourceInfo>(), nullptr);
        source->enabled = true;
        style.addSource(std::move(source));

        std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
        layer->id = PointLayerID;
        layer->source = SourceID;
        layer->sourceLayer = PointLayerID;
        layer->layout.icon.image = std::string("{sprite}");
        layer->layout.icon.allowOverlap = true;
        layer->spriteAtlas = &spriteAtlas;

        style.addLayer(std::move(layer));
    }
    // modologica BEGIN
    if (!style.getSource(MMRShipPointSourceID) && style.use_mmr) {
        std::unique_ptr<Source> source = std::make_unique<Source>(SourceType::Annotations, MMRShipPointSourceID, "", util::tileSize, std::make_unique<SourceInfo>(), nullptr);
        source->enabled = true;
        style.addSource(std::move(source));

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> size_doc;
        size_doc.Parse<0>(std::string("{\"size\": {\"base\": 1.3, \"stops\": [[13, 0.1], [16, 1.0]]}}").c_str());

        for(auto i : boost::irange(0, 36)) {
            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MMRShipPointLayerID + std::to_string(i);
            layer->source = MMRShipPointSourceID;
            layer->sourceLayer = MMRShipPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image = std::string("{sprite}");
            layer->layout.icon.rotate = static_cast<float>(i*10);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Map;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.icon.size.parse("size", size_doc);

            /*
            layer->layout.text.size = 13.0f;
            layer->layout.text.anchor = TextAnchorType::Left;
            layer->layout.text.field = std::string("rot {rot_i}\n sog {sog_i}\n cog {cog_i}");
            */

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"==\", \"rotation_degrees_i\", \"") + std::to_string(i) + std::string("\"]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);

            style.addLayer(std::move(layer));
        }
    }

    if (!style.getSource(MMRMyShipPointSourceID) && style.use_mmr) {
        std::unique_ptr<Source> source = std::make_unique<Source>(SourceType::Annotations, MMRMyShipPointSourceID, "", util::tileSize, std::make_unique<SourceInfo>(), nullptr);
        source->enabled = true;
        style.addSource(std::move(source));

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> size_doc;
        size_doc.Parse<0>(std::string("{\"size\": {\"base\": 1, \"stops\": [[13, 1.0], [16, 1.0]]}}").c_str());
        /*
        {
            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MMRMyShipPointLayerID + std::string("mmr-compass");
            layer->source = MMRMyShipPointSourceID;
            layer->sourceLayer = MMRMyShipPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.iconImage = std::string("compass");
            layer->layout.iconRotate = 0;
            layer->layout.iconRotationAlignment = RotationAlignmentType::Map;
            layer->layout.iconIgnorePlacement = true;
            layer->layout.iconAllowOverlap = true;
            layer->layout.iconSize.parse("size", size_doc);

            style.addLayer(std::move(layer));
        }
        */
        for(auto i : boost::irange(0, 36)) {

            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MMRMyShipPointLayerID + std::to_string(i);
            layer->source = MMRMyShipPointSourceID;
            layer->sourceLayer = MMRMyShipPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image = std::string("mmr-compass-simple"); // direction
            layer->layout.icon.rotate = static_cast<float>(i*10);
            layer->layout.icon.size.parse("size",size_doc);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Map;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.icon.size.parse("size", size_doc);
//            layer->layout.iconOffset = {{ 0, -((256.0-(47.0/2.0)) / 2.0) }};

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"==\", \"rotation_degrees_i\", \"") + std::to_string(i) + std::string("\"]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);

            style.addLayer(std::move(layer));
        }
    }

    if (!style.getSource(MTNPointSourceID) && style.use_mtn) {
        std::unique_ptr<Source> source = std::make_unique<Source>(SourceType::Annotations, MTNPointSourceID, "", util::tileSize, std::make_unique<SourceInfo>(), nullptr);
        source->enabled = true;
        style.addSource(std::move(source));


        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> icon_opacity_doc;
        icon_opacity_doc.Parse<0>(std::string("{\"icon-opacity\": {\"base\": 1, \"stops\": [[10, 0], [10.1, 1]]}}").c_str());

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_opacity_doc;
        text_opacity_doc.Parse<0>(std::string("{\"text-opacity\": {\"base\": 1, \"stops\": [[10, 0], [10.1, 1]]}}").c_str());

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_font_doc;
        text_font_doc.Parse<0>(std::string("{\"text-font\": [\"Open Sans Bold\"]}").c_str());

        // MTN >0 normal
        {
            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_size_doc;
            text_size_doc.Parse<0>(std::string("{\"text-size\": {\"base\": 1, \"stops\": [[14, 10], [16, 14], [20, 16]]}}").c_str());

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> image_doc;
            image_doc.Parse<0>(std::string("{\"image\": {\"base\": 1, \"stops\": [[0, \"route-marker-10-normal\"], [12, \"route-marker-20-normal\"], [14, \"route-marker-30-normal\"], [16, \"route-marker-40-normal\"]]}}").c_str());

            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MTNPointLayerID + "-idxgtezero-normal";
            layer->source = MTNPointSourceID;
            layer->sourceLayer = MTNPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image.parse("image", image_doc);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Viewport;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.text.allowOverlap = true;
            layer->layout.text.ignorePlacement = true;
            layer->layout.text.size.parse("text-size", text_size_doc);
            layer->layout.text.anchor = TextAnchorType::Center;
            layer->layout.text.field = std::string("{text}");
            layer->layout.text.font.parse("text-font", text_font_doc);
            layer->layout.icon.optional = true;
            layer->paint.text.color = {{ 0.2, 0.2, 0.2, 1 }};
            layer->paint.text.haloColor = {{ 1, 1, 1, 1 }};
            layer->paint.text.haloWidth = 1.0f;
            layer->paint.icon.opacity.parse("icon-opacity", icon_opacity_doc);
            layer->paint.text.opacity.parse("text-opacity", text_opacity_doc);

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"all\", [\"==\", \"selected_s\", \"no\"], [\"==\", \"first_s\", \"no\"]]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);

            style.addLayer(std::move(layer));
        }

        // MTN 0 normal
        {
            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> image_doc;
            image_doc.Parse<0>(std::string("{\"image\": \"route-marker-40-normal\"}").c_str());

            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MTNPointLayerID + "-idxzero-normal";
            layer->source = MTNPointSourceID;
            layer->sourceLayer = MTNPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image.parse("image", image_doc);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Viewport;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.text.allowOverlap = true;
            layer->layout.text.ignorePlacement = true;
            layer->layout.text.size = 10.0f;
            layer->layout.text.anchor = TextAnchorType::Center;
            layer->layout.text.field = std::string("{text}");
            layer->layout.text.font.parse("text-font", text_font_doc);

            layer->paint.text.color = {{ 0.2, 0.2, 0.2, 1 }};
            layer->paint.text.haloColor = {{ 1, 1, 1, 1 }};
            layer->paint.text.haloWidth = 1.0f;

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"all\", [\"==\", \"selected_s\", \"no\"], [\"==\", \"first_s\", \"yes\"]]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);
            style.addLayer(std::move(layer));
        }

        // MTN >0 selected
        {
            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_size_doc;
            text_size_doc.Parse<0>(std::string("{\"text-size\": {\"base\": 1, \"stops\": [[14, 12], [16, 16], [20, 18]]}}").c_str());

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> image_doc;
            image_doc.Parse<0>(std::string("{\"image\": {\"base\": 1, \"stops\": [[0, \"route-marker-10-selected\"], [10, \"route-marker-20-selected\"], [12, \"route-marker-30-selected\"], [14, \"route-marker-40-selected\"]]}}").c_str());

            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MTNPointLayerID + "-idxgtezero-selected";
            layer->source = MTNPointSourceID;
            layer->sourceLayer = MTNPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image.parse("image", image_doc);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Viewport;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.text.allowOverlap = true;
            layer->layout.text.ignorePlacement = true;
            layer->layout.text.size.parse("text-size", text_size_doc);
            layer->layout.text.anchor = TextAnchorType::Center;
            layer->layout.text.field = std::string("{text}");
            layer->layout.text.font.parse("text-font", text_font_doc);

            layer->paint.text.color = {{ 1, 1, 1, 1 }};
            layer->paint.text.haloColor = {{ 0.2, 0.2, 0.2, 1 }};
            layer->paint.text.haloWidth = 0.5f;
            layer->paint.icon.opacity.parse("icon-opacity", icon_opacity_doc);
            layer->paint.text.opacity.parse("text-opacity", text_opacity_doc);

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"all\", [\"==\", \"selected_s\", \"yes\"], [\"==\", \"first_s\", \"no\"]]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);

            style.addLayer(std::move(layer));
        }

        // MTN 0 selected
        {
            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> image_doc;
            image_doc.Parse<0>(std::string("{\"image\": \"route-marker-40-selected\"}").c_str());

            std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
            layer->id = MTNPointLayerID + "-idxzero-selected";
            layer->source = MTNPointSourceID;
            layer->sourceLayer = MTNPointLayerID;
            layer->spriteAtlas = style.spriteAtlas.get();
            layer->layout.icon.image.parse("image", image_doc);
            layer->layout.icon.rotationAlignment = RotationAlignmentType::Viewport;
            layer->layout.icon.ignorePlacement = true;
            layer->layout.icon.allowOverlap = true;
            layer->layout.text.allowOverlap = true;
            layer->layout.text.ignorePlacement = true;
            layer->layout.text.size = 10.0f;
            layer->layout.text.anchor = TextAnchorType::Center;
            layer->layout.text.field = std::string("{text}");
            layer->layout.text.font.parse("text-font", text_font_doc);

            layer->paint.text.color = {{ 1, 1, 1, 1 }};
            layer->paint.text.haloColor = {{ 0.2, 0.2, 0.2, 1 }};
            layer->paint.text.haloWidth = 0.5f;

            rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> doc;
            auto e = std::string("[\"all\", [\"==\", \"selected_s\", \"yes\"], [\"==\", \"first_s\", \"yes\"]]");
            doc.Parse<0>(e.c_str());
            layer->filter = parseFilterExpression(doc);

            style.addLayer(std::move(layer));
        }
    }



    if (!style.getSource(MPointSourceID) && style.use_m) {
        std::unique_ptr<Source> source = std::make_unique<Source>(SourceType::Annotations, MPointSourceID, "", util::tileSize, std::make_unique<SourceInfo>(), nullptr);
        source->enabled = true;
        style.addSource(std::move(source));

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_size_doc;
        text_size_doc.Parse<0>(std::string("{\"text-size\": {\"base\": 1, \"stops\": [[14, 6], [16, 11], [20, 16]]}}").c_str());

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> icon_opacity_doc;
        icon_opacity_doc.Parse<0>(std::string("{\"icon-opacity\": {\"base\": 1, \"stops\": [[10, 0], [10.1, 1]]}}").c_str());

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> text_opacity_doc;
        text_opacity_doc.Parse<0>(std::string("{\"text-opacity\": {\"base\": 1, \"stops\": [[10, 0], [10.1, 1]]}}").c_str());

        rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> size_doc;
        size_doc.Parse<0>(std::string("{\"size\": {\"base\": 1.3, \"stops\": [[10, 0.3], [16, 0.5]]}}").c_str());

        std::unique_ptr<SymbolLayer> layer = std::make_unique<SymbolLayer>();
        layer->id = MPointLayerID;
        layer->source = MPointSourceID;
        layer->sourceLayer = MPointLayerID;
        layer->spriteAtlas = style.spriteAtlas.get();
        layer->layout.icon.image = std::string("{symbol}");
        layer->layout.icon.rotationAlignment = RotationAlignmentType::Viewport;
        layer->layout.icon.ignorePlacement = true;
        layer->layout.icon.allowOverlap = false;
        layer->layout.icon.size.parse("size", size_doc);

        layer->layout.text.size.parse("text-size", text_size_doc);
        layer->layout.text.anchor = TextAnchorType::Center;
        layer->layout.text.field = std::string("{text}");
        layer->paint.text.color = {{ 0.2, 0.2, 0.2, 1 }};
        layer->paint.text.haloColor = {{ 1, 1, 1, 1 }};
        layer->paint.text.haloWidth = 2.0f;
        layer->paint.icon.opacity.parse("icon-opacity", icon_opacity_doc);
        layer->paint.text.opacity.parse("text-opacity", text_opacity_doc);

        style.addLayer(std::move(layer));

    }
    // modologica END

    for (const auto& shape : shapeAnnotations) {
        shape.second->updateStyle(style);
    }

    for (const auto& layer : obsoleteShapeAnnotationLayers) {
        if (style.getLayer(layer)) {
            style.removeLayer(layer);
        }
    }

    obsoleteShapeAnnotationLayers.clear();

    for (auto& monitor : monitors) {
        monitor->update(getTile(monitor->tileID));
    }
}

void AnnotationManager::addTileMonitor(AnnotationTileMonitor& monitor) {
    monitors.insert(&monitor);
    monitor.update(getTile(monitor.tileID));
}

void AnnotationManager::removeTileMonitor(AnnotationTileMonitor& monitor) {
    monitors.erase(&monitor);
}

void AnnotationManager::addIcon(const std::string& name, std::shared_ptr<const SpriteImage> sprite) {
    spriteStore.setSprite(name, sprite);
    spriteAtlas.updateDirty();
}
    
void AnnotationManager::removeIcon(const std::string& name) {
    spriteStore.removeSprite(name);
    spriteAtlas.updateDirty();
}

double AnnotationManager::getTopOffsetPixelsForIcon(const std::string& name) {
    auto sprite = spriteStore.getSprite(name);
    return sprite ? -(sprite->image.height / sprite->pixelRatio) / 2 : 0;
}

} // namespace mbgl
