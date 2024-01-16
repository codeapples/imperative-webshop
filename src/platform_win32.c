#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include "platform.h"
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>

void cursor_hide(void) {
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursor_info;
  GetConsoleCursorInfo(console_handle, &cursor_info);
  cursor_info.bVisible = false;
  SetConsoleCursorInfo(console_handle, &cursor_info);
}

void cursor_show(void) {
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursor_info;
  GetConsoleCursorInfo(console_handle, &cursor_info);
  cursor_info.bVisible = true;
  SetConsoleCursorInfo(console_handle, &cursor_info);
}

void console_write_styled(u8_t style, const char* fmt, ...) {
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  char buffer[1024] = {0};

  va_list args;
  va_start(args, fmt);
  vsprintf_s(buffer, 1024, fmt, args);
  va_end(args);

  CONSOLE_SCREEN_BUFFER_INFO console_info;

  if (style != 0) {
    // save
    GetConsoleScreenBufferInfo(console_handle, &console_info);
    // set
    SetConsoleTextAttribute(console_handle, style);
  }

  DWORD written;
  WriteConsole(console_handle, buffer, strlen(buffer), &written, NULL);

  if (style != 0) {
    // restore
    SetConsoleTextAttribute(console_handle, console_info.wAttributes);
  }
}

#endif

/*
enum ConsoleColors
{
   BlackFore   = 0,
   MaroonFore  = FOREGROUND_RED,
   GreenFore   = FOREGROUND_GREEN,
   NavyFore    = FOREGROUND_BLUE,
   TealFore    = FOREGROUND_GREEN | FOREGROUND_BLUE,
   OliveFore   = FOREGROUND_RED | FOREGROUND_GREEN,
   PurpleFore  = FOREGROUND_RED | FOREGROUND_BLUE,
   GrayFore    = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
   SilverFore  = FOREGROUND_INTENSITY,
   RedFore     = FOREGROUND_INTENSITY | FOREGROUND_RED,
   LimeFore    = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
   BlueFore    = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
   AquaFore    = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
   YellowFore  = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
   FuchsiaFore = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
   WhiteFore   = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN |
FOREGROUND_BLUE,

   BlackBack   = 0,
   MaroonBack  = BACKGROUND_RED,
   GreenBack   = BACKGROUND_GREEN,
   NavyBack    = BACKGROUND_BLUE,
   TealBack    = BACKGROUND_GREEN | BACKGROUND_BLUE,
   OliveBack   = BACKGROUND_RED | BACKGROUND_GREEN,
   PurpleBack  = BACKGROUND_RED | BACKGROUND_BLUE,
   GrayBack    = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
   SilverBack  = BACKGROUND_INTENSITY,
   RedBack     = BACKGROUND_INTENSITY | BACKGROUND_RED,
   LimeBack    = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
   BlueBack    = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
   AquaBack    = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
   YellowBack  = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
   FuchsiaBack = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
   WhiteBack   = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN |
BACKGROUND_BLUE,
};
*/

/*
#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
*/
