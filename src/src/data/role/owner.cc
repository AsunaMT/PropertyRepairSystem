#include <data/role/owner.h>

std::vector<Owner> Owner::all_owners_ = {};

Owner::Owner(std::string name, std::string address)
    : name_(name), address_(address) {}

auto Owner::get_name() const -> std::string { return this->name_; }