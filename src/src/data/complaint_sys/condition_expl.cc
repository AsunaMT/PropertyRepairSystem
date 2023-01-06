#include <data/complaint_sys/condition_expl.h>

std::vector<YardmanComplaintExpl>
    YardmanComplaintExpl::all_yardman_complaint_expl_ = {};

std::vector<WorkerComplaintExpl>
    WorkerComplaintExpl::all_worker_complaint_expl_ = {};

void ComplaintExpl::set_state(ComplaintExplState state) {
  this->state_ = state;
}

void ComplaintExpl::set_explanation(const std::string& explanation) {
  this->explanation_ = explanation;
}

auto ComplaintExpl::get_complaint() -> const Complaint* {
  return this->complaint_;
}

void YardmanComplaintExpl::set_state(ComplaintExplState state) {
  this->state_ = state;
}

void YardmanComplaintExpl::set_explanation(const std::string& explanation) {
  this->explanation_ = explanation;
}

auto YardmanComplaintExpl::get_complaint() -> const Complaint* {
  return this->complaint_;
}
void WorkerComplaintExpl::set_state(ComplaintExplState state) {
  this->state_ = state;
}

void WorkerComplaintExpl::set_explanation(const std::string& explanation) {
  this->explanation_ = explanation;
}

auto WorkerComplaintExpl::get_complaint() -> const Complaint* {
  return this->complaint_;
}

YardmanComplaintExpl::YardmanComplaintExpl(const Complaint* complaint,
                                           const Manager* manager,
                                           const Yardman* yardman)
    : complaint_(complaint),
      manager_(manager),
      yardman_(yardman),
      state_(kNotFilled) {
  static uint64_t accretion = 0;
  this->id_ = accretion++;
}

auto YardmanComplaintExpl::AddComplaintExpl(const Complaint* complaint,
                                            const Manager* manager,
                                            const Yardman* yardman)
    -> YardmanComplaintExpl& {
  YardmanComplaintExpl::all_yardman_complaint_expl_.emplace_back(
      YardmanComplaintExpl(complaint, manager, yardman));
  return YardmanComplaintExpl::all_yardman_complaint_expl_.back();
}

auto YardmanComplaintExpl::FindByYardmanAndState(const Yardman* yardman,
                                                 ComplaintExplState state)
    -> std::vector<YardmanComplaintExpl*> {
  std::vector<YardmanComplaintExpl*> res;
  for (auto& expl : YardmanComplaintExpl::all_yardman_complaint_expl_) {
    if (expl.yardman_ == yardman && expl.state_ == state) {
      res.emplace_back(&expl);
    }
  }
  return res;
}

auto YardmanComplaintExpl::FindByComplaint(const Complaint* complaint)
    -> std::vector<YardmanComplaintExpl*> {
  std::vector<YardmanComplaintExpl*> res;
  for (auto& expl : YardmanComplaintExpl::all_yardman_complaint_expl_) {
    if (expl.complaint_ == complaint) {
      res.emplace_back(&expl);
    }
  }
  return res;
}

auto YardmanComplaintExpl::AllFilled(const Complaint* complaint) -> bool {
  for (auto& expl : YardmanComplaintExpl::all_yardman_complaint_expl_) {
    if (expl.complaint_ == complaint && expl.state_ != kFilled) {
      return false;
    }
  }
  return true;
}

WorkerComplaintExpl::WorkerComplaintExpl(const Complaint* complaint,
                                         const Manager* manager,
                                         const Worker* worker)
    : complaint_(complaint),
      manager_(manager),
      worker_(worker),
      state_(kNotFilled) {
  static uint64_t accretion = 0;
  this->id_ = accretion++;
}
auto WorkerComplaintExpl::AddComplaintExpl(const Complaint* complaint,
                                           const Manager* manager,
                                           const Worker* worker)
    -> WorkerComplaintExpl& {
  WorkerComplaintExpl::all_worker_complaint_expl_.emplace_back(
      WorkerComplaintExpl(complaint, manager, worker));
  return WorkerComplaintExpl::all_worker_complaint_expl_.back();
}

auto WorkerComplaintExpl::FindByWorkerAndState(const Worker* worker,
                                               ComplaintExplState state)
    -> std::vector<WorkerComplaintExpl*> {
  std::vector<WorkerComplaintExpl*> res;
  for (auto& expl : WorkerComplaintExpl::all_worker_complaint_expl_) {
    if (expl.worker_ == worker && expl.state_ == state) {
      res.emplace_back(&expl);
    }
  }
  return res;
}

auto WorkerComplaintExpl::FindByComplaint(const Complaint* complaint)
    -> std::vector<WorkerComplaintExpl*> {
  std::vector<WorkerComplaintExpl*> res;
  for (auto& expl : WorkerComplaintExpl::all_worker_complaint_expl_) {
    if (expl.complaint_ == complaint) {
      res.emplace_back(&expl);
    }
  }
  return res;
}

auto WorkerComplaintExpl::AllFilled(const Complaint* complaint) -> bool {
  for (auto& expl : WorkerComplaintExpl::all_worker_complaint_expl_) {
    if (expl.complaint_ == complaint && expl.state_ != kFilled) {
      return false;
    }
  }
  return true;
}