#ifndef INCLUDE_CONTROLLER_YARDMAN_CONTROLLER_H_
#define INCLUDE_CONTROLLER_YARDMAN_CONTROLLER_H_

#include <common/complaint_cmn.h>
#include <common/repair_cmn.h>
#include <data/comment_sys/comment.h>
#include <data/complaint_sys/condition_expl.h>
#include <data/repair_sys/maint_record.h>
#include <data/repair_sys/repair.h>
#include <data/repair_sys/task.h>
#include <data/repair_sys/yard_request.h>
#include <data/role/owner.h>
#include <data/role/worker.h>
#include <data/role/yardman.h>

#include <string>
#include <vector>

class YardmanController {
 private:
  const Yardman* yardman_;

 public:
  void set_yardman(const Yardman* yardman);
  auto get_yardman() -> const Yardman*;
  void LogRepair(const Owner* owner, const std::time_t time,
                 const std::string& fault, const RepairMethod& method);
  void Yard(const Worker* worker, Repair* repair, FaultType type);
  auto GetSuitableWorker(FaultType type) -> std::vector<Worker*>;
  auto GetRepairByState(RepairState state) -> std::vector<Repair*>;
  auto GetTaskByState(TaskState state) -> std::vector<Task*>;
  auto GetRecordByType(RecordType type) -> std::vector<MaintRecord*>;
  void FinishRepair(const Repair* repair);
  auto GetAllComment() -> std::vector<const Comment*>;
  auto GetAllYardRequest() -> std::vector<YardRequest*>;
  __attribute__((warn_unused_result)) auto PassYardRequest(YardRequest* request)
      -> std::vector<Worker*>;
  auto GetComplaintExplByState(ComplaintExplState state)
      -> std::vector<YardmanComplaintExpl*>;
  void FillComplaintExpl(YardmanComplaintExpl* expl,
                         const std::string& explanation);
};

#endif