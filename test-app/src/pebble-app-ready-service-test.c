#include <pebble.h>
#include "pebble-app-ready-service/pebble-app-ready-service.h"

static Window *s_window;
static TextLayer *s_text_layer;
static char* message = "Waiting for JS";


static void prv_on_app_ready(void* context) {
  text_layer_set_text(s_text_layer, "JS Ready!");
}

static void prv_on_app_timeout(void *context) {
  text_layer_set_text(s_text_layer, "JS Timeout..");
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
  text_layer_set_text(s_text_layer, "Waiting for JS..");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });
  window_stack_push(s_window, true);

  app_ready_service_set_timeout(1000);
  app_ready_service_subscribe((AppReadyHandlers) {
    .ready = prv_on_app_ready,
    .timeout = prv_on_app_timeout
  }, NULL);
  app_message_open(512,512);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
