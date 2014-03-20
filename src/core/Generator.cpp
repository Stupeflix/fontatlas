
#include <iostream>
#include <fstream>
#include "core/Font.hpp"
#include "core/Distmap.hpp"
#include "core/Generator.hpp"

namespace core {

void Generator::generate(std::string const &path,
                           std::string const &output_dir,
                           std::size_t resolution,
                           std::size_t padding,
                           std::size_t size,
                           bool verbose,
                           bool generate_distmap) {

  /* Generate result path */
  std::string out_path;
  size_t slash = path.find_last_of('/');
  if (slash != std::string::npos)
    out_path = path.substr(slash + 1);
  else
    out_path = path;
  out_path = output_dir + "/" + out_path;

  /* Generate texture font */
  core::Atlas atlas(size, size);
  core::Font font(path, resolution);
  font.setPadding(padding);

  size_t currentOffset = 0;
  size_t i = 1;

  while (currentOffset < font.getSize()) {

    /* Generate atlas */
    std::string atlasPath = out_path + "." +
      utils::convert<std::string>(i) + ".png";
    if (verbose)
      std::cout << "Generated " << atlasPath << std::endl;
    currentOffset = font.generate(atlas, currentOffset);

    /* Generate distmap and save it to a file */
    if (generate_distmap) {
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
  if (verbose)
    std::cout << "Generated " << out_path << ".json" << std::endl;
  std::ofstream jsonFile(out_path + ".json");
  jsonFile << font.toJson();
  jsonFile.close();
}

}