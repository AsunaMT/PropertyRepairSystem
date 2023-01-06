#ifndef INCLUDE_DATA_REPAIR_SYS_REPAIR_H_
#define INCLUDE_DATA_REPAIR_SYS_REPAIR_H_

#include <common/repair_cmn.h>
#include <data/role/owner.h>
#include <data/role/yardman.h>

#include <ctime>
#include <memory>
#include <string>
#include <vector>

class Repair {
 private:
  static std::vector<Repair> all_repair_;
  const Owner* owner_;
  const Yardman* yardman_;
  std::time_t time_;
  std::string fault_;
  RepairMethod method_;
  RepairState state_;
  uint64_t id_;

 public:
  Repair(const Owner* owner, const Yardman* yardman, const std::time_t time,
         const std::string& fault, RepairMethod method, RepairState state);
  void set_state(RepairState state);
  auto get_state() -> RepairState;
  auto get_owner() const -> const Owner*;
  auto get_yardman() const -> const Yardman*;
  static void AddRepair(const Owner* owner, const Yardman* yardman,
                        const std::time_t time, const std::string& fault,
                        RepairMethod method);
  static auto FindRepairByState(RepairState state) -> std::vector<Repair*>;
  static auto FindRepairByOwnerAndState(const Owner* owner, RepairState state)
      -> std::vector<Repair*>;
};

#endif