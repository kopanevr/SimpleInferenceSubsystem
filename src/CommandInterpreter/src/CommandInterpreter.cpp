#include "CommandInterpreter.hpp"

//

#include <getopt.h>

//

using namespace cmd;

//

/// @brief Тело процесса.
/// @details
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
void CommandInterpreter::bodyProcess(int argc, char *argv[]) {
  int opt = {};

  const char *shortopts = ":hp:v";

  bool status = {};

  while ((opt = getopt(argc, argv, shortopts)) != -1) {
    switch (opt) {
    case 'h':
      break;
    case 'p':
      status = true;
      break;
    case 'v':
      break;
    case '?':
      break;
    case ':':
      break;
    default:
      break;
    }
  }

  isSuccessfullyParsed_ = status;
}
