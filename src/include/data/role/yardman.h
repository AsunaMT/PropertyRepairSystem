#ifndef INCLUDE_ROLE_YARDMAN_H_
#define INCLUDE_ROLE_YARDMAN_H_

#include <memory>
#include <string>
#include <vector>

class Yardman {
 private:
  static std::vector<Yardman> all_yardmen_;
  std::string name_;

 public:
  Yardman(std::string name);
  auto get_name() const -> std::string;
};

#endif