
#include <iostream>
#include "api/FontAtlas"
#include "utils/Command.hpp"

int main(int argc, char **argv) {
  utils::Command cmd;

  /* Start application */
  try {
    cmd
      .setSynopsis("<font_path> option...")
      .setDescription("Generate atlas files from the given font, and a json file"
                      " of meta-data to use it.")
      .addOpt(
          "character_resolution",
          osgStupeflix::FontAtlas::DEFAULT_CHARACTER_RESOLUTION,
          "size of the characters in pixel")
      .addOpt(
          "padding",
          osgStupeflix::FontAtlas::DEFAULT_PADDING,
          "minimal space between the characters")
      .addOpt(
          "img_size",
          osgStupeflix::FontAtlas::DEFAULT_IMG_SIZE,
          "size of the generated png")
      .addOpt(
          "verbose",
          true,
          "output some information");
    if (!cmd.parse(argc, argv))
      return 1;

  osgStupeflix::FontAtlas::generateFontData(
    cmd.getArg<std::string>(0),
    cmd.getArg<std::string>(0),
    cmd.getOpt<std::size_t>("character_resolution"),
    cmd.getOpt<std::size_t>("padding"),
    cmd.getOpt<std::size_t>("img_size"),
    cmd.getOpt<bool>("verbose")
  );

  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
