
#pragma once

#include "utils/Command.hpp"

class Application {
 public:
  Application();
  ~Application();

  void start(utils::Command const &command);
};