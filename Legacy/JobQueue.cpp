#include "JobQueue.hpp"

JobQueue::JobQueue(int thread_count) : running(true) {
    for (int i = 0; i < thread_count; ++i) {
        workers.emplace_back([this] { worker_loop(); });
    }
}

JobQueue::~JobQueue() {
    running = false;
    job_available.notify_all();
    for (auto& thread : workers) {
        if (thread.joinable())
            thread.join();
    }
}

void JobQueue::enqueue(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(jobs_mutex);
        jobs.push(std::move(job));
    }
    job_available.notify_one();
}

void JobQueue::worker_loop() {
    while (running) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            job_available.wait(lock, [this] {
                return !jobs.empty() || !running;
            });

            if (!running && jobs.empty())
                return;

            job = std::move(jobs.front());
            jobs.pop();
        }

        // Run the job outside the lock
        job();
    }
}
