
#include <iostream>
#include <fstream>
#include "utils/Path.hpp"
#include "core/Font.hpp"
#include "core/Distmap.hpp"
#include "core/MetaData.hpp"
#include "api/FontAtlas"

namespace osgStupeflix {

namespace fontAtlas {

void generateFontData(std::string const &font_path,
                      std::string const &output_dir,
                      DataType dataType,
                      std::size_t font_size,
                      std::size_t padding,
                      std::size_t size,
                      bool verbose,
                      bool generate_distmap) {


  core::MetaData meta_data;
  std::string out_path = output_dir + "/" + utils::getFileName(font_path);
  core::Atlas atlas(size, size);
  core::Font font(font_path, font_size);
  size_t currentOffset = 0;
  size_t prevOffset = 0;
  size_t i = 1;

  font.setPadding(padding);

  if (dataType == META_DATA && verbose) {
    std::cout << "Compute meta data...";
    std::cout.flush();
  }

  while (currentOffset < font.getSize()) {

    /* Generate atlas */

    std::string atlasPath = out_path + "." +
      utils::convert<std::string>(i) + ".png";
    if (verbose && dataType == FONT_DATA)
      std::cout << "Generated " << atlasPath << std::endl;
    else if (verbose && dataType == META_DATA) {
      std::cout << ".";
      std::cout.flush();
    }
    prevOffset = currentOffset;
    currentOffset = font.generate(atlas, currentOffset);
    if (dataType == META_DATA)
      meta_data.addRow(font.getCache(), prevOffset, currentOffset);

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

  if (dataType == FONT_DATA) {
    if (verbose)
      std::cout << "Generated " << out_path << ".json" << std::endl;
    std::ofstream jsonFile(out_path + ".json");
    jsonFile << font.toJson();
    jsonFile.close();
  } else if (dataType == META_DATA) {
    if (verbose)
      std::cout << std::endl << "Generated " << out_path << ".meta" << std::endl;
    meta_data.save(out_path + ".meta");
  }
}

void generateFromChar(wchar_t c,
                      std::string const &font_path,
                      std::string const &meta_dir,
                      std::string const &output_dir,
                      std::size_t font_size,
                      std::size_t padding,
                      std::size_t size) {
  core::MetaData meta_data;
  std::string meta_file = meta_dir + "/" + utils::getFileName(font_path) + ".meta";
  std::string out_path = output_dir + "/" + utils::getFileName(font_path);

  if (!meta_data.load(meta_file))
    throw std::runtime_error("Cannot find meta-data file " + meta_file);

  /* Initialize font tools */

  core::Atlas atlas(size, size);
  core::Font font(font_path, font_size, meta_data.getRowFromChar(c));
  size_t currentOffset = 0;
  size_t prevOffset = 0;
  size_t i = 1;

  font.setPadding(padding);

  while (currentOffset < font.getSize()) {

    /* Generate atlas */

    std::string atlasPath = out_path + "." +
      utils::convert<std::string>(i) + ".png";
    prevOffset = currentOffset;
    currentOffset = font.generate(atlas, currentOffset);
    /* Generate distmap and save it to a file */

    core::Distmap distmap(size, size);
    distmap.generate(atlas);
    distmap.saveToPng(atlasPath);

    atlas.clear();
    ++i;
  }

  /* Save meta data */

  std::ofstream jsonFile(out_path + ".json");
  jsonFile << font.toJson();
  jsonFile.close();


}

float getKerning(wchar_t first,
                wchar_t second,
                std::string const &font_path,
                std::size_t font_size) {
  return ft::Face(font_path, font_size).getKerning(first, second);
}


} // namespace fontAtlas

} // namespace osgStupeflix