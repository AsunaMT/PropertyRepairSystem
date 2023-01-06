#include <controller/yardman_controller.h>

void YardmanController::set_yardman(const Yardman* yardman) {
  this->yardman_ = yardman;
}

auto YardmanController::get_yardman() -> const Yardman* {
  return this->yardman_;
}

void YardmanController::LogRepair(const Owner* owner, const std::time_t time,
                                  const std::string& fault,
                                  const RepairMethod& method) {
  Repair::AddRepair(owner, this->yardman_, time, fault, method);
}

void YardmanController::Yard(const Worker* worker, Repair* repair,
                             FaultType type) {
  auto& task =
      Task::AddTask(repair, this->yardman_, worker, std::time(nullptr), type);
  task.set_state(kYardNotInWord);
  repair->set_state(KInDeal);
}

auto YardmanController::GetSuitableWorker(FaultType type)
    -> std::vector<Worker*> {
  return Worker::FindWorkerByFaultType(type);
}

auto YardmanController::GetRepairByState(RepairState state)
    -> std::vector<Repair*> {
  return Repair::FindRepairByState(state);
}

auto YardmanController::GetTaskByState(TaskState state) -> std::vector<Task*> {
  return Task::FindTaskByState(state);
}

auto YardmanController::GetRecordByType(RecordType type)
    -> std::vector<MaintRecord*> {
  return MaintRecord::FindRecordByType(type);
}

void YardmanController::FinishRepair(const Repair* repair) {
  auto rep = const_cast<Repair*>(repair);
  rep->set_state(kToComment);
}

auto YardmanController::GetAllComment() -> std::vector<const Comment*> {
  return Comment::FindAll();
}

auto YardmanController::GetAllYardRequest() -> std::vector<YardRequest*> {
  return YardRequest::FindAll();
}

auto YardmanController::PassYardRequest(YardRequest* request)
    -> std::vector<Worker*> {
  request->set_state(kPassed);
  return GetSuitableWorker(request->get_fault_type());
}

auto YardmanController::GetComplaintExplByState(ComplaintExplState state)
    -> std::vector<YardmanComplaintExpl*> {
  return YardmanComplaintExpl::FindByYardmanAndState(this->yardman_, state);
}

void YardmanController::FillComplaintExpl(YardmanComplaintExpl* expl,
                                          const std::string& explanation) {
  expl->set_explanation(explanation);
  expl->set_state(kFilled);
}