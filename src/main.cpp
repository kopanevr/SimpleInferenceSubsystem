/**
 * @file
 * @brief
 */

//

#include "Application.hpp"

//

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[]) {
  auto *const app = app::Application::getInstance();
  int ret = 0;
  if (!(ret = app->init(argc, argv))) {
    return ret;
  }

  return app->exec();
}
