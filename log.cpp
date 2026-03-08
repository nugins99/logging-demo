#include "log.hpp"

#include <format>
#include <mutex>
#include <iostream>
#include <thread>
#include <string_view>
#include <filesystem>

namespace loglib
{
    static std::mutex log_mutex;

    void log_impl(std::source_location loc, std::string_view fmt, std::format_args args)
    {
        // Format outside lock if you want higher performance
        std::string msg = std::vformat(fmt, args);

        std::lock_guard<std::mutex> lock(log_mutex);

        std::filesystem::path p(loc.file_name());
        std::string file = p.filename().string();

        std::cout
            << "[thread "
            << std::this_thread::get_id()
            << "] "
            << file << ':' << loc.line() << " "
            << msg
            << '\n';
    }
}
