#include <data/complaint_sys/complaint.h>

std::vector<Complaint> Complaint::all_complaints_ = {};

Complaint::Complaint(const Owner* owner, const Repair* repair,
                     const std::string& details)
    : owner_(owner), repair_(repair), details_(details), state_(kUntreated) {
  static uint64_t accretion = 0;
  this->id_ = accretion++;
}

void Complaint::set_manager(const Manager* manager) {
  this->manager_ = manager;
}

void Complaint::set_state(ComplaintState state) { this->state_ = state; }

auto Complaint::get_state() -> ComplaintState { return this->state_; }

auto Complaint::get_repair() const -> const Repair* { return this->repair_; }

auto Complaint::get_details() const -> std::string { return this->details_; }

void Complaint::set_result_record(const std::string& result_record) {
  this->result_record_ = result_record;
}

auto Complaint::get_result_record() -> std::string {
  return this->result_record_;
}

auto Complaint::AddComplaint(const Owner* owner, const Repair* repair,
                             const std::string& details) -> Complaint& {
  Complaint::all_complaints_.emplace_back(Complaint(owner, repair, details));
  return Complaint::all_complaints_.back();
}

auto Complaint::FindByState(ComplaintState state) -> std::vector<Complaint*> {
  std::vector<Complaint*> res;
  for (auto& complaint : Complaint::all_complaints_) {
    if (complaint.state_ == state) {
      res.emplace_back(&complaint);
    }
  }
  return res;
}