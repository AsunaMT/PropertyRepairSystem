#ifndef INCLUDE_ROLE_OWNER_H_
#define INCLUDE_ROLE_OWNER_H_

#include <memory>
#include <string>
#include <vector>

class Owner {
 private:
  static std::vector<Owner> all_owners_;
  std::string name_;
  std::string address_;

 public:
  Owner(std::string name, std::string address);
  auto get_name() const -> std::string;
};

#endif