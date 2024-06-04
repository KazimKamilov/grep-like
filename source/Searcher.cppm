export module searcher;

import <iostream>;
import <string>;
import <vector>;
import <filesystem>;
import <fstream>;

using file_path = std::filesystem::path;

static void processFile(const file_path& filepath)
{

}

export inline void searchSubstr(uint32_t job_count, const std::string& substr, file_path&& search_directory)
{
	if (std::filesystem::exists(search_directory))
	{
		std::cout << "run search" << std::endl;

		std::vector<file_path> files;

		auto root_iter{ std::filesystem::recursive_directory_iterator(search_directory) };

		for (const auto& entry : root_iter)
		{
			if (entry.is_regular_file())
				files.emplace_back(entry.path());
		}

		const auto total_files{ files.size() };

		if (total_files >= job_count)
		{
			const auto files_per_thread{ total_files / job_count };
		}
		else
		{

		}

		int y{0};
	}
	else
	{
		std::cout << "There is no directory path \"" << search_directory << "\"!" << std::endl;
	}
}
