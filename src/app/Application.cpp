
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

  /* Generate result path */
  size_t slash = path.find_last_of('/');
  if (slash != std::string::npos)
    path = path.substr(slash + 1);
  path = cmd.getOption<std::string>("output_dir") + "/" + path;

  /* Generate distmap and save it to a file */
  if (cmd.getOption<bool>("distmap")) {
    core::Distmap distmap(size, size);
    distmap.generate(atlas);
    distmap.saveToPng(path + ".png");
  } else {
    atlas.saveToPng(path + ".png");
  }

  /* Save meta data */
  std::ofstream jsonFile(path + ".json");
  jsonFile << font.toJson();
  jsonFile.close();

}