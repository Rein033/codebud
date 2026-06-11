// Native LVGL "simulator" for the launcher: renders the home screen and each
// app to a BMP file, without needing real ESP32 hardware.

#include <lvgl.h>

#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <vector>

#include "launcher.h"

constexpr int W = 240;
constexpr int H = 320;

static lv_color_t fb[W * H];
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[W * 10];

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            fb[y * W + x] = *color_p;
            color_p++;
        }
    }
    lv_disp_flush_ready(disp);
}

void write_bmp(const char *filename) {
    int row_size = (W * 3 + 3) & ~3;
    int data_size = row_size * H;
    int file_size = 54 + data_size;

    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *reinterpret_cast<int32_t *>(&header[2]) = file_size;
    *reinterpret_cast<int32_t *>(&header[10]) = 54;
    *reinterpret_cast<int32_t *>(&header[14]) = 40;
    *reinterpret_cast<int32_t *>(&header[18]) = W;
    *reinterpret_cast<int32_t *>(&header[22]) = H;
    *reinterpret_cast<int16_t *>(&header[26]) = 1;
    *reinterpret_cast<int16_t *>(&header[28]) = 24;
    *reinterpret_cast<int32_t *>(&header[34]) = data_size;

    FILE *f = fopen(filename, "wb");
    fwrite(header, 1, 54, f);

    std::vector<unsigned char> row(row_size, 0);
    for (int y = H - 1; y >= 0; y--) {
        for (int x = 0; x < W; x++) {
            lv_color32_t c32;
            c32.full = lv_color_to32(fb[y * W + x]);
            row[x * 3 + 0] = c32.ch.blue;
            row[x * 3 + 1] = c32.ch.green;
            row[x * 3 + 2] = c32.ch.red;
        }
        fwrite(row.data(), 1, row_size, f);
    }
    fclose(f);
}

void settle(int iterations = 30) {
    for (int i = 0; i < iterations; i++) {
        lv_timer_handler();
        usleep(10000);
    }
}

int main() {
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, nullptr, W * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = W;
    disp_drv.ver_res = H;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    launcher_show();
    settle();
    write_bmp("tools/sim/out_home.bmp");

    const char *names[] = {"clock", "pomodoro", "pong", "sensor", "messages"};
    for (int i = 0; i < 5; i++) {
        lv_obj_t *home = lv_scr_act();
        lv_obj_t *tile = lv_obj_get_child(home, i);
        lv_event_send(tile, LV_EVENT_CLICKED, nullptr);
        settle(i == 2 ? 60 : 30);  // give Pong extra frames so the ball moves

        char filename[64];
        snprintf(filename, sizeof(filename), "tools/sim/out_%s.bmp", names[i]);
        write_bmp(filename);

        lv_obj_t *app_scr = lv_scr_act();
        lv_obj_t *top_bar = lv_obj_get_child(app_scr, 0);
        lv_obj_t *back_btn = lv_obj_get_child(top_bar, 0);
        lv_event_send(back_btn, LV_EVENT_CLICKED, nullptr);
        settle();
    }

    return 0;
}
