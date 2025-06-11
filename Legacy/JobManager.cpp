#include "JobManager.hpp"

JobManager::JobManager() : next_job_id(0) {}

int JobManager::create_job() {
    std::lock_guard<std::mutex> lock(status_mutex);
    int job_id = next_job_id++;
    job_status[job_id] = "created";
    return job_id;
}

void JobManager::set_status(int job_id, const std::string& status) {
    std::lock_guard<std::mutex> lock(status_mutex);
    job_status[job_id] = status;
}

std::string JobManager::get_status(int job_id) {
    std::lock_guard<std::mutex> lock(status_mutex);
    auto it = job_status.find(job_id);
    if (it != job_status.end()) {
        return it->second;
    }
    return "not_found";
}
