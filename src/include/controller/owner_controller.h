#ifndef INCLUDE_CONTROLLER_OWNER_CONTROLLER_H_
#define INCLUDE_CONTROLLER_OWNER_CONTROLLER_H_

#include <common/comment_cmn.h>
#include <data/comment_sys/comment.h>
#include <data/complaint_sys/complaint.h>
#include <data/repair_sys/repair.h>
#include <data/role/owner.h>

#include <vector>

class OwnerController {
 private:
  const Owner* owner_;

 public:
  void set_owner(const Owner* owner);
  auto get_owner() const -> const Owner*;
  auto GetCanCommentRepair() -> std::vector<Repair*>;
  auto GetFinishedRepair() -> std::vector<Repair*>;
  auto GetAllRepair() -> std::vector<Repair*>;
  auto DoComment(const Repair* repair, Timeliness timeliness,
                 ServiceAttitude serviceAttitude,
                 SatisfactionDegree satisfactionDegree) -> const Comment&;
  auto PullComplaint(const Repair* repair, const std::string& details)
      -> Complaint&;
};

#endif