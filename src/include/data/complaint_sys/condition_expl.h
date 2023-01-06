#ifndef INCLUDE_DATA_COMPLAINT_SYS_COMPLAINT_EXPL_H_
#define INCLUDE_DATA_COMPLAINT_SYS_COMPLAINT_EXPL_H_

#include <common/complaint_cmn.h>
#include <data/complaint_sys/complaint.h>
#include <data/repair_sys/repair.h>
#include <data/role/manager.h>
#include <data/role/worker.h>
#include <data/role/yardman.h>

#include <ctime>
#include <string>
#include <vector>

class ComplaintExpl {
 protected:
  const Complaint* complaint_;
  const Manager* manager_;
  uint64_t id_;
  ComplaintExplState state_;
  std::string explanation_;

 public:
  void set_state(ComplaintExplState state);
  void set_explanation(const std::string& explanation);
  auto get_complaint() -> const Complaint*;
};

class YardmanComplaintExpl : public ComplaintExpl {
 private:
  static std::vector<YardmanComplaintExpl> all_yardman_complaint_expl_;
  const Complaint* complaint_;
  const Manager* manager_;
  uint64_t id_;
  ComplaintExplState state_;
  std::string explanation_;
  const Yardman* yardman_;

 public:
  YardmanComplaintExpl(const Complaint* complaint, const Manager* manager,
                       const Yardman* yardman);
  void set_state(ComplaintExplState state);
  void set_explanation(const std::string& explanation);
  auto get_complaint() -> const Complaint*;
  static auto AddComplaintExpl(const Complaint* complaint,
                               const Manager* manager, const Yardman* yardman)
      -> YardmanComplaintExpl&;
  static auto FindByYardmanAndState(const Yardman* yardman,
                                    ComplaintExplState state)
      -> std::vector<YardmanComplaintExpl*>;
  static auto FindByComplaint(const Complaint* complaint)
      -> std::vector<YardmanComplaintExpl*>;
  static auto AllFilled(const Complaint* complaint) -> bool;
};

class WorkerComplaintExpl : public ComplaintExpl {
 private:
  static std::vector<WorkerComplaintExpl> all_worker_complaint_expl_;
  const Complaint* complaint_;
  const Manager* manager_;
  uint64_t id_;
  ComplaintExplState state_;
  std::string explanation_;
  const Worker* worker_;

 public:
  WorkerComplaintExpl(const Complaint* complaint, const Manager* manager,
                      const Worker* worker);
  void set_state(ComplaintExplState state);
  void set_explanation(const std::string& explanation);
  auto get_complaint() -> const Complaint*;
  static auto AddComplaintExpl(const Complaint* complaint,
                               const Manager* manager, const Worker* worker)
      -> WorkerComplaintExpl&;

  static auto FindByWorkerAndState(const Worker* worker,
                                   ComplaintExplState state)
      -> std::vector<WorkerComplaintExpl*>;
  static auto FindByComplaint(const Complaint* complaint)
      -> std::vector<WorkerComplaintExpl*>;
  static auto AllFilled(const Complaint* complaint) -> bool;
};

#endif