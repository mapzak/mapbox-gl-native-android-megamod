package com.mapbox.mapboxsdk.annotations;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.View;

import com.mapbox.mapboxsdk.R;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.maps.MapView;
import com.mapbox.mapboxsdk.maps.MapboxMap;

/**
 * Marker is an annotation that shows an icon image at a geographical location.
 * </p>
 * An {@link InfoWindow} can be shown when a Marker is pressed
 * <p/>
 */
public class MPoint extends Marker {

    public LatLng position;
    public String symbol;
    public String snippet;
    public String title;
    private InfoWindow infoWindow = null;
    private boolean infoWindowShown = false;
    private int topOffsetPixels;

    /**
     * Constructor
     */
    MPoint() {
        super();
    }

    public MPoint(BaseMarkerOptions baseMarkerOptions) {
        position = baseMarkerOptions.position;
        snippet = baseMarkerOptions.snippet;
        title = baseMarkerOptions.title;
    }

    public LatLng getPosition() {
        return position;
    }

    public String getSnippet() {
        return snippet;
    }

    public String getTitle() {
        return title;
    }

    public String getSymbol() {
        return symbol;
    }

    /**
     * Do not use this method. Used internally by the SDK.
     */
    public void hideInfoWindow() {
        if (infoWindow != null) {
            infoWindow.close();
        }
        infoWindowShown = false;
    }

    /**
     * Do not use this method. Used internally by the SDK.
     */
    public boolean isInfoWindowShown() {
        return infoWindowShown;
    }

    /**
     * Sets the position.
     *
     * @param position new position
     */
    public void setPosition(LatLng position) {
        this.position = position;
        MapboxMap map = getMapboxMap();
        if (map != null) {
//            map.updateMarker(this);
        }
    }

    void setSnippet(String snippet) {
        this.snippet = snippet;
    }

    void setTitle(String title) {
        this.title = title;
    }

    void setSymbol(String symbol) {
        this.symbol = symbol;
    }

    /**
     * Do not use this method. Used internally by the SDK.
     */
    public void setTopOffsetPixels(int topOffsetPixels) {
        this.topOffsetPixels = topOffsetPixels;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;

        MPoint marker = (MPoint) o;
        return !(getPosition() != null ? !getPosition().equals(marker.getPosition()) : marker.getPosition() != null);
    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + (getPosition() != null ? getPosition().hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return "Marker [position[" + getPosition() + "]]";
    }
}
