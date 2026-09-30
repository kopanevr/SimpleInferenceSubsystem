/**
 * @file
 * @brief Описание интерпретатора команд.
 */

//

#include <cassert>

//

namespace app {
class Application;
} // namespace app

//

namespace cmd {
/// @brief Аргументы.
struct CommandLineArgs final {};

//

/// @brief Интерпретатор команд.
class CommandInterpreter final {
public:
  /// @brief Деструктор.
  ~CommandInterpreter() = default;

  /// @brief
  /// @return
  static CommandInterpreter &getInstance() { return *instance_; }

  /// @brief Возвращает ссылку на аргументы.
  /// @return Ссылка на аргументы.
  const cmd::CommandLineArgs &getArgs() const { return args_; }

  /// @brief
  /// @return
  bool isParsed() const { return isSuccessfullyParsed_; }

private:
  /// @brief Конструктор.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  CommandInterpreter(int argc, char *argv[]) { process(argc, argv); }

  /// @brief Дружественный класс.
  friend class app::Application;

  /// @brief Тело процесса.
  /// @param argc Количество аргументов.
  /// @param argv Указатель на список аргументов.
  void bodyProcess(int argc, char *argv[]);

  /// @brief Процесс.
  void process(int argc, char *argv[]) { bodyProcess(argc, argv); }

private:
  /// @brief Указатель на экземпляр.
  static inline CommandInterpreter *instance_;

  /// @brief Аргументы.
  cmd::CommandLineArgs args_;

  /// @brief Состояние обработки команд.
  bool isSuccessfullyParsed_ : 1;
};
} // namespace cmd
