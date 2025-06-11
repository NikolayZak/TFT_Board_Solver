#pragma once
#include <unordered_map>
#include <mutex>
#include <string>

class JobManager {
public:
    JobManager();

    int create_job(); // creates and returns a new job ID
    void set_status(int job_id, const std::string& status);
    std::string get_status(int job_id);

private:
    std::unordered_map<int, std::string> job_status;
    std::mutex status_mutex;
    int next_job_id;
};
