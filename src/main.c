#include <pebble.h>
#include "category_window.h"
#include "craft.h"
#include "main.h"


static MenuLayer *menu_layer;
static Window *window;
Recipe current_recipe;


uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context){
  return 10;
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context){
  switch(cell_index->row)
    // Information gathered from http://minecraft.gamepedia.com/Inventory#Creative_inventory
  {
    // Create a row called "Building"
    case 0:
    menu_cell_basic_draw(ctx, cell_layer, "Building", NULL, NULL);
    break;
    // Create a row called "Decoration"
    case 1:
    menu_cell_basic_draw(ctx, cell_layer, "Decoration", NULL, NULL);
    break;
    // Create a row called "Redstone"
    case 2:
    menu_cell_basic_draw(ctx, cell_layer, "Redstone", NULL, NULL);
    break;
    // Create a row called "Transport"
    case 3:
    menu_cell_basic_draw(ctx, cell_layer, "Transport", NULL, NULL);
    break;
    // Create a row called "Misc." -- short for "Miscellaneous"
    case 4:
    menu_cell_basic_draw(ctx, cell_layer, "Misc.", NULL, NULL);
    break;
    // Create a row called "Food"
    case 5:
    menu_cell_basic_draw(ctx, cell_layer, "Food", NULL, NULL);
    break;
    // Create a row called "Tools"
    case 6:
    menu_cell_basic_draw(ctx, cell_layer, "Tools", NULL, NULL);
    break;
    // Create a row called "Combat"
    case 7:
    menu_cell_basic_draw(ctx, cell_layer, "Combat", NULL, NULL);
    break;
    // Create a row called "Brewing"
    case 8:
    menu_cell_basic_draw(ctx, cell_layer, "Brewing", NULL, NULL);
    break;
    // Create a row called "Snapshot"
    case 9:
    menu_cell_basic_draw(ctx, cell_layer, "Snapshot", NULL, NULL);
    break;
  }
}

void process_tuple(Tuple *t){
    int key = t->key;
    int value = t->value->int32;
  
  switch (key) {
    case 0:
    current_recipe.type = value;
    break;
    
    case 1:
    strcpy(current_recipe.title[0], t->value->cstring);
    break;
    
    case 2:
    strcpy(current_recipe.name[0], t->value->cstring);
    break;
    
    case 3:
    strcpy(current_recipe.recipe_url[0], t->value->cstring);
    break;
    
  }
  
}

void inbox(DictionaryIterator *iter, void *context){
  // Download the item names fromt the selected category
  Tuple *t = dict_read_first(iter);
  if(t){
    process_tuple(t);
  } while(t != NULL){
    t = dict_read_next(iter);
    if(t){
      process_tuple(t);
    }
  }
  category_add_recipe(current_recipe);
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex, *cell_index, void *callback_context){
  // Get the row that is being selected
  int which = cell_index->row;
  
  category_set_index(which);
  window_stack_push(category_get_window(), true);
}

static void window_load(Window *window){
  // Load the Menu Layer
  menu_layer = menu_layer_create(GRect(0, 0, 144, 168 - 16));
  menu_layer_set_click_config_onto_window(menu_layer, window);
  MenuLayerCallbacks = {
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback,
  };
  
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

static void window_unload(Window *window){
  menu_layer_destroy(menu_layer);
}

static void init(){
  APP_LOG(APP_LOG_LEVEL_INFO, "HELLO!");
  window = window_create();
  // Set handlers to manage elements inside the window
  window_set_window_handlers(window, (WindowHandlers) {
  .load = window_load,
  .unload = window_unload
  });
  
  app_message_register_inbox_recieived(inbox);
  app_message_open(512, 512);
  
  window_stack_push(window, true);
}

static void deinit(){
  // Destroy window
  window_destroy(window);
}

int main(void){
  init();
  category_init();
  craft_window_init();
  app_event_loop();
  category_deinit();
  craft_window_deinit();
  deinit();  
}