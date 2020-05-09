#pragma once

#include "app_log.h"

#include <as5048a.h>
#include <ma730.h>

#include <cmath>
#include <iomanip>

#include "config/model.h" //< for KERISE_SELECT

class Encoder {
private:
  static constexpr int ENCODER_STACK_SIZE = 4096;
  static constexpr int ENCODER_PRIORITY = 5;

public:
  Encoder(float gear_ratio, float wheel_diameter)
      : gear_ratio(gear_ratio), wheel_diameter(wheel_diameter) {
    sampling_end_semaphore = xSemaphoreCreateBinary();
  }
#if KERISE_SELECT == 4 || KERISE_SELECT == 3
  bool begin(spi_host_device_t spi_host, int8_t pin_cs) {
    if (!as.init(spi_host, pin_cs)) {
      loge << "AS5048A init failed :(" << std::endl;
      return false;
    }
    xTaskCreate([](void *arg) { static_cast<decltype(this)>(arg)->task(); },
                "Encoder", ENCODER_STACK_SIZE, this, ENCODER_PRIORITY, NULL);
    return true;
  }
#elif KERISE_SELECT == 5
  bool begin(spi_host_device_t spi_host, std::array<gpio_num_t, 2> pins_cs) {
    if (!ma[0].begin(spi_host, pins_cs[0])) {
      loge << "Encoder L init failed :(" << std::endl;
      return false;
    }
    if (!ma[1].begin(spi_host, pins_cs[1])) {
      loge << "Encoder R init failed :(" << std::endl;
      return false;
    }
    xTaskCreate([](void *arg) { static_cast<decltype(this)>(arg)->task(); },
                "Encoder", ENCODER_STACK_SIZE, this, ENCODER_PRIORITY, NULL);
    return true;
  }
#endif
  int get_raw(uint8_t ch) const { return pulses[ch]; }
  float get_position(uint8_t ch) const {
    /* the reason the type of pulses is no problem with type int */
    /* estimated position 1,000 [mm/s] * 10 [min] * 60 [s/min] = 600,000 [mm] */
    /* int32_t 2,147,483,647 / 16384 * 1/3 * 3.1415 * 13 [mm] = 1,784,305 [mm]*/
    return positions[ch];
  }
  void clearOffset() { pulses_ovf[0] = pulses_ovf[1] = 0; }
  void samplingSemaphoreTake(TickType_t xBlockTime = portMAX_DELAY) {
    xSemaphoreTake(sampling_end_semaphore, xBlockTime);
  }
  void csv() {
    std::cout << "0," << get_position(0) << "," << get_position(1) << std::endl;
    // std::cout << "0," << get_raw(0) << "," << get_raw(1) << std::endl;
  }
  friend std::ostream &operator<<(std::ostream &os, const Encoder &e) {
    return os << "Encoder: position (" //
              << std::setw(6) << std::setfill(' ') << e.get_position(0) << ", "
              << std::setw(6) << std::setfill(' ') << e.get_position(1) << ")"
              << " pulses (" //
              << std::setw(6) << std::setfill(' ') << e.get_raw(0) << ", "
              << std::setw(6) << std::setfill(' ') << e.get_raw(1) << ")";
  }

private:
#if KERISE_SELECT == 3 || KERISE_SELECT == 4
  AS5048A_DUAL as;
#elif KERISE_SELECT == 5
  MA730 ma[2];
#endif
  SemaphoreHandle_t sampling_end_semaphore;
  const float gear_ratio;
  const float wheel_diameter;
  int pulses[2];
  int pulses_prev[2];
  int pulses_ovf[2];
  float positions[2];

  void update() {
#if KERISE_SELECT == 3 || KERISE_SELECT == 4
    int pulses_size = AS5048A_DUAL::PULSES_SIZE;
    as.update();
    for (int i = 0; i < 2; i++)
      pulses[i] = as.get(i);
#elif KERISE_SELECT == 5
    int pulses_size = MA730::PULSES_SIZE;
    for (int i = 0; i < 2; i++) {
      ma[i].update();
      pulses[i] = ma[i].get();
    }
#endif
    for (int i = 0; i < 2; i++) {
      /* count overflow */
      if (pulses[i] > pulses_prev[i] + pulses_size / 2) {
        pulses_ovf[i]--;
      } else if (pulses[i] < pulses_prev[i] - pulses_size / 2) {
        pulses_ovf[i]++;
      }
      pulses_prev[i] = pulses[i];
      /* calculate position */
      positions[i] = (pulses_ovf[i] + float(pulses[i]) / pulses_size) *
                     wheel_diameter * M_PI * gear_ratio;
    }
    /* fix rotation direction */
#if KERISE_SELECT == 3 || KERISE_SELECT == 4
    positions[1] *= -1;
#elif KERISE_SELECT == 5
    positions[0] *= -1;
#endif
  }
  void task() {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1));
      update();
      xSemaphoreGive(sampling_end_semaphore);
    }
  }
};
