#ifndef INCLUDE_DATA_COMMENT_SYS_COMMENT_H_
#define INCLUDE_DATA_COMMENT_SYS_COMMENT_H_

#include <common/comment_cmn.h>
#include <data/repair_sys/repair.h>
#include <data/role/owner.h>

#include <ctime>
#include <string>
#include <vector>

class Comment {
 private:
  static std::vector<Comment> all_comment_;
  const Owner* owner_;
  const Repair* repair_;
  Timeliness timeliness_;
  ServiceAttitude service_attitude_;
  SatisfactionDegree satisfaction_degree_;

 public:
  Comment(const Owner* owner, const Repair* repair, Timeliness timeliness,
          ServiceAttitude service_attitude,
          SatisfactionDegree satisfaction_degree);
  auto get_timeliness() const -> Timeliness;
  static auto AddComment(const Owner* owner, const Repair* repair,
                         Timeliness timeliness,
                         ServiceAttitude service_attitude,
                         SatisfactionDegree satisfaction_degree) -> Comment&;
  static auto FindAll() -> std::vector<const Comment*>;
};

#endif