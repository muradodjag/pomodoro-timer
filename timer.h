#include "time.h"
#include <raylib.h>

void InitTimer();
void FormattedTime(struct tm *time_info, char *buffer, size_t buffer_size);
void DrawTimer();
void ZeroTime(struct tm *timer_info);
void DrawButton(Rectangle rect, char *text, Vector2 text_position);
bool IsMouseOnButton(Rectangle rect, Vector2 mouse_point);
void StartBtnHandler(Rectangle rect, Vector2 mouse_point);
void ResetBtnHandler(Rectangle rect, Vector2 mouse_point);
void MinuteTextFieldHandler(Rectangle text_box, Vector2 mouse_point);
void InitNotificationLibrary();
void CloseNotificationLibrary();
void SendNotification(const char *title, const char *message);
void InitDefaultValues();
void BlockSites();
void UnblockSites();
