#include <pebble.h>
#include "craft.h"
#include "main.h"

Window *craft_window;
ScrollLayer *scroll;
BitmapLayer *recipe;
GBitmap *background_bitmap;
Recipe craft_recipe;

void craft_window_load(Window *window){
  scroll = scroll_layer_create(layer_get_bounds(window_get_root_layer(window)));
  scroll_layer_set_click_config_onto_window(scroll, craft_window);
}