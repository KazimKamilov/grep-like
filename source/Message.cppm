export module msg;

import <iostream>;
import <format>;
import <mutex>;

std::mutex mtx;

export template<typename... Args>
inline void message(std::string_view&& message, Args&&... args)
{
	std::lock_guard lock(mtx);

	std::cout << std::vformat(message, std::make_format_args(args...)) << std::endl;
}
