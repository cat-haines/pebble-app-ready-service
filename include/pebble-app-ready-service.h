#pragma once

// Define the callback type
typedef void (*AppReadyCallback)(void* context);

// Define the struct to hold our handlers
typedef struct AppReadyHandlers {
  AppReadyCallback ready;
  AppReadyCallback timeout;
} AppReadyHandlers;

// Define the public methods to interact with the service
void app_ready_service_set_timeout(uint16_t timeout);
void app_ready_service_subscribe(AppReadyHandlers handlers, void* context);
