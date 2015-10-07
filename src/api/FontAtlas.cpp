
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#include "utils/Path.hpp"
#include "core/Font.hpp"
#include "core/Distmap.hpp"
#include "core/MetaData.hpp"
#include "api/FontAtlas"

namespace osgStupeflix {

const std::size_t FontAtlas::DEFAULT_CHARACTER_RESOLUTION = 80;
const std::size_t FontAtlas::DEFAULT_PADDING = 20;
const std::size_t FontAtlas::DEFAULT_IMG_SIZE = 1024;

void FontAtlas::generateFontData(std::string const &font_path,
                                 std::string const &data_path,
                                 std::size_t character_resolution,
                                 std::size_t padding,
                                 std::size_t img_size,
                                 bool verbose) {

  core::Atlas atlas(img_size, img_size);
  core::Font font(font_path, character_resolution);
  std::size_t currentOffset = 0;
  std::size_t prevOffset = 0;
  std::size_t i = 1;

  font.setPadding(padding);

    if (verbose)
      std::cout << "Font size is " << font.getSize() << std::endl;

  while (currentOffset < font.getSize()) {

    /* Generate atlas */

    std::string atlasPath = font_path + "." +
      utils::convert<std::string>(i) + ".png";
    currentOffset = font.generate(atlas, currentOffset);

    if (verbose)
      std::cout << "Generated " << atlasPath << " - offset is now " << currentOffset << std::endl;
    prevOffset = currentOffset;

    /* Generate distmap and save it to a file */

    core::Distmap distmap(img_size, img_size);
    distmap.generate(atlas);
    distmap.saveToPng(atlasPath);

    atlas.clear();
    ++i;
  }

  /* Save meta data */

  if (verbose)
    std::cout << "Generated " << font_path << ".json" << std::endl;
  std::ofstream jsonFile((font_path + ".json").c_str());
  jsonFile << font.toJson();
  jsonFile.close();

}

void FontAtlas::generateMetaData(std::string const &font_path,
                                 std::string const &data_path,
                                 std::size_t character_resolution,
                                 std::size_t padding,
                                 std::size_t img_size,
                                 bool verbose) {


  core::MetaData meta_data;
  core::Atlas atlas(img_size, img_size);
  core::Font font(font_path, character_resolution);

  font.setPadding(padding);

  if (verbose) {
    std::cout << "Compute meta data...";
    std::cout.flush();
  }
  std::size_t currentOffset = 0;
  std::size_t prevOffset = 0;
  std::size_t i = 1;
  while (currentOffset < font.getSize()) {

    if (verbose) {
      std::cout << ".";
      std::cout.flush();
    }

    /* Generate atlas */

    std::string atlasPath = data_path + "." +
      utils::convert<std::string>(i) + ".png";

    prevOffset = currentOffset;
    currentOffset += font.generate(atlas, currentOffset);
    meta_data.addRow(font.getCache(), prevOffset, currentOffset);
    atlas.clear();
    ++i;
  }

  /* Save meta data */

  if (verbose)
    std::cout << std::endl << "Generated " << data_path << ".meta" << std::endl;
  meta_data.save(data_path + ".meta");

}

unsigned short FontAtlas::generateFromChar(unsigned short c,
                                           std::string const &font_path,
                                           std::string const &data_path,
                                           std::size_t character_resolution,
                                           std::size_t padding,
                                           std::size_t img_size) {

  /* Load meta-data */

  core::MetaData meta_data;
  std::string meta_file = data_path + ".meta";
  if (!meta_data.load(meta_file))
    throw std::runtime_error("Cannot find meta-data file " + meta_file);

  /* Initialize font tools */

  core::Atlas atlas(img_size, img_size);
  core::MetaData::Row const &row = meta_data.getRowFromChar(c);
  std::string out_path = data_path + "." + utils::convert<std::string>(row.index);

  /* Just return the index and do nothing when the atlas has already been generated */

  if (access((out_path + ".png").c_str(), F_OK) != -1)
    return row.index;

  /* Generate atlas from font */

  core::Font font(font_path, character_resolution, row.chars);
  font.setPadding(padding);
  font.generate(atlas, row.from);

  /* Generate distmap and save it to a file */

  core::Distmap distmap(img_size, img_size);
  distmap.generate(atlas);
  distmap.saveToPng(out_path + ".png");

  /* Save meta data */

  std::ofstream jsonFile((out_path + ".json").c_str());
  jsonFile << font.toJson();
  jsonFile.close();

  return row.index;
}

float FontAtlas::getKerning(unsigned short first,
                unsigned short second,
                std::string const &font_path,
                std::size_t character_resolution) {
  return ft::Face(font_path, character_resolution).getKerning(first, second);
}


} // namespace osgStupeflix
