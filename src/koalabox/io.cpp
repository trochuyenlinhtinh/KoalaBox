#include <koalabox/io.hpp>
#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>

#include <fstream>

namespace koalabox::io {

    String read_file(const Path& file_path) {
        std::ifstream input_stream(file_path);
        input_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            return { std::istreambuf_iterator<char>{ input_stream }, {}};
        } catch (const std::system_error& e) {
            const auto& code = e.code();
            throw std::runtime_error(
                fmt::format("Input file stream error code: {}, message: {}", code.value(),
                    code.message())
            );
        }
    }
}
