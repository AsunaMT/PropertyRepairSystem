#ifndef INCLUDE_ROLE_MANAHER_H_
#define INCLUDE_ROLE_MANAHER_H_

#include <memory>
#include <string>
#include <vector>

class Manager {
 private:
  static std::vector<Manager> all_managers_;
  std::string name_;

 public:
  Manager(std::string name);
  auto get_name() const -> std::string;
};

#endif