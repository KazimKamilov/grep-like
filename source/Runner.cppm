export module runner;

import <string>;
import <thread>;

import msg;
import searcher;


static constexpr auto help_desc
{
    "Usage: \"grep-like [substring] [path]\" or\n\"grep-like -j[job_count] [substring] [path]\""
};

static constexpr auto noargs_desc
{
    "There are not enough arguments to run the program. "
};

static constexpr auto wrong_jobs_str_desc
{
    "Wrong jobs number. "
};

/**
    It's just check for number inside string
*/
static constexpr bool isNumber(const std::string& str)
{
    for (char ch : str)
    {
        if (!((ch >= '0') && (ch <= '9')))
            return false;
    }

    return true;
}

/**
    The function of parsing the number of jobs from the argument
*/
uint32_t getJobCountFromArg(const std::string& str)
{
    const std::string comm{ "-j" };

    const auto pos{ str.find(comm) };

    if (pos != std::string::npos)
    {
        uint32_t jobs{ std::thread::hardware_concurrency() };

        if (const auto& str_num{ str.substr(comm.length()) }; isNumber(str_num))
            jobs = std::stoul(str_num);
        else
            message("{} The maximum number of threads will be set.", wrong_jobs_str_desc);

        return jobs;
    }
    else
    {
        message(wrong_jobs_str_desc);

        return 0u;
    }
}

/**
    The main search function. Counts the number of files, then
    split them between the available (or specified) threads.
*/
export void run_search(const int argc, const char** argv)
{
    switch(argc)
    {
        // No arguments
        case 1:
        {
            message("{}\n{}", noargs_desc, help_desc);
        }
        break;

        // Not enough arguments, or shows help
        case 2:
        {
            const std::string arg(argv[1]);

            if ((arg == "-h") || (arg == "--help"))
            {
                message(help_desc);
            }
            else
            {
                message("{}\n{}", noargs_desc, help_desc);
            }
        }
        break;

        // Starts the search with the maximum number of physic threads(std::thread::hardware_concurrency())
        case 3:
        {
            std::string substr(argv[1]);
            std::string path(argv[2]);

            searchSubstr(std::thread::hardware_concurrency(), substr, path);
        }
        break;

        /**
            Starts the search with specified number of threads, usage:
            exe -jN [substr] [path]
        */
        case 4:
        {
            std::string jobs(argv[1]);
            std::string substr(argv[2]);
            std::string path(argv[3]);

            if (const uint32_t job_count{ getJobCountFromArg(jobs) }; (job_count > 0))
                searchSubstr(job_count, substr, path);
            else
                message(wrong_jobs_str_desc);
        }
        break;

        // Wrong arguments
        default:
        {
            message(wrong_jobs_str_desc);
        }
        break;
    }
}
