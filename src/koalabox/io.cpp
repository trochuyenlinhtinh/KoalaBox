#include <koalabox/io.hpp>
#include <koalabox/logger.hpp>
#include <koalabox/util.hpp>

#include <miniz.h>

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

    bool write_file(const Path& file_path, const String& contents) noexcept {
        try {
            if (!std::filesystem::exists(file_path)) {
                std::filesystem::create_directories(file_path.parent_path());
            }

            std::ofstream output_stream(file_path);
            if (output_stream.good()) {
                output_stream << contents;

                LOG_DEBUG(R"(Writing file to disk: "{}")", file_path.string())
                return true;
            }

            LOG_ERROR(
                R"(Error opening output stream: "{}". Flags: {})",
                file_path.string(), output_stream.flags()
            )
            return false;
        } catch (const Exception& e) {
            LOG_ERROR("Unexpected exception caught: {}", e.what())
            return false;
        }
    }

    bool unzip_file(
        const Path& source_zip,
        const String& target_file,
        const Path& destination_dir
    ) noexcept {
        try {
            // open the zip archive for reading
            DECLARE_STRUCT(mz_zip_archive, archive);
            if (!mz_zip_reader_init_file(&archive, source_zip.string().c_str(), 0)) {
                throw KException("Error opening archive");
            }

            // get the index of the file in the archive
            mz_uint file_index = mz_zip_reader_locate_file(
                &archive,
                target_file.c_str(),
                nullptr,
                0
            );

            if (file_index == -1) {
                mz_zip_reader_end(&archive);
                throw KException("File not found in archive");
            }

            // get the uncompressed size of the file
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&archive, file_index, &file_stat)) {
                mz_zip_reader_end(&archive);
                throw KException("Error getting file stats");
            }

            // allocate a buffer to hold the uncompressed file data
            void* file_data = malloc(file_stat.m_uncomp_size);
            if (!file_data) {
                // error allocating buffer
                mz_zip_reader_end(&archive);
                throw KException("Error allocating buffer");
            }

            // read the file data into the buffer
            if (!mz_zip_reader_extract_to_mem(
                &archive,
                file_index,
                file_data,
                file_stat.m_uncomp_size,
                0
            )) {
                free(file_data);
                mz_zip_reader_end(&archive);
                throw KException("Error extracting file");
            }

            // write the file data to disk
            const auto destination_path = destination_dir / Path(target_file).filename();
            FILE* fp = fopen(destination_path.string().c_str(), "wb");
            if (!fp) {
                // error creating file
                free(file_data);
                mz_zip_reader_end(&archive);
                throw KException("Error creating file");
            }
            fwrite(file_data, 1, file_stat.m_uncomp_size, fp);
            fclose(fp);

            // clean up
            free(file_data);
            mz_zip_reader_end(&archive);

            return true;
        } catch (const Exception& e) {
            LOG_ERROR(
                R"(Error unzipping file "{}" from "{}" to "{}": {})",
                target_file, source_zip.string(), destination_dir.string(), e.what()
            )

            return false;
        }
    }
}
