/**
 * @file
 * @brief Описание приложения.
 */

#pragma once

//

#include <memory>

//

#include "CommandInterpreter.hpp"

//

#include "TimerManager.hpp"

#include "BitField.hpp"

//

#include "ApplicationContext.hpp"

//

// Подсистемы.

#include "SubsystemManager.hpp"

//

namespace app {
/// @brief Приложение.
class Application final {
public:
  /// @brief
  static Application *getInstance() {
    static Application instance{};
    return &instance;
  }

  /// @brief Инициализация.
  /// @details
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  /// @return Состояние выполнения.
  int init(int argc, char *argv[]);
  /// @brief Деинициализация.
  /// @details
  void deinit();

  /// @brief Выполнение.
  /// @return Состояние выполнения.
  int exec();

private:
  /// @brief Конструктор.
  Application() = default;
  ~Application();

  /// @brief Подготовка при инициализации.
  /// @details
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  bool prepare(int argc, char *argv[]);

  /// @brief Вывод информации о приложении.
  void printInfo() const {
    LOG("Информация о приложении:");

    LOG("Мажорная версия: ", 0);
    LOG("Минорная версия: ", 0);
    LOG("Номер сборки: ", 0);

    LOG("Дата сборки: ", __DATE__);
    LOG("Время сборки: ", __TIME__);

    SEPARATOR;
  }

private:
  /// @brief Контекст приложения.
  std::unique_ptr<app::ApplicationContext> applicationContext_;
  /// @brief Интерпретатор команд.
  std::unique_ptr<cmd::CommandInterpreter> commandInterpreter_;
  /// @brief Менеджер подсистем.
  std::unique_ptr<subsystemManager::SubsystemManager> subsystemManager_;

  /// @brief Менеджер таймера.
  TimerManager timerManager_;
};
} // namespace app
