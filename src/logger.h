#pragma once

#include <ostream>
#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Logger {
public:
  Logger() {}
  auto clear() { return buf.clear(); }
  auto push(const std::vector<float> &data) { return buf.push_back(data); }
  auto size() const { return buf.size(); }
  auto reserve(std::size_t n) { return buf.reserve(n); }
  void print(std::ostream &os = std::cout) const {
    for (const auto &data : buf) {
      bool first = true;
      for (const auto &value : data) {
        if (!first)
          os << "\t";
        if (first)
          first = false;
        os << value;
      }
      os << std::endl;
      vTaskDelay(1);
    }
  }

private:
  std::vector<std::vector<float>> buf;
};