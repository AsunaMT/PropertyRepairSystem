#include <allsystem.h>
#include <controllers.h>
#include <limits.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Return;
using namespace testing;
// using namespace std;

// 正常流程
TEST(NormalProcess, REPAIR_TEST) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  EXPECT_EQ(suitable_workers.size(), 1);
  auto worker = suitable_workers.front();
  std::string expect_worker_name = "工人C";
  EXPECT_EQ(worker->get_name(), expect_worker_name);
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  EXPECT_EQ(repairs.size(), 1);
  yardCtrl.Yard(worker, repairs.front(), kPowerCut);
  // 工人维修并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(tasks.size(), 1);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx");
  // 调度员确认报修完成
  auto finished_task = yardCtrl.GetTaskByState(kTaskFinish);
  EXPECT_EQ(finished_task.size(), 1);
  yardCtrl.FinishRepair(finished_task.front()->get_repair());
  //  业主评价
  OwnerController ownerCtrl;
  ownerCtrl.set_owner(&owner);
  auto comment_repair = ownerCtrl.GetCanCommentRepair();
  EXPECT_EQ(comment_repair.size(), 1);
  ownerCtrl.DoComment(comment_repair.front(), kVeryTimely, kVeryKindly,
                      kQuiteSatisfied);
  // 调度员查看评价
  auto comments = yardCtrl.GetAllComment();
  EXPECT_EQ(comments.size(), 1);
  EXPECT_EQ(comments.front()->get_timeliness(), kVeryTimely);
  Worker::ClearAllWorkers();
}

// 扩展流程1--多次调度
TEST(ExtendProcess1, REPAIR_EXTEND_TEST1) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  EXPECT_EQ(suitable_workers.size(), 1);
  auto worker = suitable_workers.front();
  std::string expect_worker_name = "工人C";
  EXPECT_EQ(worker->get_name(), expect_worker_name);
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  EXPECT_EQ(repairs.size(), 1);
  yardCtrl.Yard(worker, repairs.front(), kPowerCut);
  // 工人维修并提出调度申请
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(tasks.size(), 1);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  workerCtrl.TurnOverMaintenance(record, "xx_process_xx");
  workerCtrl.PullYardRequest(record.get_task()->get_repair(), kElevator);
  EXPECT_EQ(record.get_task()->get_state(), kTurnOver);
  // 调度员重新调度
  auto requests = yardCtrl.GetAllYardRequest();
  EXPECT_EQ(requests.size(), 1);
  auto request = requests.front();
  auto new_workers = yardCtrl.PassYardRequest(request);
  EXPECT_EQ(new_workers.size(), 1);
  auto new_worker = new_workers.front();
  auto this_repair = const_cast<Repair*>(request->get_repair());
  yardCtrl.Yard(new_worker, this_repair, request->get_fault_type());
  // 新的匹配工人看到新调度给自己的任务
  workerCtrl.set_worker(new_worker);
  auto new_task = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(new_task.size(), 1);
  EXPECT_EQ(new_task.front()->get_repair(), this_repair);
  Worker::ClearAllWorkers();
}

