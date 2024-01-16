#include "menu.h"
#include "platform.h"
#include "webshop.h"
#include <conio.h>
#include <stdlib.h>

void menu_draw(state_t* state) {
  system("cls");
  if (state->running == false) {
    console_write("Bye.\n");
    return;
  }

  console_write_color(3, "****************** WEBSHOP v1.0 ******************\n");
  console_write_backlighted("Hint: For navigation please use arrow keys.\n");
  console_write_backlighted("ENTER to select an item. ESC to go back or exit.\n\n");

  // if we need to show submenu call it and return
  if (state->menu.submenu.callback != NULL) {
    console_write("%s\n\n", state->menu.submenu.title);
    state->menu.submenu.callback(state);
    return menu_draw(state);
  }

  console_write("Store data: ");
  if (darray_length(state->store) > 0) {
    console_write_color(2, "loaded. %d items in store.\n", darray_length(state->store));
  } else {
    console_write_color(4, "not loaded.\n");
  }
  console_write("User cart: ");
  if (darray_length(state->cart) > 0) {
    console_write_color(2, "contains %d items.\n", darray_length(state->cart));
  } else {
    console_write_color(4, "is empty.\n");
  }

  console_write("\n");
  for (int i = 0; i < state->menu.items_num; i++) {
    if (i == state->menu.selected_item) {
      console_write_highlighted("%d. %s\n", i + 1, state->menu.items[i].text);
    } else {
      console_write("%d. %s\n", i + 1, state->menu.items[i].text);
    }
  }
}

void menu_select_next(state_t* state) {
  if (state->menu.selected_item < state->menu.items_num - 1) {
    state->menu.selected_item++;
  }
}

void menu_select_prev(state_t* state) {
  if (state->menu.selected_item > 0) {
    state->menu.selected_item--;
  }
}

void menu_exec(state_t* state) {
  system("cls");
  state->menu.items[state->menu.selected_item].action(state);
}

void menu_submenu_clear(state_t* state) {
  state->menu.submenu.title = NULL;
  state->menu.submenu.callback = NULL;
  state->menu.submenu.variant = 0;
  state->menu.submenu.selected_item = 0;
  state->menu.submenu.back = false;
}

void menu_dialog_single(state_t* state, const char* text, u8_t id, bool clear) {
  console_write_highlighted("%s\n", text);

  i16_t key = 0;
  while (true) {
    key = _getch();
    if (key == 13) { // enter
      if (clear)
        menu_submenu_clear(state);
      state->menu.submenu.variant = id;
      break;
    }
    if (key == 27) { // esc
      if (clear)
        menu_submenu_clear(state);
      state->menu.submenu.variant = id;
      break;
    }
  }
}

void menu_dialog_binary(
  state_t* state, const char* fst, const char* snd, u8_t fst_id, u8_t snd_id, bool clear
) {
  if (!state->menu.submenu.back) {
    console_write_highlighted("%s\n", fst);
    console_write("%s\n", snd);
  } else {
    console_write("%s\n", fst);
    console_write_highlighted("%s\n", snd);
  }

  i16_t key = 0;
  while (true) {
    key = _getch();
    if (key == 80) { // down
      state->menu.submenu.back = true;
      break;
    }
    if (key == 72) { // up
      state->menu.submenu.back = false;
      break;
    }
    if (key == 13) { // enter
      if (!state->menu.submenu.back)
        state->menu.submenu.variant = fst_id;
      else {
        if (clear)
          menu_submenu_clear(state);
        else
          state->menu.submenu.back = false;
        state->menu.submenu.variant = snd_id;
      }
      break;
    }
    if (key == 27) {
      if (clear)
        menu_submenu_clear(state);
      else
        state->menu.submenu.back = false;
      state->menu.submenu.variant = snd_id;
      break;
    }
  }
}
