
#include <iostream>
#include <fstream>
#include "core/Font.hpp"
#include "core/Distmap.hpp"
#include "core/Generator.hpp"

namespace core {

void Generator::generate(std::string const &path,
                         std::string const &output_dir,
                         DataType dataType,
                         std::size_t resolution,
                         std::size_t padding,
                         std::size_t size,
                         bool verbose,
                         bool generate_distmap) {

  /* Generate result path */
  std::string meta_data;
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
  size_t prevOffset = 0;
  size_t i = 1;

  if (dataType == META_DATA) {
    meta_data = "[\n";
    if (verbose) {
      std::cout << "Compute meta data...";
      std::cout.flush();
    }
  }

  while (currentOffset < font.getSize()) {

    /* Generate atlas */

    std::string atlasPath = out_path + "." +
      utils::convert<std::string>(i) + ".png";
    if (verbose && dataType == FONT_DATA)
      std::cout << "Generated " << atlasPath << std::endl;
    if (verbose && dataType == META_DATA)
      std::cout << ".";
    std::cout.flush();
    prevOffset = currentOffset;
    currentOffset = font.generate(atlas, currentOffset);
    if (dataType == META_DATA) {
      meta_data += "  {\"offset\": " + utils::convert<std::string>(prevOffset);
      meta_data += ", \"cache\": " + font.cacheToJson(prevOffset, currentOffset);
      meta_data += "}";
      if (currentOffset != font.getSize())
        meta_data += ",";
      meta_data += "\n";
    }

    /* Generate distmap and save it to a file */

    if (generate_distmap) {
      core::Distmap distmap(size, size);
      distmap.generate(atlas);
      if (dataType == FONT_DATA)
        distmap.saveToPng(atlasPath);
    } else {
      if (dataType == FONT_DATA)
        atlas.saveToPng(atlasPath);
    }

    atlas.clear();
    ++i;
  }

  /* Save meta data */

  if (verbose && dataType == FONT_DATA)
    std::cout << "Generated " << out_path << ".json" << std::endl;
  else if (verbose && dataType == META_DATA)
    std::cout << std::endl << "Generated " << out_path << ".meta.json" << std::endl;

  if (dataType == FONT_DATA) {
    std::ofstream jsonFile(out_path + ".json");
    jsonFile << font.toJson();
    jsonFile.close();
  } else if (dataType == META_DATA) {
    std::ofstream jsonFile(out_path + ".meta.json");
    jsonFile << meta_data << "\n]";
    jsonFile.close();
  }
}

}