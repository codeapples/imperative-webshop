#ifndef WEBSHOP_H
#define WEBSHOP_H

#include "darray.h"
#include "menu.h"
#include "types.h"

typedef struct shop_item_t {
  char name[64];
  u16_t price;
  u16_t quantity;
} shop_item_t;

typedef struct state_t {
  bool running;
  menu_t menu;
  shop_item_t* store;
  shop_item_t* cart;
} state_t;

state_t webshop_init(void);
void webshop_shutdown(state_t* state);

#endif
