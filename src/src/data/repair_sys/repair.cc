#include <data/repair_sys/repair.h>

std::vector<Repair> Repair::all_repair_ = {};

Repair::Repair(const Owner* owner, const Yardman* yardman,
               const std::time_t time, const std::string& fault,
               RepairMethod method, RepairState state)
    : owner_(owner),
      yardman_(yardman),
      time_(time),
      fault_(fault),
      method_(method),
      state_(state) {
  static uint64_t accretion = 0;
  this->id_ = accretion++;
}

void Repair::set_state(RepairState state) { this->state_ = state; }

auto Repair::get_state() -> RepairState { return this->state_; }

auto Repair::get_owner() const -> const Owner* { return this->owner_; }

auto Repair::get_yardman() const -> const Yardman* { return this->yardman_; }

void Repair::AddRepair(const Owner* owner, const Yardman* yardman,
                       const std::time_t time, const std::string& fault,
                       RepairMethod method) {
  Repair::all_repair_.emplace_back(
      Repair(owner, yardman, time, fault, method, kToYard));
}

auto Repair::FindRepairByState(RepairState state) -> std::vector<Repair*> {
  std::vector<Repair*> res;
  for (auto& repair : Repair::all_repair_) {
    if (repair.state_ == state) {
      res.emplace_back(&repair);
    }
  }
  return res;
}

auto Repair::FindRepairByOwnerAndState(const Owner* owner, RepairState state)
    -> std::vector<Repair*> {
  std::vector<Repair*> res;
  for (auto& repair : Repair::all_repair_) {
    if (repair.owner_ == owner && repair.state_ == state) {
      res.emplace_back(&repair);
    }
  }
  return res;
}