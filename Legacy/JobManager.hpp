/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2025-06-13
    Last Modified: 2025-06-13

    Description: JobManager.hpp
    Resposible for queuing jobs, fetching results and polling jobs
*/

#pragma once
#include "Common.hpp"
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include <optional>
#include <utility>

using std::vector;
using std::mutex;
using std::shared_ptr;
using std::atomic;
using std::thread;
using std::function;
using std::optional;
using std::lock_guard;
using std::unique_lock;
using std::nullopt;
using std::unordered_map;
using std::pair;

enum class JobStatus {
    NotFound,
    Running,
    Completed
};

struct Job {
    vector<BoardResult> results;
    JobStatus status = JobStatus::Running;
    mutex mtx;
    std::chrono::_V2::steady_clock::time_point completed_at;
    float runtime;
};

class JobManager {
private:
    atomic<int> next_id;
    unordered_map<int, shared_ptr<Job>> results;
    mutex result_mutex;
    thread cleanup_thread;
    int job_expiry_duration;
    int cleanup_timer;
    bool stop_cleanup;

    void cleanup_expired_jobs();

public:
    // values are in seconds
    JobManager(int job_expiry_duration, int cleanup_timer);
    ~JobManager();

    int submit(function<vector<BoardResult>()> func);
    JobStatus get_status(int job_id);
    optional<pair<vector<BoardResult>, float>> get_result(int job_id);
};
