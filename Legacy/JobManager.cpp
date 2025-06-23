#include "JobManager.hpp"

JobManager::JobManager(int job_expiry_duration, int cleanup_timer){
    this->job_expiry_duration = job_expiry_duration;
    this->cleanup_timer = cleanup_timer;
    next_id = 1;
    stop_cleanup = false;
    cleanup_thread = std::thread([this]() { this->cleanup_expired_jobs(); }); // initialise the housekeeping thread
}

JobManager::~JobManager() {
    stop_cleanup = true;
    if (cleanup_thread.joinable()){
        cleanup_thread.join();
    }
}

int JobManager::submit(function<vector<BoardResult>()> func) {
    // create a new job
    int job_id = next_id++;
    auto current = std::make_shared<Job>();

    // add the job to the map
    {
        lock_guard<mutex> lock(result_mutex);
        results[job_id] = current;
    }

    // queue the job
    thread([func = move(func), current]() {

        auto start = std::chrono::steady_clock::now();
        vector<BoardResult> r = func(); // compute the job
        // expose the result
        {
            unique_lock<mutex> lock(current->mtx);
            current->results = r;
            current->status = JobStatus::Completed;
            current->completed_at = std::chrono::steady_clock::now();
            current->runtime = std::chrono::duration<float, std::milli>(current->completed_at - start).count() / 1000.0f;
        }
    }).detach();

    return job_id;
}

JobStatus JobManager::get_status(int job_id) {
    lock_guard<mutex> lock(result_mutex);
    auto it = results.find(job_id);
    if (it == results.end()) return JobStatus::NotFound;

    lock_guard<mutex> lock2(it->second->mtx);
    return it->second->status;
}

optional<pair<vector<BoardResult>, float>> JobManager::get_result(int job_id) {
    lock_guard<mutex> lock(result_mutex);
    auto it = results.find(job_id);
    if (it == results.end()) {
        return nullopt; // Job was cleaned up or not found
    }

    lock_guard<mutex> job_lock(it->second->mtx);
    return make_pair(it->second->results, it->second->runtime);
}

void JobManager::cleanup_expired_jobs() {
    while (!stop_cleanup) { // cleanup thread loop
        {
            lock_guard<mutex> lock(result_mutex);
            auto now = std::chrono::steady_clock::now();

            for (auto it = results.begin(); it != results.end(); ) {
                lock_guard<mutex> lock2(it->second->mtx);
                if (it->second->status == JobStatus::Completed) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second->completed_at);
                    if (elapsed.count() > job_expiry_duration) {
                        it = results.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(cleanup_timer));
    }
}