// 扩展流程2--需要多次执行的活动
TEST(ExtendProcess2, REPAIR_EXTEND_TEST2) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  EXPECT_EQ(suitable_workers.size(), 1);
  auto worker = suitable_workers.front();
  std::string expect_worker_name = "工人C";
  EXPECT_EQ(worker->get_name(), expect_worker_name);
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  EXPECT_EQ(repairs.size(), 1);
  yardCtrl.Yard(worker, repairs.front(), kPowerCut);
  // 工人维修完成一部分并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(tasks.size(), 1);
  auto task = tasks.front();
  auto par_records = yardCtrl.GetRecordByType(kToContinue);
  EXPECT_EQ(par_records.size(), 0);
  auto& record1 = workerCtrl.StartMaintenance(task);
  workerCtrl.PartialFinishMaintenance(record1, "xx_process_xx_partail1");
  // 工人继续维修并记录
  auto& record2 = workerCtrl.StartMaintenance(task);
  workerCtrl.PartialFinishMaintenance(record2, "xx_process_xx_partail2");
  // 工人继续维修并记录
  auto& record3 = workerCtrl.StartMaintenance(task);
  workerCtrl.PartialFinishMaintenance(record3, "xx_process_xx_partail3");
  // 工人完全完成维修并记录
  auto& record4 = workerCtrl.StartMaintenance(task);
  workerCtrl.AllFinishMaintenance(record4, "xx_process_xx_all_finish");
  par_records = yardCtrl.GetRecordByType(kToContinue);
  EXPECT_EQ(par_records.size(), 3);
  Worker::ClearAllWorkers();
}

// 扩展流程3--故障类型与维修工人工种的匹配
TEST(ExtendProcess3, REPAIR_EXTEND_TEST3) {
  Yardman yardman("调度员A");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator}),
      worker3("工人E", std::vector<FaultType>{kSewer});
  Worker::AddWorker({worker1, worker2, worker3});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  auto power_worker = yardCtrl.GetSuitableWorker(kPowerCut);
  auto elevator_worker = yardCtrl.GetSuitableWorker(kElevator);
  EXPECT_EQ(power_worker.size(), 1);
  EXPECT_EQ(power_worker.front()->get_name(), "工人C");
  EXPECT_EQ(elevator_worker.size(), 1);
  EXPECT_EQ(elevator_worker.front()->get_name(), "工人D");
  Worker::ClearAllWorkers();
}

// 扩展流程4--投诉
TEST(ExtendProcess4, REPAIR_EXTEND_TEST4) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  EXPECT_EQ(suitable_workers.size(), 1);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  EXPECT_EQ(repairs.size(), 1);
  auto repair = repairs.front();
  yardCtrl.Yard(worker, repair, kPowerCut);
  // 工人维修并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(tasks.size(), 1);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx");
  // 调度员确认报修完成
  yardCtrl.FinishRepair(repair);
  // 业主投诉
  OwnerController ownerCtrl;
  ownerCtrl.set_owner(&owner);
  auto this_repairs = ownerCtrl.GetCanCommentRepair();
  EXPECT_EQ(this_repairs.size(), 1);
  auto this_repair = this_repairs.front();
  EXPECT_NE(this_repair, nullptr);
  ownerCtrl.PullComplaint(this_repair, "修的太差了");
  // 物业经理处理
  ManagerController managerCtrl;
  Manager manager("物业经理X");
  managerCtrl.set_manager(&manager);
  auto cmpls = managerCtrl.GetComplaintByState(kUntreated);
  EXPECT_EQ(cmpls.size(), 1);
  auto cmpl = cmpls.front();
  EXPECT_EQ(cmpl->get_details(), "修的太差了");
  managerCtrl.HandleComplaint(cmpl);
  // 调度员和工人查看需要填写的投诉情况说明并填写
  auto yardman_expls = yardCtrl.GetComplaintExplByState(kNotFilled);
  EXPECT_EQ(yardman_expls.size(), 1);
  EXPECT_EQ(yardman_expls.front()->get_complaint(), cmpl);
  yardCtrl.FillComplaintExpl(yardman_expls.front(), "非常抱歉,马上补救");
  auto worker_expls = workerCtrl.GetComplaintExplByState(kNotFilled);
  EXPECT_EQ(worker_expls.size(), 1);
  workerCtrl.FillComplaintExpl(worker_expls.front(), "非常抱歉,马上补救");
  // 物业经理检查是否全部填写
  EXPECT_EQ(managerCtrl.ComplaintOk(cmpl), true);
  // 物业经理与客户沟通将情况记录下来并关闭投诉
  managerCtrl.CloseComplaint(cmpl, "业主接受了后续处理,完美解决");
  EXPECT_EQ(cmpl->get_state(), kClose);
  EXPECT_EQ(cmpl->get_result_record(), "业主接受了后续处理,完美解决");
  Worker::ClearAllWorkers();
}

