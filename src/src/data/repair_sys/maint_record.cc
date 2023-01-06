#include <data/repair_sys/maint_record.h>

std::vector<MaintRecord> MaintRecord::all_record_ = {};

MaintRecord::MaintRecord(const Worker* worker, const Task* task,
                         std::time_t start_time)
    : worker_(worker),
      task_(task),
      start_time_(start_time),
      type_(kUnFinished) {}

auto MaintRecord::get_task() const -> const Task* { return this->task_; }

auto MaintRecord::get_start_time() const -> std::time_t {
  return this->start_time_;
}

auto MaintRecord::get_end_time() const -> std::time_t {
  return this->end_time_;
}

auto MaintRecord::GetLaborTime() -> std::time_t {
  return this->end_time_ - this->start_time_;
}

auto MaintRecord::AddMaintRecord(const Worker* worker, const Task* task,
                                 std::time_t start_time) -> MaintRecord& {
  MaintRecord::all_record_.emplace_back(MaintRecord(worker, task, start_time));
  return MaintRecord::all_record_.back();
}

void MaintRecord::FinishRecord(std::string process, RecordType type,
                               std::time_t end_time) {
  this->end_time_ = end_time;
  this->process_ = process;
  this->type_ = type;
}

auto MaintRecord::FindRecordByType(RecordType type)
    -> std::vector<MaintRecord*> {
  std::vector<MaintRecord*> res;
  for (auto& record : MaintRecord::all_record_) {
    if (record.type_ == type) {
      res.emplace_back(&record);
    }
  }
  return res;
}

auto MaintRecord::FindFinishedRecordByWorker(const Worker* worker)
    -> std::vector<MaintRecord*> {
  std::vector<MaintRecord*> res;
  for (auto& record : MaintRecord::all_record_) {
    if (record.worker_ == worker && record.type_ != kUnFinished) {
      res.emplace_back(&record);
    }
  }
  return res;
}