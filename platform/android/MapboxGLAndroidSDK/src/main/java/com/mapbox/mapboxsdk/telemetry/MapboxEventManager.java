package com.mapbox.mapboxsdk.telemetry;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.location.Location;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.BatteryManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;
import com.mapbox.mapboxsdk.BuildConfig;
import com.mapbox.mapboxsdk.constants.GeoConstants;
import com.mapbox.mapboxsdk.constants.MapboxConstants;
import com.mapbox.mapboxsdk.location.LocationServices;
import com.mapbox.mapboxsdk.utils.MathUtils;
import org.json.JSONArray;
import org.json.JSONObject;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Hashtable;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;
import java.util.Vector;
import okhttp3.CertificatePinner;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

/**
 * Singleton control center for managing Telemetry Data.
 * Primary access is via MapboxEventManager.getMapboxEventManager()
 */
public class MapboxEventManager {

    private static final String TAG = "MapboxEventManager";

    private static MapboxEventManager mapboxEventManager = null;
// modologica BEGIN

    private MapboxEventManager() {
        super();
    }

    public void initialize(@NonNull Context context, @NonNull String accessToken) {
    }

    public static MapboxEventManager getMapboxEventManager() {
        if (mapboxEventManager == null) {
            mapboxEventManager = new MapboxEventManager();
        }
        return mapboxEventManager;
    }

    private static SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSSZ", Locale.US);

    public static String generateCreateDate() {
        return dateFormat.format(new Date());
    }

    public boolean isTelemetryEnabled() {
        return false;
    }

    public void pushEvent(Hashtable<String, Object> eventWithAttributes) {
    }

    public void setTelemetryEnabled(boolean telemetryEnabled) {

    }
    public void addLocationEvent(Location location) {

    }
    void flushEventsQueueImmediately() {

    }
// modologica END
}
