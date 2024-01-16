#ifndef PLATFORM_H
#define PLATFORM_H

#include "types.h"

void cursor_hide(void);
void cursor_show(void);

void console_write_styled(u8_t style, const char* fmt, ...);

#define console_write(fmt, ...) console_write_styled(0, fmt, ##__VA_ARGS__);

#define console_write_color(color, fmt, ...) console_write_styled(color, fmt, ##__VA_ARGS__);

#define console_write_highlighted(fmt, ...) console_write_styled(0x0080, fmt, ##__VA_ARGS__);

#define console_write_backlighted(fmt, ...) console_write_styled(8, fmt, ##__VA_ARGS__);

#endif
