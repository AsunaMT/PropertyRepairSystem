#ifndef INCLUDE_DATA_COMPLAINT_SYS_COMPLAINT_H_
#define INCLUDE_DATA_COMPLAINT_SYS_COMPLAINT_H_

#include <common/complaint_cmn.h>
#include <data/repair_sys/repair.h>
#include <data/role/manager.h>
#include <data/role/owner.h>

#include <ctime>
#include <string>
#include <vector>

class Complaint {
 private:
  static std::vector<Complaint> all_complaints_;
  const Owner* owner_;
  const Repair* repair_;
  const Manager* manager_;
  std::string details_;
  uint64_t id_;
  ComplaintState state_;
  std::string result_record_;

 public:
  Complaint(const Owner* owner, const Repair* repair,
            const std::string& details_);
  void set_manager(const Manager* manager);
  void set_state(ComplaintState state);
  auto get_state() -> ComplaintState;
  auto get_repair() const -> const Repair*;
  auto get_details() const -> std::string;
  void set_result_record(const std::string& result_record);
  auto get_result_record() -> std::string;
  static auto AddComplaint(const Owner* owner, const Repair* repair,
                           const std::string& details) -> Complaint&;
  static auto FindByState(ComplaintState state) -> std::vector<Complaint*>;
};

#endif