#pragma once

#include <VL6180X.h>
#include <ctrl/accumulator.h>
#include <peripheral/i2c.h>

class ToF {
public:
  static constexpr UBaseType_t Priority = 1;

public:
  ToF(i2c_port_t i2c_port, float tof_dist_offset)
      : sensor(i2c_port), tof_dist_offset(tof_dist_offset) {}
  bool init() {
    sensor.setTimeout(80);
    sensor.init();
    sensor.configureDefault();
    sensor.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 0x20);
    // sensor.writeReg(VL6180X::READOUT__AVERAGING_SAMPLE_PERIOD, 0x40);
    xTaskCreate([](void *arg) { static_cast<decltype(this)>(arg)->task(); },
                "ToF", 4096, this, Priority, NULL);
    vTaskDelay(pdMS_TO_TICKS(40));
    if (sensor.last_status != 0) {
      log_e("ToF failed :(");
      return false;
    }
    return true;
  }
  void enable() { enabled = true; }
  void disable() { enabled = false; }
  uint16_t getDistance() const { return distance; }
  const auto &getLog() const { return log; }
  uint16_t passedTimeMs() const { return passed_ms; }
  bool isValid() const { return passed_ms < 20; }
  void print() const { log_d("ToF: %d [mm] Dur: %d [ms]", getDistance(), dur); }
  void csv() const {
    printf("0,45,90,135,180,%d,%d\n", getDistance(), passed_ms);
  }

private:
  VL6180X sensor;
  float tof_dist_offset;
  bool enabled = true;
  uint16_t distance;
  uint16_t dur;
  int passed_ms;
  ctrl::Accumulator<uint16_t, 10> log;

  void task() {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
      if (!enabled) {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
        passed_ms++;
        continue;
      }
      /* sampling start */
      sensor.writeReg(VL6180X::SYSTEM__INTERRUPT_CLEAR, 0x01);
      sensor.writeReg(VL6180X::SYSRANGE__START, 0x01);
      {
        uint32_t startAt = millis();
        while ((sensor.readReg(VL6180X::RESULT__INTERRUPT_STATUS_GPIO) &
                0x04) == 0) {
          vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
          passed_ms++;
          if (millis() - startAt > 100)
            break;
        }
        dur = millis() - startAt;
      }
      /* get data from sensor */
      uint16_t range = sensor.readReg(VL6180X::RESULT__RANGE_VAL);
      distance = range + tof_dist_offset;
      log.push(distance);
      if (range != 255) {
        passed_ms = 0;
      }
    }
  }
};
