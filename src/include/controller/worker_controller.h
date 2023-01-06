#ifndef INCLUDE_CONTROLLER_WORKER_CONTROLLER_H_
#define INCLUDE_CONTROLLER_WORKER_CONTROLLER_H_

#include <common/complaint_cmn.h>
#include <common/repair_cmn.h>
#include <data/complaint_sys/condition_expl.h>
#include <data/repair_sys/maint_record.h>
#include <data/repair_sys/repair.h>
#include <data/repair_sys/task.h>
#include <data/repair_sys/yard_request.h>
#include <data/role/owner.h>
#include <data/role/worker.h>
#include <data/role/yardman.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

class WorkerController {
 private:
  Worker* worker_;

 public:
  void set_worker(Worker* worker);
  auto get_worker() -> Worker*;
  auto GetTaskByState(TaskState state) -> std::vector<Task*>;
  auto GetFinishedRecord() -> std::vector<MaintRecord*>;
  auto StartMaintenance(const Task* task, std::time_t time = std::time(nullptr))
      -> MaintRecord&;
  auto ContinueMaintenance(const Task* task,
                           std::time_t time = std::time(nullptr))
      -> MaintRecord&;
  void PartialFinishMaintenance(MaintRecord& record, std::string process,
                                std::time_t time = std::time(nullptr));
  void AllFinishMaintenance(MaintRecord& record, std::string process,
                            std::time_t time = std::time(nullptr));
  void TurnOverMaintenance(MaintRecord& record, std::string process,
                           std::time_t time = std::time(nullptr));
  auto PullYardRequest(const Repair* repair, FaultType type) -> YardRequest&;
  auto GetComplaintExplByState(ComplaintExplState state)
      -> std::vector<WorkerComplaintExpl*>;
  void FillComplaintExpl(WorkerComplaintExpl* expl,
                         const std::string& explanation);

  auto CalculateLaborTime(std::time_t start, std::time_t end) -> std::time_t;
};

#endif