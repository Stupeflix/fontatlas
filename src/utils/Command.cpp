
#include <iostream>
#include "utils/Command.hpp"

namespace utils {

Command::Command() {}

Command::~Command() {}

bool Command::parse(int argc, char **argv) {
  for (size_t i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    size_t n = arg.find('=');
    if (n != std::string::npos) {
      std::string name = arg.substr(0, n);
      if (_options.find(name) == _options.end())
        throw std::runtime_error("unknown option " + name + ".");
      _options[name] = arg.substr(n + 1);
    } else if (arg == "help") {
      _displayHelp(argv[0]);
      return false;
    }
    else
      _args.push_back(arg);
  }
  return true;
}

void Command::_displayHelp(std::string const &name) {
  std::cout << "Usage: " << name << " " << _synopsis << std::endl;
  std::cout << std::endl;
  if (!_description.empty()) {
    std::cout << "Descrition:" << std::endl;
    std::cout << "  " << _description << std::endl;
    std::cout << std::endl;
  }
  if (!_options.empty()) {
    std::cout << "Options:" << std::endl;
    for (auto const &o : _optionsDesc)
      std::cout << "  " << o.first << " -- " << o.second << std::endl;
  }
}

}