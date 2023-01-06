#include <data/repair_sys/yard_request.h>

std::vector<YardRequest> YardRequest::all_request_ = {};

YardRequest::YardRequest(const Worker* worker, const Repair* repair,
                         FaultType type)
    : worker_(worker), repair_(repair), fault_type_(type), state_(kPulled) {}

auto YardRequest::get_fault_type() const -> FaultType {
  return this->fault_type_;
}

auto YardRequest::get_repair() const -> const Repair* { return this->repair_; }

void YardRequest::set_state(RequestState state) { this->state_ = state; }

auto YardRequest::AddYardRequest(const Worker* worker, const Repair* repair,
                                 FaultType type) -> YardRequest& {
  YardRequest::all_request_.emplace_back(YardRequest(worker, repair, type));
  return YardRequest::all_request_.front();
}

auto YardRequest::FindAll() -> std::vector<YardRequest*> {
  std::vector<YardRequest*> res;
  for (auto& request : YardRequest::all_request_) {
    res.emplace_back(&request);
  }
  return res;
}