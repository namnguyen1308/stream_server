# PPE Detection RTSP Streaming Server
---

## 1. Yêu cầu hệ thống (Prerequisites)

### Phía Server (Windows Laptop)
* **GStreamer:** Cài đặt bản **MSVC 64-bit** (bao gồm cả `runtime` và `development` package).
  * Link: [GStreamer Download](https://gstreamer.freedesktop.org/download/)
* **Compiler:** MSVC (Visual Studio 2022) hoặc MinGW.
* **Build Tool:** CMake (phiên bản 3.10 trở lên).
* **FFmpeg:** Dùng để tiền xử lý video.
* **FFmpeg:** Dùng để tiền xử lý và nén video H.265.
  * **Cách cài đặt nhanh (Trên Windows 10/11):** Mở Command Prompt (cmd) và gõ lệnh sau:
    ```cmd
    winget install ffmpeg
    ```
  * *(Lưu ý: Sau khi cài đặt xong, hãy tắt và mở lại Terminal/VS Code để hệ thống nhận diện lệnh `ffmpeg`).*
* **VLC Media Player** (Để test trên Windows).


---

## 2. Chuẩn bị dữ liệu (Video Preparation)

Để luồng video không bị lag và mất gói tin trên mạng không dây, cần nén video về chuẩn H.265 **Zero-Latency** (loại bỏ B-frames):

```cmd
ffmpeg -i "input_video.mp4" -c:v libx265 -x265-params "keyint=30:min-keyint=30:bframes=0:scenecut=0" -preset ultrafast -b:v 2M "PPE_Part1_HEVC.mp4"
```
e.g: ffmpeg -i "C:\Users\NAM NGUYEN\Desktop\DACN\vid\PPE_Part1.mp4" -c:v libx265 -x265-params "keyint=30:min-keyint=30:scenecut=0" -preset ultrafast -b:v 2M "C:\Users\NAM NGUYEN\Desktop\DACN\vid\PPE_Part1_HEVC.mp4"

---

## 3. Cài đặt & Biên dịch (Build & Setup)

1. **Clone repository:**
   ```bash
   git clone [https://github.com/namnguyen1308/EdgeAI/tree/main](https://github.com/namnguyen1308/EdgeAI/tree/main)
   cd stream_server
   ```
2. **Cấu hình CMake:**
   Mở `CMakeLists.txt` và đảm bảo đường dẫn `GSTREAMER_ROOT` trỏ đúng vào thư mục cài đặt (ví dụ: `D:/gstreamer/msvc_x86_64`).
3. **Build dự án (VS Code):**
   * Nhấn `Ctrl + Shift + P` -> `CMake: Select a Kit` -> Chọn `Visual Studio Community 2022 Release - amd64`.
   * Nhấn **Build** (F7).

---

## 4. Khởi chạy Server

1. **Mở cổng Firewall:** Đảm bảo cổng **8554** đã được mở (Inbound Rule) hoặc tắt Windows Firewall.
2. **Lấy địa chỉ IP:** Mở CMD gõ `ipconfig` để lấy IPv4 (ví dụ: `10.166.185.163`).
3. **Chạy Server:**
   ```Terminalx
   cd build/Debug
   ppe_server.exe
   ```

---
## 5. Kiểm tra luồng RTSP bằng VLC (Trên Laptop)

Trước khi kết nối với Raspberry Pi, cần test luồng trên VLC và cấu hình chống mất gói tin (UDP to TCP):

* **Bước 1: Cấu hình giao thức TCP cho VLC**
  1. Mở VLC, nhấn tổ hợp phím **`Ctrl + P`** (Preferences).
  2. Góc dưới bên trái (Show settings), chọn **All**.
  3. Mở rộng mục **Input / Codecs** -> Chọn **Demuxers** -> Chọn **RTP/RTSP**.
  4. Tích vào ô: **Use RTP over RTSP (TCP)**.
  5. Nhấn **Save**.

* **Bước 2: Mở luồng với bộ đệm thấp**
  1. Nhấn tổ hợp phím **`Ctrl + N`** (Open Network Stream).
  2. Nhập địa chỉ: `rtsp://127.0.0.1:8554/live_ppe` (hoặc IP LAN của máy).
  3. Tích vào ô **Show more options**.
  4. Đặt **Caching** = `200` ~ `500` ms.
  5. Nhấn **Play**.
## 6. Các lỗi thường gặp (Troubleshooting)

* **Port 8554 occupied:** Do server cũ chưa tắt hẳn. Dùng `netstat -ano | findstr :8554` để tìm PID và `taskkill /F /PID <số>` để diệt.
* **Invalid cast from GstBin:** Đã được fix trong source code bằng cách lấy Bus trực tiếp từ `GstElement`.
* **Lỗi Port missing in URL (Trên Raspberry Pi):** Đảm bảo URL có đầy đủ tiền tố `rtsp://`, cổng `:8554` và không có khoảng trắng.
