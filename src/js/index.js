var AppReadyService = function () { };

AppReadyService.prototype.ready = function (cb) {
  Pebble.addEventListener("ready", function(event) {
    Pebble.sendAppMessage({
      "AppReadyService_Ready": 1
    }, function(data) {
      cb(data, null);
    }, function(data, err) {
      cb(data, err);
    });
  });
};

module.exports = new AppReadyService();
