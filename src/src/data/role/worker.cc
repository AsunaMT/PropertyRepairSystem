#include <data/role/worker.h>

#include <algorithm>

std::vector<Worker> Worker::all_workers_ = {};

Worker::Worker(std::string name, std::vector<FaultType> abilities)
    : name_(name), abilities_(abilities), state_(kFree) {}

auto Worker::get_name() const -> std::string { return this->name_; }

void Worker::set_state(WorkerState state) { this->state_ = state; }

auto Worker::get_state() const -> WorkerState { return this->state_; }

void Worker::AddWorker(const std::vector<Worker>& workers) {
  for (auto& worker : workers) {
    Worker::all_workers_.emplace_back(worker);
  }
}

void Worker::AddWorker(const Worker& worker) {
  Worker::all_workers_.emplace_back(worker);
}

void Worker::ClearAllWorkers() { Worker::all_workers_.clear(); }

auto Worker::FindWorkerByFaultType(FaultType type) -> std::vector<Worker*> {
  std::vector<Worker*> res;
  for (auto& worker : Worker::all_workers_) {
    if (std::find(worker.abilities_.begin(), worker.abilities_.end(), type) !=
        worker.abilities_.end()) {
      res.emplace_back(&worker);
    }
  }
  return res;
}

auto Worker::FindWorkerByState(WorkerState state) -> std::vector<Worker*> {
  std::vector<Worker*> res;
  for (auto& worker : Worker::all_workers_) {
    if (worker.state_ == state) {
      res.emplace_back(&worker);
    }
  }
  return res;
}