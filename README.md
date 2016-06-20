# pebble-app-ready-service

The AppReadyService adds an `AppReady` that fires when the app's JavaScript has
successfully started, and sent a message to the C code. Developer's should wait
for this event to fire before sending any AppMessages to the JavaScript.

## Getting Stated

This package includes both C and JavaScript code. Information on how to install
the package, and work with it in your JavaScript and C code are detailed below.

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
by creating an `AppReadyHandler` object, and then invoking the
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
  app_ready_service_subscribe((){
    .ready = prv_app_ready,
    .timeout = prv_app_timeout
  }, NULL);
}
```
