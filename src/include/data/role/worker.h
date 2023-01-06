#ifndef INCLUDE_ROLE_WORKER_H_
#define INCLUDE_ROLE_WORKER_H_

#include <common/repair_cmn.h>
#include <common/role_cmn.h>

#include <memory>
#include <string>
#include <vector>

class Worker {
 private:
  static std::vector<Worker> all_workers_;
  std::string name_;
  std::vector<FaultType> abilities_;
  WorkerState state_;

 public:
  Worker(std::string name, std::vector<FaultType> abilities);
  auto get_name() const -> std::string;
  auto get_state() const -> WorkerState;
  void set_state(WorkerState state);
  static void AddWorker(const std::vector<Worker>& workers);
  static void AddWorker(const Worker& worker);
  static void ClearAllWorkers();
  static auto FindWorkerByFaultType(FaultType type) -> std::vector<Worker*>;
  static auto FindWorkerByState(WorkerState state) -> std::vector<Worker*>;
};

#endif