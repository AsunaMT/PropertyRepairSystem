#include <controller/owner_controller.h>

void OwnerController::set_owner(const Owner* owner) { this->owner_ = owner; }
auto OwnerController::get_owner() const -> const Owner* { return this->owner_; }

auto OwnerController::GetCanCommentRepair() -> std::vector<Repair*> {
  return Repair::FindRepairByOwnerAndState(this->owner_, kToComment);
}

auto OwnerController::GetFinishedRepair() -> std::vector<Repair*> {
  return Repair::FindRepairByOwnerAndState(this->owner_, kRepairFinish);
}

auto OwnerController::GetAllRepair() -> std::vector<Repair*> {
  return std::vector<Repair*>{};
}

auto OwnerController::DoComment(const Repair* repair, Timeliness timeliness,
                                ServiceAttitude serviceAttitude,
                                SatisfactionDegree satisfactionDegree)
    -> const Comment& {
  auto rep = const_cast<Repair*>(repair);
  rep->set_state(kRepairFinish);
  return Comment::AddComment(this->owner_, repair, timeliness, serviceAttitude,
                             satisfactionDegree);
}

auto OwnerController::PullComplaint(const Repair* repair,
                                    const std::string& details) -> Complaint& {
  return Complaint::AddComplaint(this->owner_, repair, details);
}
