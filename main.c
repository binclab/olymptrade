#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

GApplication *gapp;
GdkDisplay *display;
GdkMonitor *projector;
GdkSurface *surface;

void activate(GtkApplication* olymptrade)
{
    GtkWidget *window = gtk_application_window_new(olymptrade);
    GtkWidget *webview;
    char path[strlen(g_get_home_dir()) + 24];
    sprintf(path, "%s/.local/share/olymptrade/", g_get_home_dir());
    char *appath = (char *)&path;
    WebKitWebsiteDataManager *manager = webkit_website_data_manager_new("base-data-directory", appath, NULL);
    WebKitWebContext *context = webkit_web_context_new_with_website_data_manager(manager);
    webview = webkit_web_view_new_with_context(context);
    WebKitSettings *settings = webkit_settings_new_with_settings("enable-offline-web-application-cache", TRUE, NULL);
    WebKitCookieManager *cookiejar = webkit_web_context_get_cookie_manager(context);
    webkit_web_view_set_zoom_level((WebKitWebView *)webview, 0.85);
    // g_object_set((GObject *)settings, "enable-offline-web-application-cache", TRUE, NULL);
    webkit_cookie_manager_set_persistent_storage(cookiejar, "storage", WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE);
    webkit_web_view_set_settings((WebKitWebView *)webview, settings);
    webkit_web_view_load_uri((WebKitWebView *)webview, "https://olymptrade.com/platform");
    gtk_widget_set_size_request(window, 1024, 576);
    gtk_window_maximize((GtkWindow *)window);
    gtk_window_set_child((GtkWindow *)window, webview);
    gtk_window_present((GtkWindow *)window);
}

int main(int argc, char *argv[])
{
    int status;
    GtkApplication *olymptrade = gtk_application_new("com.olymptrade.platform", 0);
    gapp = (GApplication *)olymptrade;
    g_signal_connect(olymptrade, "activate", (GCallback)activate, NULL);
    status = g_application_run(gapp, argc, argv);
    g_object_unref(olymptrade);
    return status;
}

void quit()
{
    g_application_quit(gapp);
    g_object_unref(projector);
}