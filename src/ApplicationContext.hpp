/**
 * @file
 * @brief Содержит описание контекста приложения.
 */

#pragma once

//

#include <cstdint>

//

#include <chrono>

//

namespace app {
/// @brief
struct ApplicationContext final {
  /// @brief Время запуска.
  std::chrono::steady_clock::time_point startTime;
  /// @brief Период времени выполнения.
  std::chrono::steady_clock::duration executedTime;

  /// @brief Состояние.
  enum class State : uint8_t {
    Uninitialized,
    Ready,
    Running,
    Deinitialized
  } state{State::Uninitialized};
};
} // namespace app
