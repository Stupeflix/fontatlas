
#include <iostream>
#include "core/Generator.hpp"
#include "utils/Command.hpp"

int main(int argc, char **argv) {
  utils::Command cmd;

  /* Start application */
  try {
    cmd
      .setSynopsis("<font_path> option...")
      .setDescription("Simple command line tool to generate font atlas with"
                      " some parameters. The program will generate a "
                      "`{font_name}.ttf.json` meta-data file and a"
                      "`{font_name}.ttf.png` atlas file.")
      .addOpt("size", 1024, "size of the generated png")
      .addOpt("resolution", 50, "size of the characters")
      .addOpt("padding", 20, "minimal space between the characters")
      .addOpt("output_dir", ".", "target directory of the generated file")
      .addOpt("distmap", true, "whether to generate as distmap or not")
      .addOpt("verbose", true, "output some information")
      .addOpt("dataType", "META_DATA", "FONT_DATA to generate font json and png, META_DATA to generate json meta-data file");
    if (!cmd.parse(argc, argv))
      return 1;

  core::Generator::generate(
    cmd.getArg<std::string>(0),
    cmd.getOpt<std::string>("output_dir"),
    cmd.getOpt<std::string>("dataType") == "META_DATA" ?
        core::Generator::META_DATA : core::Generator::FONT_DATA,
    cmd.getOpt<std::size_t>("resolution"),
    cmd.getOpt<std::size_t>("padding"),
    cmd.getOpt<std::size_t>("size"),
    cmd.getOpt<bool>("verbose"),
    cmd.getOpt<bool>("distmap")
  );

  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
