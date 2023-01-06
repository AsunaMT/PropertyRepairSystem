#ifndef INCLUDE_DATA_REPAIR_SYS_MAINT_RECORD_H_
#define INCLUDE_DATA_REPAIR_SYS_MAINT_RECORD_H_

#include <common/repair_cmn.h>
#include <data/repair_sys/repair.h>
#include <data/repair_sys/task.h>
#include <data/role/worker.h>

#include <ctime>
#include <string>
#include <vector>

class MaintRecord {
 private:
  static std::vector<MaintRecord> all_record_;
  const Worker* worker_;
  const Task* task_;
  std::time_t start_time_;
  std::time_t end_time_;
  std::string process_;
  RecordType type_;

 public:
  MaintRecord(const Worker* worker, const Task* task,
              std::time_t start_time = std::time(nullptr));
  auto get_task() const -> const Task*;
  auto get_start_time() const -> std::time_t;
  auto get_end_time() const -> std::time_t;
  auto GetLaborTime() -> std::time_t;
  static auto AddMaintRecord(const Worker* worker, const Task* task,
                             std::time_t start_time = std::time(nullptr))
      -> MaintRecord&;
  void FinishRecord(std::string process, RecordType type,
                    std::time_t end_time = std::time(nullptr));
  static auto FindRecordByType(RecordType type) -> std::vector<MaintRecord*>;
  static auto FindFinishedRecordByWorker(const Worker* Worker)
      -> std::vector<MaintRecord*>;
};

#endif