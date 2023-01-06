#ifndef INCLUDE_DATA_REPAIR_SYS_TASK_H_
#define INCLUDE_DATA_REPAIR_SYS_TASK_H_

#include <common/repair_cmn.h>
#include <data/repair_sys/repair.h>
#include <data/role/worker.h>
#include <data/role/yardman.h>

#include <ctime>
#include <memory>
#include <string>
#include <vector>

class Task {
 private:
  static std::vector<Task> all_tasks_;
  const Repair* repair_;
  const Yardman* yardman_;
  const Worker* worker_;
  std::time_t time_;
  TaskState state_;
  FaultType fault_type_;

 public:
  Task(const Repair* repair, const Yardman* yardman, const Worker* worker,
       std::time_t time, FaultType faultType);
  void set_state(TaskState state);
  auto get_state() const -> TaskState;
  auto get_worker() const -> const Worker*;
  auto get_yardman() const -> const Yardman*;
  auto get_repair() const -> const Repair*;

  static auto AddTask(const Repair* repair, const Yardman* yardman,
                      const Worker* worker, std::time_t time,
                      FaultType faultType) -> Task&;
  static auto FindTaskByState(TaskState state) -> std::vector<Task*>;
  static auto FindTaskByWorkerAndState(const Worker* worker, TaskState state)
      -> std::vector<Task*>;
  static auto FindTaskByRepair(const Repair* repair) -> std::vector<Task*>;
};

#endif