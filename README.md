# pebble-app-ready-service

The AppReadyService adds an `AppReady` that fires when the app's JavaScript has
successfully started, and sent a message to the C code. Developer's should wait
for this event to fire before sending any AppMessages to the JavaScript.

## Getting Stated

This package includes both C and JavaScript code. Information on how to install
the package, and work with it in your JavaScript and C code are detailed below.

The [Test Application](./test-app) demonstrates a minimal code sample with the
package.

### Installation

To install the package, run the following command from a Pebble project:

```
> pebble package install pebble-app-ready-service
```

### JavaScript

After install the package, add the following to the start of your app.js code:

```js
var AppReadyService = require('pebble-app-ready-service');

AppReadyService.ready(function() {
  // App is ready to communicate with C code
});
```

### C Code

To use the package in your C code, you will first need to `include` the header
file in your application:

```c
#include "pebble-app-ready-service/app-ready-service.h"
```

After including the file, you can subscribe to the `ready` and `timeout` events
by creating an `AppReadyHandlers` object, and then invoking the
`app_ready_service_subscribe` method. This is typically done in your app's init
code.

```c
static void prv_app_ready(void *context) {
  // JavaScript is running, and ready to accept AppMessages
  // ...

}

static void prv_app_timeout(void *context) {
  // JS is not running (watch is likely disconnected)
  // ...

}

static void prv_init() {
  // Init code
  // ...

  // Subscribe to the ready service
  app_ready_service_subscribe((AppReadyHandlers){
    .ready = prv_app_ready,
    .timeout = prv_app_timeout
  }, NULL);
}
```

## API Documentation

#### AppReadyHandlers

```
// Define the struct to hold our handlers
typedef struct AppReadyHandlers {
  AppReadyCallback ready;
  AppReadyCallback timeout;
} AppReadyHandlers;
```

The AppReadyHandlers struct is used to define the `.ready` and `.timeout`
callbacks for the AppReadyService. The callbacks must be `void` functions with
a `void* context` parameter.

#### void app_ready_service_subscribe(AppReadyHandlers handlers, void* context);

The `app_ready_service_subscribe` registers the supplied callback methods. The
second parameter is a pointer to the context data the callback methods will use.
If not context is required, `NULL` should be passed as the second parameter.

#### void app_ready_service_set_timeout(uint16_t timeout);

The `app_ready_service_set_timeout` methods allows you to change the default
timeout period (5000 ms). 

**NOTE**: If changing the timeout period, this method *must* be called before
`app_ready_service_subscribe`.

## License

This package is licensed under the [MIT License](./LICENSE).
