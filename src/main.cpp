
#include <iostream>
#include "app/Application.hpp"
#include "utils/Command.hpp"

int main(int argc, char **argv) {
  Application app;
  utils::Command cmd;

  /* Start application */
  try {
    cmd
      .setSynopsis("<font_path> option...")
      .setDescription("Simple command line tool to generate distmap.")
      .addOption("size", 1024, "size of the generated png")
      .addOption("resolution", 50, "size of the characters")
      .addOption("padding", 20, "minimal space between the characters.")
      .addOption("output_dir", ".", "target directory of the generated file.");
    if (!cmd.parse(argc, argv))
      return 1;

    app.start(cmd);

  } catch (std::exception const &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
