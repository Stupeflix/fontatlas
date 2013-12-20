
#include <iostream>
#include <fstream>
#include "core/Font.hpp"
#include "core/Distmap.hpp"
#include "app/Application.hpp"

Application::Application() {}

Application::~Application() {}

void Application::start(utils::Command const &cmd) {

  std::size_t size = cmd.getOption<std::size_t>("size");
  std::string path = cmd.getArg<std::string>(0);

  /* Generate texture font */
  core::Atlas atlas(size, size);
  core::Font font(path,
      cmd.getOption<std::size_t>("resolution"));
  font.setPadding(cmd.getOption<std::size_t>("padding"));
  font.generate(atlas);

  /* Generate distmap and save it to a file */
  core::Distmap distmap(size, size);
  distmap.generate(atlas);
  std::string prefix = cmd.getOption<std::string>("output_dir") + "/";
  size_t slash = path.find_last_of('/');
  if (slash != std::string::npos)
    path = path.substr(slash + 1);
  distmap.saveToPng(prefix + path + ".png");

  /* Save meta data */
  std::ofstream jsonFile(prefix + path + ".json");
  jsonFile << font.toJson();
  jsonFile.close();

}