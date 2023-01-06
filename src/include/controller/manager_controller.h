#ifndef INCLUDE_CONTROLLER_MANAGER_CONTROLLER_H_
#define INCLUDE_CONTROLLER_MANAGER_CONTROLLER_H_

#include <common/comment_cmn.h>
#include <data/comment_sys/comment.h>
#include <data/complaint_sys/complaint.h>
#include <data/complaint_sys/condition_expl.h>
#include <data/repair_sys/repair.h>
#include <data/repair_sys/task.h>
#include <data/role/owner.h>
#include <data/role/worker.h>
#include <data/role/yardman.h>

#include <vector>

class ManagerController {
 private:
  const Manager* manager_;

 public:
  void set_manager(const Manager* manager);
  auto get_manager() const -> const Manager*;
  auto GetComplaintByState(ComplaintState state) -> std::vector<Complaint*>;
  void HandleComplaint(Complaint* complaint);
  auto ComplaintOk(const Complaint* complaint) -> bool;
  void CloseComplaint(Complaint* complaint, const std::string& result);
  auto GetAllWorker(std::vector<Task*> tasks) -> std::vector<const Worker*>;
  auto GetAllYardman(std::vector<Task*> tasks, const Repair* repair)
      -> std::vector<const Yardman*>;
};

#endif