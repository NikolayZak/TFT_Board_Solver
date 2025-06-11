#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <atomic>

class JobQueue {
public:
    JobQueue(int thread_count = std::thread::hardware_concurrency());
    ~JobQueue();

    void enqueue(std::function<void()> job);

private:
    void worker_loop();

    std::queue<std::function<void()>> jobs;
    std::vector<std::thread> workers;
    std::mutex jobs_mutex;
    std::condition_variable job_available;
    std::atomic<bool> running;
};
