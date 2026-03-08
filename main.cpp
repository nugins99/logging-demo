#include "log.hpp"

#include <thread>
#include <vector>
#include <cstdlib> // for std::getenv

void worker(int id)
{
    for (int i = 0; i < 5; i++)
    {
        LOG("worker {} iteration {}", id, i);
    }
}

int main()
{
    LOG("program started");
    LOG("Now printing a char* format: {}", "Hello, world!");
    LOG("Print an environment variable: {}", std::getenv("PATH"));

    std::vector<std::thread> threads;

    for (int i = 0; i < 4; i++)
    {
        bool isOdd = (i % 2 == 1);
        LOG("creating worker thread {} (is odd: {})", i, isOdd);
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    LOG("program finished");

    return 0;
}
