#include <data/role/manager.h>

std::vector<Manager> Manager::all_managers_ = {};

Manager::Manager(std::string name) : name_(name) {}

auto Manager::get_name() const -> std::string { return this->name_; }