#include "timer.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <bits/types/timer_t.h>
#include <libnotify/notify.h>
#include <stddef.h>
#include <time.h>

const static int screenWidth = 300;
const static int screenHeight = 250;

const static int btn_w = 100;
const static int btn_h = 40;
const static int text_width_padding = 15;
const static int text_height_padding = 10;

static bool mouse_on_text = false;
static char minutes[3] = {0};
static int digit_count = 0;
static bool is_timer_started = false;
static char *start_btn_text = "START";
static float time_left = 0.0f;

void InitTimer() {
  InitWindow(screenWidth, screenHeight, "pomodoro-timer");
  InitNotificationLibrary();
  SetTargetFPS(60);
  char formattedTime[9];
  struct tm timer_info = {0};
  Rectangle rect = {screenWidth / 3.0f - btn_w / 1.2f,
                    screenHeight - 1.5f * btn_h, btn_w, btn_h};
  Vector2 mouse_point = {0.0f, 0.0f};

  Rectangle text_box = {screenWidth / 2.0f - btn_w / 2.0f, screenHeight / 2.0f,
                        50, btn_h};

  while (!WindowShouldClose()) {
    mouse_point = GetMousePosition();

    StartBtnHandler(rect, mouse_point, &timer_info);

    MinuteTextFieldHandler(text_box, mouse_point);
    if (is_timer_started) {
      time_left -= GetFrameTime();

      if (time_left <= 0.0f) {
        time_left = 0.0f;
        is_timer_started = false;
        start_btn_text = "START";
        SendNotification("Pomodoro", "Time to Relax");
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTimer();
    DrawButton(rect);
    DrawRectangleRec(text_box, DARKGRAY);
    if (mouse_on_text)
      DrawRectangleLines(text_box.x, text_box.y, text_box.width,
                         text_box.height, LIGHTGRAY);
    else
      DrawRectangleLines(text_box.x, text_box.y, text_box.width,
                         text_box.height, DARKGRAY);

    DrawText(minutes, text_box.x + 13, text_box.y + 10, 20, MAROON);

    EndDrawing();
  }

  CloseNotificationLibrary();
  CloseWindow();
}

void DrawTimer() {
  int hours = (int)time_left / 3600;
  int mins = ((int)time_left / 60) % 60;
  int secs = (int)time_left % 60;

  const char *formattedTime = TextFormat("%02i:%02i:%02i", hours, mins, secs);
  DrawText(formattedTime, screenWidth / 3, 20, 20, LIGHTGRAY);
}

void FormattedTime(struct tm *time_info, char *buffer, size_t buffer_size) {
  char time_string[9];

  strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);

  strncpy(buffer, time_string, buffer_size - 1);
  buffer[buffer_size - 1] = '\0';
}

void DrawButton(Rectangle rect) {

  DrawRectangleRec(rect, BLUE);
  DrawText(start_btn_text,
           screenWidth / 3.0f - btn_w / 1.2f + text_width_padding,
           screenHeight - 1.5f * btn_h + text_height_padding, 20, WHITE);
}

void StartBtnHandler(Rectangle rect, Vector2 mouse_point,
                     struct tm *timer_info) {
  if (IsMouseOnButton(rect, mouse_point) &&
      IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (minutes[0] != '\0') {
      int converted_minutes = atoi(minutes);
      time_left = converted_minutes * 60.0f;

      while (digit_count > 0) {
        minutes[digit_count - 1] = '\0';
        digit_count--;
      }
    }

    if (time_left > 0.0f) {
      is_timer_started = !is_timer_started;
      if (is_timer_started) {
        start_btn_text = "PAUSE";
      } else {
        start_btn_text = "START";
      }
    }
  }
}

void MinuteTextFieldHandler(Rectangle text_box, Vector2 mouse_point) {
  if (IsMouseOnButton(text_box, mouse_point)) {
    mouse_on_text = true;
  } else {
    mouse_on_text = false;
  }

  if (mouse_on_text) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);

    int key = GetCharPressed();
    if (key >= 48 && key <= 57 && digit_count < 2) {
      minutes[digit_count] = (char)key;
      minutes[digit_count + 1] = '\0';
      digit_count++;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
      digit_count--;
      if (digit_count < 0)
        digit_count = 0;
      minutes[digit_count] = '\0';
    }
  } else
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

bool IsMouseOnButton(Rectangle rect, Vector2 mouse_point) {
  bool collision = false;
  if ((mouse_point.x >= rect.x && mouse_point.x <= rect.x + rect.width) &&
      (mouse_point.y >= rect.y && mouse_point.y <= rect.y + rect.height)) {
    collision = true;
  }

  return collision;
}

void InitNotificationLibrary(void) { notify_init("Pomodoro"); }

void CloseNotificationLibrary(void) { notify_uninit(); }

void SendNotification(const char *title, const char *message) {
  NotifyNotification *n = notify_notification_new(title, message, NULL);

  notify_notification_set_urgency(n, NOTIFY_URGENCY_CRITICAL);

  notify_notification_show(n, NULL);

  g_object_unref(G_OBJECT(n));
}
