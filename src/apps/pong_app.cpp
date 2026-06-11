#include "pong_app.h"

// Tiny single-player Pong: drag anywhere to move the paddle, keep the ball bouncing.

namespace {
constexpr int PADDLE_W = 60;
constexpr int PADDLE_H = 10;
constexpr int BALL_SIZE = 10;

lv_obj_t *paddle = nullptr;
lv_obj_t *ball = nullptr;
lv_obj_t *score_label = nullptr;
lv_timer_t *timer = nullptr;

int area_w = 0, area_h = 0;
int ball_x, ball_y, ball_dx, ball_dy;
int paddle_x;
int score = 0;

void reset_ball() {
    ball_x = area_w / 2 - BALL_SIZE / 2;
    ball_y = 0;
    ball_dx = 3;
    ball_dy = 3;
}

void game_tick(lv_timer_t *) {
    ball_x += ball_dx;
    ball_y += ball_dy;

    if (ball_x <= 0 || ball_x >= area_w - BALL_SIZE) ball_dx = -ball_dx;
    if (ball_y <= 0) ball_dy = -ball_dy;

    if (ball_y >= area_h - BALL_SIZE - PADDLE_H) {
        if (ball_x + BALL_SIZE >= paddle_x && ball_x <= paddle_x + PADDLE_W) {
            ball_dy = -ball_dy;
            score++;
            lv_label_set_text_fmt(score_label, "Score: %d", score);
        } else if (ball_y >= area_h - BALL_SIZE) {
            score = 0;
            lv_label_set_text_fmt(score_label, "Score: %d", score);
            reset_ball();
        }
    }

    lv_obj_set_pos(ball, ball_x, ball_y);
}

void on_pressing(lv_event_t *e) {
    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t p;
    lv_indev_get_point(indev, &p);

    lv_obj_t *target = static_cast<lv_obj_t *>(lv_event_get_target(e));
    lv_area_t area;
    lv_obj_get_coords(target, &area);

    paddle_x = (p.x - area.x1) - PADDLE_W / 2;
    if (paddle_x < 0) paddle_x = 0;
    if (paddle_x > area_w - PADDLE_W) paddle_x = area_w - PADDLE_W;
    lv_obj_set_x(paddle, paddle_x);
}

void create(lv_obj_t *parent) {
    lv_obj_update_layout(parent);
    area_w = lv_obj_get_width(parent);
    area_h = lv_obj_get_height(parent);

    score_label = lv_label_create(parent);
    lv_label_set_text(score_label, "Score: 0");
    lv_obj_align(score_label, LV_ALIGN_TOP_MID, 0, 0);

    ball = lv_obj_create(parent);
    lv_obj_remove_style_all(ball);
    lv_obj_set_size(ball, BALL_SIZE, BALL_SIZE);
    lv_obj_set_style_bg_color(ball, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(ball, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(ball, LV_RADIUS_CIRCLE, 0);

    paddle_x = area_w / 2 - PADDLE_W / 2;
    paddle = lv_obj_create(parent);
    lv_obj_remove_style_all(paddle);
    lv_obj_set_size(paddle, PADDLE_W, PADDLE_H);
    lv_obj_set_style_bg_color(paddle, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(paddle, LV_OPA_COVER, 0);
    lv_obj_set_pos(paddle, paddle_x, area_h - PADDLE_H);

    score = 0;
    reset_ball();
    lv_obj_set_pos(ball, ball_x, ball_y);

    lv_obj_add_event_cb(parent, on_pressing, LV_EVENT_PRESSING, nullptr);

    timer = lv_timer_create(game_tick, 30, nullptr);
}

void destroy() {
    if (timer) {
        lv_timer_del(timer);
        timer = nullptr;
    }
    paddle = nullptr;
    ball = nullptr;
    score_label = nullptr;
}
}  // namespace

App pong_app = {
    .name = "Pong",
    .icon = LV_SYMBOL_PLAY,
    .create = create,
    .update = nullptr,
    .destroy = destroy,
};
