#include "log.hpp"

#include <format>
#include <mutex>
#include <iostream>
#include <thread>

namespace loglib
{
    static std::mutex log_mutex;

    void log_impl(const char* file, int line, std::string_view fmt, std::format_args args)
    {
        // Format outside lock if you want higher performance
        std::string msg = std::vformat(fmt, args);

        std::lock_guard<std::mutex> lock(log_mutex);

        std::cout
            << "[thread "
            << std::this_thread::get_id()
            << "] "
            << file << ':' << line << " "
            << msg
            << '\n';
    }
}
