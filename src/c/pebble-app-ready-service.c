#include <pebble.h>
#include "pebble-events/pebble-events.h"
#include "pebble-app-ready-service.h"

static AppReadyHandlers s_ready_handlers;
static void* s_context;
static bool s_fired = false;

static AppTimer* s_timeout_timer;
static uint16_t s_ready_timeout = 5000;

static void prv_on_app_message(DictionaryIterator* params, void* context) {
  // If we haven't fired the callback already, and it's the correct message
  if (!s_fired && dict_find(params, MESSAGE_KEY_AppReadyService_Ready)) {
    // Set the fired flag, cancel the timeout if it exists, and execute callback
    s_fired = true;

    if (s_timeout_timer) {
      app_timer_cancel(s_timeout_timer);
      s_timeout_timer = NULL;
    }

    if (s_ready_handlers.ready) s_ready_handlers.ready(s_context);
  }
}

static void prv_on_timeout(void* context) {
  if (!s_fired) {
    s_fired = true;
    s_timeout_timer = NULL;
  }

  if (s_ready_handlers.timeout) s_ready_handlers.timeout(s_context);
}

// Define the public methods to interact with the service
void app_ready_service_set_timeout(uint16_t timeout) {
  s_ready_timeout = timeout;
}

void app_ready_service_subscribe(AppReadyHandlers handlers, void* context){
  s_ready_handlers = handlers;
  s_context = context;

  // Register the timeout Timer and AppMessage handler
  events_app_message_register_inbox_received(prv_on_app_message, NULL);
  s_timeout_timer = app_timer_register(s_ready_timeout, prv_on_timeout, NULL);
}
