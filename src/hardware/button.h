/**
 * @file button.h
 * @brief Button Driver
 * @author Ryotaro Onuki <kerikun11+github@gmail.com>
 * @date 2021-11-21
 * @copyright Copyright 2021 Ryotaro Onuki <kerikun11+github@gmail.com>
 */
#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "config/config.h"

namespace hardware {

class Button {
 public:
  Button() {}
  bool init(const gpio_num_t pin) {
    pin_ = pin;
    ESP_ERROR_CHECK(gpio_reset_pin(pin));
    ESP_ERROR_CHECK(gpio_set_direction(pin, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY));
    flags = 0x00;
    xTaskCreatePinnedToCore(
        [](void* arg) { static_cast<decltype(this)>(arg)->task(); }, "Button",
        configMINIMAL_STACK_SIZE, this, TASK_PRIORITY_BUTTON, NULL,
        TASK_CORE_ID_BUTTON);
    return true;
  }
  union {
    uint8_t flags; /**< all flags */
    struct {
      uint8_t pressed : 1;         /**< pressed */
      uint8_t long_pressed_1 : 1;  /**< long-pressed level 1 */
      uint8_t long_pressed_2 : 1;  /**< long-pressed level 2 */
      uint8_t long_pressed_3 : 1;  /**< long-pressed level 3 */
      uint8_t pressing : 1;        /**< pressing */
      uint8_t long_pressing_1 : 1; /**< long-pressing level 1 */
      uint8_t long_pressing_2 : 1; /**< long-pressing level 2 */
      uint8_t long_pressing_3 : 1; /**< long-pressing level 3 */
    };
  };

 private:
  static constexpr int button_sampling_time_ms = 20;
  static constexpr int button_time_press = 1;
  static constexpr int button_time_long_press_1 = 20;
  static constexpr int button_time_long_press_2 = 100;
  static constexpr int button_time_long_press_3 = 500;

 private:
  gpio_num_t pin_;
  int counter_ = 0;

  void update() {
    if (gpio_get_level(pin_) == 0) {
      if (counter_ < button_time_long_press_3 + 1) counter_++;
      if (counter_ == button_time_long_press_3) long_pressing_3 = 1;
      if (counter_ == button_time_long_press_2) long_pressing_2 = 1;
      if (counter_ == button_time_long_press_1) long_pressing_1 = 1;
      if (counter_ == button_time_press) pressing = 1;
    } else {
      if (counter_ >= button_time_long_press_3)
        long_pressed_3 = 1;
      else if (counter_ >= button_time_long_press_2)
        long_pressed_2 = 1;
      else if (counter_ >= button_time_long_press_1)
        long_pressed_1 = 1;
      else if (counter_ >= button_time_press)
        pressed = 1;
      counter_ = 0;
      flags &= 0x0F;
    }
  }
  void task() {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(button_sampling_time_ms));
      update();
    }
  }
};

};  // namespace hardware
