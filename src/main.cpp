#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <iostream>

int main(int argc, char *argv[]) {
    ;//init GStreamer
    gst_init(&argc, &argv);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GstRTSPServer *server = gst_rtsp_server_new();
    
   
    gst_rtsp_server_set_address(server, "0.0.0.0");
    gst_rtsp_server_set_service(server, "8554");

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
    
  
    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    //convert h264 to h265
   gst_rtsp_media_factory_set_launch(factory,
    "( filesrc location=\"C:/Users/NAM NGUYEN/Desktop/DACN/vid/PPE_Part1_HEVC.mp4\" ! "
    "qtdemux ! h265parse ! "
    "rtph265pay name=pay0 pt=96 config-interval=1 aggregate-mode=zero-latency mtu=1400 ! "
    "sink. )");


    gst_rtsp_media_factory_set_shared(factory, TRUE);

   
    gst_rtsp_mount_points_add_factory(mounts, "/live_ppe", factory);
    g_object_unref(mounts);

    if (gst_rtsp_server_attach(server, NULL) == 0) {
        std::cerr << "Error cannot connect RTSP Server!" << std::endl;
        return -1;
    }

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "IP Camera Server is running!" << std::endl;
    std::cout << "Address: rtsp://127.0.0.1:8554/live_ppe" << std::endl;
    std::cout << "Press Ctrl + C to stop." << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    g_main_loop_run(loop);

    return 0;
}