// 需求1--调度员需要了解报修的状态，包括解调度的过程以及维修工的活动记录
// 注：在我的设计下，这部分在前面的测试大都有比较充分的体现，此处专门测试
TEST(SystemRepuirement1, REQUIREMENT_TEST1) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto this_repair = repairs.front();
  EXPECT_EQ(this_repair->get_state(), kToYard);
  yardCtrl.Yard(worker, this_repair, kPowerCut);
  EXPECT_EQ(this_repair->get_state(), KInDeal);
  // 工人维修并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx");
  // 调度员确认报修完成
  auto finished_task = yardCtrl.GetTaskByState(kTaskFinish);
  yardCtrl.FinishRepair(this_repair);
  EXPECT_EQ(this_repair->get_state(), kToComment);
  //  业主评价
  OwnerController ownerCtrl;
  ownerCtrl.set_owner(&owner);
  auto comment_repair = ownerCtrl.GetCanCommentRepair();
  ownerCtrl.DoComment(comment_repair.front(), kVeryTimely, kVeryKindly,
                      kQuiteSatisfied);
  EXPECT_EQ(this_repair->get_state(), kRepairFinish);
  // 获取活动记录
  auto finish_records = yardCtrl.GetRecordByType(kAllOver);
  EXPECT_EQ(finish_records.size() >= 1, true);
  Worker::ClearAllWorkers();
}

// 需求2--维修工人需要了解当前分配给他的各种任务已经完成情况，包括调度给他的报修和需要他处理的投诉
// 注：在我的设计下，这部分在前面的测试大都有比较充分的体现，此处专门测试
TEST(SystemRepuirement2, REQUIREMENT_TEST2) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto repair = repairs.front();
  yardCtrl.Yard(worker, repair, kPowerCut);
  // 工人获取调度给他的报修并工作
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  EXPECT_EQ(tasks.size(), 1);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx");
  yardCtrl.FinishRepair(repair);
  // 业主投诉
  OwnerController ownerCtrl;
  ownerCtrl.set_owner(&owner);
  auto this_repairs = ownerCtrl.GetCanCommentRepair();
  EXPECT_EQ(this_repairs.size(), 1);
  auto this_repair = this_repairs.front();
  EXPECT_NE(this_repair, nullptr);
  ownerCtrl.PullComplaint(this_repair, "修的太差了");
  // 物业经理处理
  ManagerController managerCtrl;
  Manager manager("物业经理X");
  managerCtrl.set_manager(&manager);
  auto cmpls = managerCtrl.GetComplaintByState(kUntreated);
  auto cmpl = cmpls.front();
  managerCtrl.HandleComplaint(cmpl);
  // 工人查看需要填写的投诉情况
  auto worker_expls = workerCtrl.GetComplaintExplByState(kNotFilled);
  EXPECT_EQ(worker_expls.size(), 1);
  workerCtrl.FillComplaintExpl(worker_expls.front(), "非常抱歉,马上补救");
  // 工人查看完成任务
  auto finish_tasks = workerCtrl.GetTaskByState(kTaskFinish);
  EXPECT_EQ(finish_tasks.empty(), false);
}

// 需求3--对于处理中的报修，知道当前活动的调度
// 注：在我的设计下，这部分在前面的测试大都有比较充分的体现，此处专门测试
TEST(SystemRepuirement3, REQUIREMENT_TEST3) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto repair = repairs.front();
  yardCtrl.Yard(worker, repair, kPowerCut);
  auto tasks = Task::FindTaskByRepair(repair);
  EXPECT_EQ(tasks.empty(), false);
}

