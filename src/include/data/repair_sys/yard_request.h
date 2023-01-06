#ifndef INCLUDE_DATA_REPAIR_SYS_YARD_REQUEST_H_
#define INCLUDE_DATA_REPAIR_SYS_YARD_REQUEST_H_

#include <common/repair_cmn.h>
#include <data/repair_sys/repair.h>
#include <data/role/worker.h>

#include <ctime>
#include <memory>
#include <string>
#include <vector>

class YardRequest {
 private:
  static std::vector<YardRequest> all_request_;
  const Worker* worker_;
  const Repair* repair_;
  FaultType fault_type_;
  RequestState state_;

 public:
  YardRequest(const Worker* worker, const Repair* repair, FaultType type);
  auto get_fault_type() const -> FaultType;
  auto get_repair() const -> const Repair*;
  void set_state(RequestState state);
  static auto AddYardRequest(const Worker* worker, const Repair* repair,
                             FaultType type) -> YardRequest&;
  static auto FindAll() -> std::vector<YardRequest*>;
};

#endif