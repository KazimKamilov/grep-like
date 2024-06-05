import runner;
import msg;

import <chrono>;


int main(int argc, const char* argv[])
{
    const auto iter_begin{ std::chrono::steady_clock::now() };
    run_search(argc, argv);
    const auto iter_end{ std::chrono::steady_clock::now() };

    const auto iter_elapsed_time{ std::chrono::duration_cast<std::chrono::milliseconds>(iter_end - iter_begin).count() };

    message("Elapsed time: {}ms", iter_elapsed_time);

    return 0;
}