// 需求4--需要知道一个维修工人当前是否空闲，以便合理调度
TEST(SystemRepuirement4, REQUIREMENT_TEST4) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto repair = repairs.front();
  yardCtrl.Yard(worker, repair, kPowerCut);
  EXPECT_EQ(worker->get_state(), kFree);
  // 工人维修
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  auto task = tasks.front();
  auto& record = workerCtrl.StartMaintenance(task);
  EXPECT_EQ(worker->get_state(), kWorking);
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx");
  EXPECT_EQ(worker->get_state(), kFree);
}

// 需求5--统计处理一次报修已经用掉的工时
TEST(SystemRepuirement5, REQUIREMENT_TEST5) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto this_repair = repairs.front();
  yardCtrl.Yard(worker, this_repair, kPowerCut);
  // 工人维修并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  auto task = tasks.front();
  std::time_t start_time = 0;
  auto& record = workerCtrl.StartMaintenance(task, start_time);
  std::time_t end_time = 1000;
  workerCtrl.AllFinishMaintenance(record, "xx_process_xx", end_time);
  EXPECT_EQ(record.GetLaborTime(), 1000);
  // 测试为求简单直观，直接使用整数构造time_t,实际时间则可使用tm结构然后调用mktime()转化为time_t
}

// 需求6--统计维修工人在一个时间段内的工作时间
TEST(SystemRepuirement6, REQUIREMENT_TEST6) {
  Yardman yardman("调度员A");
  Owner owner("业主B", "2单元703");
  Worker worker1("工人C", std::vector<FaultType>{kPowerCut}),
      worker2("工人D", std::vector<FaultType>{kElevator});
  Worker::AddWorker({worker1, worker2});
  YardmanController yardCtrl;
  yardCtrl.set_yardman(&yardman);
  // 调度员录入报修信息并调度任务
  std::string fault_message = "房间停电了";
  yardCtrl.LogRepair(&owner, std::time(nullptr), fault_message, kPhone);
  auto suitable_workers = yardCtrl.GetSuitableWorker(kPowerCut);
  auto worker = suitable_workers.front();
  auto repairs = yardCtrl.GetRepairByState(kToYard);
  auto this_repair = repairs.front();
  yardCtrl.Yard(worker, this_repair, kPowerCut);
  // 工人维修并记录
  WorkerController workerCtrl;
  workerCtrl.set_worker(worker);
  auto tasks = workerCtrl.GetTaskByState(kYardNotInWord);
  auto task = tasks.front();
  std::time_t start_time = 0;
  std::time_t end_time = 1000;
  auto& record1 = workerCtrl.StartMaintenance(task, start_time);
  workerCtrl.AllFinishMaintenance(record1, "xx_process_xx_partail1", end_time);
  // 工人继续维修并记录
  start_time = 2000;
  end_time = 3000;
  auto& record2 = workerCtrl.StartMaintenance(task, start_time);
  workerCtrl.PartialFinishMaintenance(record2, "xx_process_xx_partail2",
                                      end_time);
  // 工人继续维修并记录
  start_time = 4000;
  end_time = 4500;
  auto& record3 = workerCtrl.StartMaintenance(task, start_time);
  workerCtrl.PartialFinishMaintenance(record3, "xx_process_xx_partail3",
                                      end_time);
  // 工人完全完成维修并记录
  start_time = 100000;
  end_time = 200000;
  auto& record4 = workerCtrl.StartMaintenance(task, start_time);
  workerCtrl.AllFinishMaintenance(record4, "xx_process_xx_all_finish",
                                  end_time);
  std::time_t respected_time =
      (1000 - 0) + (3000 - 2000) + (4500 - 4000);  // =2500
  EXPECT_EQ(workerCtrl.CalculateLaborTime(0, 9000), respected_time);
  // 测试为求简单直观，直接使用整数构造time_t,实际时间则可使用tm结构然后调用mktime()转化为time_t
}
