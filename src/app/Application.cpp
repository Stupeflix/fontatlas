
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

  /* Generate result path */
  std::string out_path;
  size_t slash = path.find_last_of('/');
  if (slash != std::string::npos)
    out_path = path.substr(slash + 1);
  else
    out_path = path;
  out_path = cmd.getOption<std::string>("output_dir") + "/" + out_path;

  /* Generate texture font */
  core::Atlas atlas(size, size);
  core::Font font(path,
      cmd.getOption<std::size_t>("resolution"));
  font.setPadding(cmd.getOption<std::size_t>("padding"));

  size_t currentOffset = 0;
  size_t i = 1;

  while (currentOffset < font.getSize()) {

    /* Generate atlas */
    std::string atlasPath = out_path + "." +
      utils::convert<std::string>(i) + ".png";
    if (cmd.getOption<bool>("verbose"))
      std::cout << "Generate " << atlasPath << std::endl;
    currentOffset = font.generate(atlas, currentOffset);

    /* Generate distmap and save it to a file */
    if (cmd.getOption<bool>("distmap")) {
      core::Distmap distmap(size, size);
      distmap.generate(atlas);
      distmap.saveToPng(atlasPath);
    } else {
      atlas.saveToPng(atlasPath);
    }

    atlas.clear();
    ++i;
  }

  /* Save meta data */
  if (cmd.getOption<bool>("verbose"))
    std::cout << "Generate " << out_path << ".json" << std::endl;
  std::ofstream jsonFile(out_path + ".json");
  jsonFile << font.toJson();
  jsonFile.close();

}