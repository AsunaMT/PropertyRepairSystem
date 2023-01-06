#ifndef INCLUDE_COMMON_REPAIR_CMN_H_
#define INCLUDE_COMMON_REPAIR_CMN_H_

enum FaultType { kPowerCut, kElevator, kSewer };

enum RepairMethod { kPhone, KWechat };

enum RepairState {
  kToYard,       // 待调度
  KInDeal,       // 在处理
  kToComment,    // 待评论
  kRepairFinish  // 已完成
};

enum TaskState {
  kUnYard,         // 未调度/分配
  kYardNotInWord,  // 已调度但未在进行
  kInWork,         // 在进行
  kTaskFinish,     // 已完成
  kTurnOver        // 已转交
};

enum RecordType {
  kUnFinished,  // 本次工作未完成
  kToContinue,  // 结束部分任务待继续
  kAllOver,     // 完全结束任务
  kToTurnOver   // 结束待转交
};

enum RequestState {
  kPulled,  // 已提交
  kPassed   // 已通过
};

#endif