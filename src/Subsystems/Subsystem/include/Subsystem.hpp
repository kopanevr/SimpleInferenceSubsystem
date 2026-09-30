/**
 * @file
 * @brief Описание подсистемы.
 */

#pragma once

//

#include <cassert>
#include <cstdint>

//

#include <string>
#include <string_view>

//

#include "SubsystemId.hpp"

//

/// @brief
#define SET_SUBSYSTEM_ID(identifier) subsystemHandle_.id = identifier

/// @brief
#define SET_SUBSYSTEM_NAME(subsystemName)                                      \
  static_assert(std::string_view(subsystemName).size() <                       \
                std::string{}.capacity());                                     \
  subsystemHandle_.name = subsystemName

struct SubsystemHandle {
  /// @brief Идентификатор подсистемы.
  subsystemManager::SubsystemId id;
  /// @brief Имя подсистемы.
  /// @warning
  std::string name;

  /// @brief Состояние запуска подсистемы.
  bool isStarted : 1;
};

/// @brief Подсистема.
class Subsystem {
public:
  /// @brief Конструктор.
  Subsystem() = default;

  /// @brief Деструктор.
  virtual ~Subsystem() = default;

  /// @brief Запуск подсистемы.
  bool startUp() {
    if (subsystemHandle_.isStarted) {
      return false;
    }
    if (!setBeforeStartUp()) {
      return false;
    }
    subsystemHandle_.isStarted = true;
    return true;
  }

  /// @brief Остановка подсистемы.
  void shutDown() {
    if (!subsystemHandle_.isStarted) {
      return;
    }
    setBeforeShutDown();
    subsystemHandle_.isStarted = false;
  }

  /// @brief Возвращает идентификатор подсистемы.
  [[nodiscard]] subsystemManager::SubsystemId getId() const {
    return subsystemHandle_.id;
  }

  /// @brief Проверка запуска подсистемы.
  [[nodiscard]] bool isRunning() const { return subsystemHandle_.isStarted; }

  /// @brief Основной процесс.
  /// @details Вызывается в главном потоке.
  void process() { processBody(); }

protected:
  /// @brief Дескриптор подсистемы.
  SubsystemHandle subsystemHandle_;

protected:
  /// @brief Инициализация подсистемы.
  virtual void init() = 0;
  /// @brief
  virtual bool setBeforeStartUp() = 0;
  /// @brief
  virtual void setBeforeShutDown() = 0;
  /// @brief Тело основного цикла.
  /// @details Вызывается в @ref process.
  virtual void processBody() = 0;

private:
};
