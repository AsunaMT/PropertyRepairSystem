#include <data/repair_sys/task.h>

std::vector<Task> Task::all_tasks_ = {};

Task::Task(const Repair* repair, const Yardman* yardman, const Worker* worker,
           std::time_t time, FaultType faultType)
    : repair_(repair),
      yardman_(yardman),
      worker_(worker),
      time_(time),
      state_(kUnYard),
      fault_type_(faultType) {}

void Task::set_state(TaskState state) { this->state_ = state; }

auto Task::get_state() const -> TaskState { return this->state_; }

auto Task::get_worker() const -> const Worker* { return this->worker_; }

auto Task::get_yardman() const -> const Yardman* { return this->yardman_; }

auto Task::get_repair() const -> const Repair* { return this->repair_; }

auto Task::AddTask(const Repair* repair, const Yardman* yardman,
                   const Worker* worker, std::time_t time, FaultType faultType)
    -> Task& {
  // std::shared_ptr<Task> task =
  //     std::make_shared<Task>(new Task(repair, yardman, worker, time));
  Task::all_tasks_.emplace_back(Task(repair, yardman, worker, time, faultType));
  return Task::all_tasks_.back();
}

auto Task::FindTaskByState(TaskState state) -> std::vector<Task*> {
  std::vector<Task*> res;
  for (auto& task : Task::all_tasks_) {
    if (task.state_ == state) res.emplace_back(&task);
  }
  return res;
}

auto Task::FindTaskByWorkerAndState(const Worker* worker, TaskState state)
    -> std::vector<Task*> {
  std::vector<Task*> res;
  for (auto& task : Task::all_tasks_) {
    if (task.worker_ == worker && task.state_ == state) res.emplace_back(&task);
  }
  return res;
}

auto Task::FindTaskByRepair(const Repair* repair) -> std::vector<Task*> {
  std::vector<Task*> res;
  for (auto& task : Task::all_tasks_) {
    if (task.repair_ == repair) res.emplace_back(&task);
  }
  return res;
}