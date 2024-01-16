#ifndef MENU_H
#define MENU_H

#include "types.h"

#define MENU_ITEMS_NUM 6

typedef struct state_t state_t;

typedef struct menu_item_t {
  const char* text;
  void (*action)(state_t* state);
} menu_item_t;

typedef struct submenu_t {
  char* title;
  void (*callback)(state_t* state);
  u8_t selected_item;
  u8_t variant;
  bool back;
} submenu_t;

typedef struct menu_t {
  const menu_item_t items[MENU_ITEMS_NUM];
  const u8_t items_num;
  u8_t selected_item;
  submenu_t submenu;
} menu_t;

void menu_draw(state_t* state);

void menu_select_next(state_t* state);
void menu_select_prev(state_t* state);
void menu_exec(state_t* state);

void menu_submenu_clear(state_t* state);

void menu_dialog_single(state_t* state, const char* text, u8_t id, bool clear);
void menu_dialog_binary(
  state_t* state, const char* fst, const char* snd, u8_t fst_id, u8_t snd_id, bool clear
);

#endif
