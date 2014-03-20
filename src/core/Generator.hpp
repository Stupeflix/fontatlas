
#pragma once

namespace core {

class Generator {
public:
    static void generate(std::string const &path,
                         std::string const &output_dir,
                         std::size_t resolution = 50,
                         std::size_t padding = 20,
                         std::size_t size = 1024,
                         bool verbose = false,
                         bool generate_distmap = true);
};

}