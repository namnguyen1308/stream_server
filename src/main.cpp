#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <iostream>


static void on_eos(GstBus *bus, GstMessage *msg, GstElement *pipeline) {
    // Thực hiện lệnh quay lại vị trí 0
    gboolean res = gst_element_seek_simple(pipeline, GST_FORMAT_TIME,
        (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), 0);
    
    if (res) {
        std::cout << ">> [Phát lại] Video đã quay về 0s." << std::endl;
    } else {
        std::cerr << ">> [Lỗi] Không thể quay lại đầu video." << std::endl;
    }
}

static void on_media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data) {
    GstElement *element = gst_rtsp_media_get_element(media);
    GstBus *bus = gst_element_get_bus(element);
    
    if (bus) {
        gst_bus_add_signal_watch(bus);
       
        g_signal_connect(bus, "message::eos", (GCallback)on_eos, element);
        gst_object_unref(bus);
    }
    
}

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstRTSPServer *server = gst_rtsp_server_new();
    
    gst_rtsp_server_set_address(server, "0.0.0.0");
    gst_rtsp_server_set_service(server, "8554");

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    // Pipeline HEVC
    gst_rtsp_media_factory_set_launch(factory,
        "( filesrc location=\"C:/Users/NAM NGUYEN/Desktop/DACN/vid/PPE_Part1_HEVC.mp4\" ! "
        "qtdemux ! h265parse ! rtph265pay name=pay0 pt=96 config-interval=1 aggregate-mode=zero-latency mtu=1400 )");

    gst_rtsp_media_factory_set_shared(factory, TRUE);
    g_signal_connect(factory, "media-configure", (GCallback)on_media_configure, NULL);

    gst_rtsp_mount_points_add_factory(mounts, "/live_ppe", factory);
    g_object_unref(mounts);

    if (gst_rtsp_server_attach(server, NULL) == 0) {
        std::cerr << "CRITICAL: Cannot bind to port 8554! Check if it's already in use." << std::endl;
        return -1;
    }

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "IP Camera Server is LIVE at: rtsp://127.0.0.1:8554/live_ppe" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    g_main_loop_run(loop);
    return 0;
}
