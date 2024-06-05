export module searcher;

import <string>;
import <vector>;
import <filesystem>;
import <fstream>;
import <thread>;
import <mutex>;
import <algorithm>;

import msg;


using file_path = std::filesystem::path;
using thread_pool = std::vector<std::thread>;

/**
    Opens the file and searches for a substring in it.
*/
inline void processFile(const std::string& substr, const file_path& filepath)
{
    std::fstream file(filepath, std::ios::in | std::ios::binary | std::ios::ate);

    if (file.good())
    {
        const auto file_size{ file.tellp() };
        std::vector<char> filebuf(file_size);

        file.seekp(std::ios::beg);

        file.read(filebuf.data(), file_size);

        if (std::search(filebuf.cbegin(), filebuf.cend(), substr.cbegin(), substr.cend()) != filebuf.cend())
            message("Found in file: \"{}\"", filepath.string());
    }
    else
    {
        message("Cannot open \"{}\" file to check!", filepath.string());
    }

    file.close();
}

/**
    Recursively checks the directory and creates a list
    of files, then divides them between the specified
    number of threads and starts the search process.
*/
export inline void searchSubstr(uint32_t job_count, const std::string& substr, file_path&& search_directory)
{
    thread_pool pool;

    // First, we check for target directory
    if (std::filesystem::exists(search_directory))
    {
        // Our list with all files
        std::vector<file_path> files;

        // Iterator for all files
        auto root_iter{ std::filesystem::recursive_directory_iterator(search_directory) };

        // Here we push items inside list
        for (const auto& entry : root_iter)
        {
            if (entry.is_regular_file())
                files.emplace_back(entry.path());
        }

        const auto total_files{ static_cast<uint32_t>(files.size()) };

        /**
            We split files between the specified threads, or create the necessary number of them
        */
        if (total_files >= job_count)
        {
            const auto files_per_thread{ total_files / job_count };
            const auto files_per_thread_tail{ total_files % job_count };

            std::vector<std::vector<file_path>> jobs_paths(static_cast<size_t>(job_count));

            auto begin_iter{ files.cbegin() };

            for (auto i{ 0u }; i < job_count; ++i)
            {
                // Copy iter
                auto iter_end{ begin_iter };

                // Move end iter
                std::advance(iter_end, files_per_thread);

                jobs_paths.at(i) = std::vector<file_path>(begin_iter, iter_end);

                // Move begin iter
                std::advance(begin_iter, files_per_thread);
            }

            // We also add the tail files
            uint32_t iter{ 0u }, thread_id{ 0u };
            const auto target_count{ total_files - files_per_thread_tail };

            while (iter < files_per_thread_tail)
            {
                jobs_paths.at(thread_id).emplace_back(files.at(target_count + iter));

                ++thread_id;

                if (thread_id >= job_count)
                    thread_id = 0u;

                ++iter;
            }

            // Creating threads and running them
            for (auto job_index{ 0u }; job_index < job_count; ++job_index)
            {
                pool.emplace_back(std::move(
                    std::thread([substr, vec{ jobs_paths.at(job_index) }]()
                    {
                        for (const auto& path : vec)
                            processFile(substr, path);
                    }
                )));
            }
        }
        else
        {
            // Just create the required number of threads
            for (const auto path : files)
            {
                pool.emplace_back(std::move(
                    std::thread(processFile, substr, path)
                ));
            }
        }
    }
    else // no target directory
    {
        message("There is no directory path \"{}\"", search_directory.string());
    }

    // We are waiting for the end of the search
    for (auto& thread : pool)
        thread.join();
}
