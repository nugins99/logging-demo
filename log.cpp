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
    static std::string buffer;

    void log_impl(std::source_location loc, std::string_view fmt, std::format_args args)
    {

        std::lock_guard<std::mutex> lock(log_mutex);
        // Format outside lock if you want higher performance
        //std::string msg = std::vformat(fmt, args);
        std::vformat_to(std::back_inserter(buffer), fmt, args);

        std::filesystem::path p(loc.file_name());
        std::string file = p.filename().string();

        std::cout
            << "[thread "
            << std::this_thread::get_id()
            << "] "
            << file << ':' << loc.line() << " "
            << buffer 
            << '\n';
        buffer.clear();
    }
}
