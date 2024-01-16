#include "webshop.h"
#include "darray.h"
#include "platform.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void app_shutdown(state_t* state) {
  if (state->menu.submenu.variant == 1) {
    state->running = false;
    return;
  }
  console_write_color(3, "Are you sure you want to exit?\n\n");
  menu_dialog_binary(state, "Yes", "No", 1, 0, true);
}

void store_load(state_t* state) {
  // dialog if failed to load, retry vs quit
  if (state->menu.submenu.variant == 1) {
    console_write_color(4, "Failed to load store data from a file.\n\n");
    menu_dialog_binary(state, "Try again", "Back", 0, 0, true);
    return;
  }
  // success dialog
  if (state->menu.submenu.variant == 2) {
    console_write_color(5, "Store data loaded successfully!\n\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }

  // regular dialog to load file
  cursor_show();
  console_write("Please enter the name of the file: ");
  char filename[128] = {0};
  fgets(filename, sizeof(filename), stdin); // scanf_s works weirdly
  sscanf_s(filename, "%s", filename, sizeof(filename));
  cursor_hide();

  FILE* f = NULL;
  fopen_s(&f, filename, "r");

  if (!f) {
    state->menu.submenu.variant = 1;
    return;
  }

  // clear the store
  darray_clear(state->store);

  i8_t n = 0;
  shop_item_t item = {0};
  while ((n = fscanf_s(f, "%s %hd %hd", item.name, 64, &item.price, &item.quantity)) != EOF &&
         f != NULL) {
    if (n != 3)
      continue;
    darray_push(state->store, &item);
  }
  fclose(f);
  state->menu.submenu.variant = 2;
}

void store_save(state_t* state) {
  // empty store handle
  if (darray_length(state->store) == 0) {
    console_write_color(5, "Nothing is available in store!\n");
    console_write("If you haven't loaded data yet, you must load it first.\n\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }
  // dialog if failed to save, retry vs quit
  if (state->menu.submenu.variant == 1) {
    console_write_color(4, "Failed to save store data to a file.\n\n");
    menu_dialog_binary(state, "Try again", "Back", 0, 0, true);
    return;
  }
  // success dialog
  if (state->menu.submenu.variant == 2) {
    console_write_color(5, "Store data saved successfully!\n\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }

  // regular dialog to save file
  cursor_show();
  console_write("Please enter the name of the file: ");
  char filename[128] = {0};
  fgets(filename, sizeof(filename), stdin); // scanf_s works weirdly
  sscanf_s(filename, "%s", filename, sizeof(filename));
  cursor_hide();

  FILE* f = NULL;
  fopen_s(&f, filename, "w");

  if (!f) {
    state->menu.submenu.variant = 1;
    return;
  }

  u64_t length = darray_length(state->store);
  for (int i = 0; i < length; i++) {
    shop_item_t* item = &state->store[i];
    fprintf(f, "%s %d %d\n", item->name, item->price, item->quantity);
  }
  fclose(f);
  state->menu.submenu.variant = 2;
}

void cart_add(state_t* state) {
  // empty store handle
  if (darray_length(state->store) == 0) {
    console_write_color(5, "Nothing is available in store!\n");
    console_write("If you haven't loaded data yet, you must load it first.\n\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }
  // buying dialog
  if (state->menu.submenu.variant == 1) {
    shop_item_t* item = &state->store[state->menu.submenu.selected_item];
    console_write_color(3, "How many %s do you want to buy?\n", item->name);
    console_write("Available: ");
    console_write_color(2, "%d\n", item->quantity);
    u16_t cart_quantity = 0;
    for (int i = 0; i < darray_length(state->cart); i++) {
      shop_item_t* cart_item = &state->cart[i];
      if (strcmp(cart_item->name, item->name) == 0) {
        cart_quantity += cart_item->quantity;
      }
    }
    console_write("In cart: ");
    console_write_color(2, "%d\n\n", cart_quantity);

    cursor_show();
    console_write("Please enter required quantity: ");
    cursor_hide();
    // use fgets & sscanf instead of scanf_s for stability
    char buffer[128] = {0};
    u16_t quantity = 0;
    if (!fgets(buffer, sizeof(buffer), stdin) || sscanf_s(buffer, "%hd", &quantity) != 1) {
      state->menu.submenu.variant = 2;
    } else {
      // if we got here then we have a valid quantity
      if (quantity + cart_quantity > item->quantity) {
        state->menu.submenu.variant = 3;
      } else {
        // see if item already in cart
        shop_item_t* cart_item = NULL;
        for (int i = 0; i < darray_length(state->cart); i++) {
          if (strcmp(state->cart[i].name, item->name) == 0) {
            cart_item = &state->cart[i];
            break;
          }
        }
        // if not, add it
        if (!cart_item) {
          u64_t len = darray_length(state->cart);
          darray_push(state->cart, item);
          state->cart[len].quantity = quantity;
        } else {
          cart_item->quantity += quantity;
        }

        state->menu.submenu.variant = 4;
      }
    }

    return;
  }
  // wrong input dialog
  if (state->menu.submenu.variant == 2) {
    console_write_color(4, "Incorrect input.\n");
    console_write("Please provide a correct number.\n\n");
    menu_dialog_binary(state, "Try again", "Back", 1, 0, false);
    return;
  }
  // not enough items dialog
  if (state->menu.submenu.variant == 3) {
    console_write_color(4, "Not enough items in the store.\n\n");
    menu_dialog_binary(state, "Try again", "Back", 1, 0, false);
    return;
  }
  // added to cart dialog
  if (state->menu.submenu.variant == 4) {
    console_write_color(5, "Items added to cart.\n\n");
    menu_dialog_binary(state, "Continue shopping", "Back to menu", 0, 0, true);
    return;
  }

  console_write_backlighted("#\tName\t\tPrice\tQuantity\n");
  u64_t length = darray_length(state->store);
  for (int i = 0; i < length; i++) {
    shop_item_t* item = &state->store[i];

    if (i == state->menu.submenu.selected_item) {
      console_write_highlighted(
        "%d.\t%-*s\t%d\t%d\n", i + 1, 12, item->name, item->price, item->quantity
      );
    } else {
      console_write("%d.\t%-*s\t%d\t%d\n", i + 1, 12, item->name, item->price, item->quantity);
    }
  }

  i16_t key = 0;
  while (true) {
    key = _getch();
    if (key == 80) { // down
      if (state->menu.submenu.selected_item < length - 1)
        state->menu.submenu.selected_item++;
      break;
    }
    if (key == 72) { // up
      if (state->menu.submenu.selected_item > 0)
        state->menu.submenu.selected_item--;
      break;
    }
    if (key == 13) { // enter
      state->menu.submenu.variant = 1;
      break;
    }
    if (key == 27) { // esc
      menu_submenu_clear(state);
      break;
    }
  }
}

void cart_price(state_t* state) {
  if (darray_length(state->cart) == 0) {
    console_write_color(5, "Nothing in cart yet!\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }

  console_write_backlighted("#\tName\t\tPrice\tQuantity\n");
  u64_t length = darray_length(state->cart);
  u32_t total = 0;
  for (int i = 0; i < length; i++) {
    shop_item_t* item = &state->cart[i];
    console_write("%d.\t%-*s\t%d\t%d\n", i + 1, 12, item->name, item->price, item->quantity);
    total += item->price * item->quantity;
  }

  console_write_color(3, "\nTotal price of items in cart: ");
  console_write_color(2, "%d\n\n", total);
  menu_dialog_single(state, "Ok", 0, true);
}

void cart_buy(state_t* state) {
  if (darray_length(state->cart) == 0) {
    console_write_color(5, "Nothing in cart yet!\n");
    menu_dialog_single(state, "Ok", 0, true);
    return;
  }

  u64_t length = darray_length(state->cart);
  u32_t total = 0;
  console_write_backlighted("#\tName\t\tPrice\tQuantity\n");
  for (int i = 0; i < length; i++) {
    shop_item_t* item = &state->cart[i];
    console_write("%d.\t%-*s\t%d\t%d\n", i + 1, 12, item->name, item->price, item->quantity);
    total += item->price * item->quantity;
  }
  console_write_color(3, "\nTotal price of items in cart: ");
  console_write_color(2, "%d\n\n", total);

  // bought successfully
  if (state->menu.submenu.variant == 1) {
    for (int i = 0; i < length; i++) {
      for (int j = 0; j < darray_length(state->store); j++) {
        if (strcmp(state->cart[i].name, state->store[j].name) == 0) {
          if (state->cart[i].quantity >= state->store[j].quantity) {
            darray_pop_at(state->store, j);
          } else {
            state->store[j].quantity -= state->cart[i].quantity;
          }
        }
      }
    }

    darray_clear(state->cart);
    console_write_color(5, "Items bought successfully!\n\n");
    menu_dialog_single(state, "Ok", 0, true);

    return;
  }

  console_write("Are you sure you want to buy these items?\n\n");
  menu_dialog_binary(state, "Yes", "No", 1, 0, true);
}

void app_shutdown_menu(state_t* state) {
  state->menu.submenu.title = "Exiting...";
  state->menu.submenu.callback = &app_shutdown;
}

void store_load_menu(state_t* state) {
  state->menu.submenu.title = "Loading store data from a file...";
  state->menu.submenu.callback = &store_load;
}

void store_save_menu(state_t* state) {
  state->menu.submenu.title = "Saving store data to a file...";
  state->menu.submenu.callback = &store_save;
}

void cart_add_menu(state_t* state) {
  state->menu.submenu.title = "Choose which item you want...";
  state->menu.submenu.callback = &cart_add;
}

void cart_price_menu(state_t* state) {
  state->menu.submenu.title = "Here is what you have in cart...";
  state->menu.submenu.callback = &cart_price;
}

void cart_buy_menu(state_t* state) {
  state->menu.submenu.title = "Buying items...";
  state->menu.submenu.callback = &cart_buy;
}

state_t webshop_init(void) {
  cursor_hide();
  return (state_t){
    .running = true,
    .menu =
      (menu_t){
        .items =
          {
            (menu_item_t){.text = "Load store data from a file", .action = &store_load_menu},
            (menu_item_t){.text = "Save store data to a file", .action = &store_save_menu},
            (menu_item_t){.text = "Add item to cart", .action = &cart_add_menu},
            (menu_item_t){.text = "Price of items in cart", .action = &cart_price_menu},
            (menu_item_t){.text = "Buy products", .action = &cart_buy_menu},
            (menu_item_t){.text = "Exit the program", .action = &app_shutdown_menu},
          },
        .items_num = MENU_ITEMS_NUM,
      },
    .store = darray_create(shop_item_t),
    .cart = darray_create(shop_item_t),
  };
}

void webshop_shutdown(state_t* state) {
  darray_destroy(state->store);
  darray_destroy(state->cart);
  cursor_show();
}
