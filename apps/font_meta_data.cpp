
#include <iostream>
#include "api/FontAtlas"
#include "utils/Command.hpp"

int main(int argc, char **argv) {
  utils::Command cmd;

  /* Start application */
  try {
    cmd
      .setSynopsis("<font_path> option...")
      .setDescription("Generate a binary meta-data to use with generateFromChar "
                      "function.")
      .addOpt("character_resolution", 50, "size of the characters")
      .addOpt("padding", 20, "minimal space between the characters")
      .addOpt("img_size", 1024, "size of the generated png")
      .addOpt("verbose", true, "output some information");
    if (!cmd.parse(argc, argv))
      return 1;

  osgStupeflix::FontAtlas::generateMetaData(
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
