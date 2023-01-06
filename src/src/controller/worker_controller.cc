#include <controller/worker_controller.h>

void WorkerController::set_worker(Worker* worker) { this->worker_ = worker; }

auto WorkerController::get_worker() -> Worker* { return this->worker_; }

auto WorkerController::GetTaskByState(TaskState state) -> std::vector<Task*> {
  return Task::FindTaskByWorkerAndState(this->worker_, state);
}

auto WorkerController::GetFinishedRecord() -> std::vector<MaintRecord*> {
  return MaintRecord::FindFinishedRecordByWorker(this->worker_);
}

auto WorkerController::StartMaintenance(const Task* task, std::time_t time)
    -> MaintRecord& {
  this->worker_->set_state(kWorking);
  auto this_task = const_cast<Task*>(task);
  this_task->set_state(kInWork);
  return MaintRecord::AddMaintRecord(this->worker_, task, time);
}

auto WorkerController::ContinueMaintenance(const Task* task, std::time_t time)
    -> MaintRecord& {
  this->worker_->set_state(kWorking);
  auto this_task = const_cast<Task*>(task);
  this_task->set_state(kInWork);
  return MaintRecord::AddMaintRecord(this->worker_, task, time);
}

void WorkerController::PartialFinishMaintenance(MaintRecord& record,
                                                std::string process,
                                                std::time_t time) {
  this->worker_->set_state(kFree);
  record.FinishRecord(process, kToContinue, time);
  auto task = const_cast<Task*>(record.get_task());
  task->set_state(kYardNotInWord);
}

void WorkerController::AllFinishMaintenance(MaintRecord& record,
                                            std::string process,
                                            std::time_t time) {
  this->worker_->set_state(kFree);
  record.FinishRecord(process, kAllOver, time);
  auto task = const_cast<Task*>(record.get_task());
  task->set_state(kTaskFinish);
}

void WorkerController::TurnOverMaintenance(MaintRecord& record,
                                           std::string process,
                                           std::time_t time) {
  this->worker_->set_state(kFree);
  record.FinishRecord(process, kToTurnOver, time);
  auto task = const_cast<Task*>(record.get_task());
  task->set_state(kTurnOver);
}

auto WorkerController::PullYardRequest(const Repair* repair, FaultType type)
    -> YardRequest& {
  return YardRequest::AddYardRequest(this->worker_, repair, type);
}

auto WorkerController::GetComplaintExplByState(ComplaintExplState state)
    -> std::vector<WorkerComplaintExpl*> {
  return WorkerComplaintExpl::FindByWorkerAndState(this->worker_, state);
}

void WorkerController::FillComplaintExpl(WorkerComplaintExpl* expl,
                                         const std::string& explanation) {
  expl->set_explanation(explanation);
  expl->set_state(kFilled);
}

auto WorkerController::CalculateLaborTime(std::time_t start, std::time_t end)
    -> std::time_t {
  std::time_t res = 0;
  auto records = this->GetFinishedRecord();
  for (auto record : records) {
    if (record->get_start_time() >= start && record->get_end_time() <= end) {
      res += record->GetLaborTime();
    }
  }
  return res;
}