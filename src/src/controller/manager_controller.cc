#include <controller/manager_controller.h>

#include <algorithm>

auto ManagerController::GetAllWorker(std::vector<Task*> tasks)
    -> std::vector<const Worker*> {
  std::vector<const Worker*> res;
  for (auto task : tasks) {
    auto worker = task->get_worker();
    if (std::find(res.begin(), res.end(), worker) == res.end()) {
      res.emplace_back(worker);
    }
  }
  return res;
}

auto ManagerController::GetAllYardman(std::vector<Task*> tasks,
                                      const Repair* repair)
    -> std::vector<const Yardman*> {
  std::vector<const Yardman*> res;
  for (auto task : tasks) {
    auto yardman = task->get_yardman();
    if (std::find(res.begin(), res.end(), yardman) == res.end()) {
      res.emplace_back(yardman);
    }
  }
  auto yardman = repair->get_yardman();
  if (std::find(res.begin(), res.end(), yardman) == res.end()) {
    res.emplace_back(yardman);
  }
  return res;
}

void ManagerController::set_manager(const Manager* manager) {
  this->manager_ = manager;
}

auto ManagerController::get_manager() const -> const Manager* {
  return this->manager_;
}

auto ManagerController::GetComplaintByState(ComplaintState state)
    -> std::vector<Complaint*> {
  return Complaint::FindByState(state);
}

void ManagerController::HandleComplaint(Complaint* complaint) {
  complaint->set_manager(this->manager_);
  complaint->set_state(kTreating);
  auto repair = complaint->get_repair();
  auto tasks = Task::FindTaskByRepair(repair);
  auto all_yardmen = GetAllYardman(tasks, repair);
  auto all_workers = GetAllWorker(tasks);
  for (auto yardman : all_yardmen) {
    YardmanComplaintExpl::AddComplaintExpl(complaint, this->manager_, yardman);
  }
  for (auto worker : all_workers) {
    WorkerComplaintExpl::AddComplaintExpl(complaint, this->manager_, worker);
  }
}

auto ManagerController::ComplaintOk(const Complaint* complaint) -> bool {
  return WorkerComplaintExpl::AllFilled(complaint) &&
         YardmanComplaintExpl::AllFilled(complaint);
}

void ManagerController::CloseComplaint(Complaint* complaint,
                                       const std::string& result) {
  complaint->set_result_record(result);
  complaint->set_state(kClose);
}