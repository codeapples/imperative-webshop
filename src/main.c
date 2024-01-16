#include "webshop.h"
#include <conio.h>

// will only work on Windows
int main(void) {
  i16_t key = 0;

  state_t state = webshop_init();

  while (state.running) {
    menu_draw(&state);
    // get key without output to the console
    key = _getch();

    if (key == 80)
      menu_select_next(&state);

    if (key == 72)
      menu_select_prev(&state);

    if (key == 13)
      menu_exec(&state);

    if (key == 27)
      state.menu.items[MENU_ITEMS_NUM - 1].action(&state);
  }

  webshop_shutdown(&state);

  return 0;
}
