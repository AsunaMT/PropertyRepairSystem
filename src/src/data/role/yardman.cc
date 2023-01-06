#include <data/role/yardman.h>

std::vector<Yardman> Yardman::all_yardmen_ = {};

Yardman::Yardman(std::string name) : name_(name) {}

auto Yardman::get_name() const -> std::string { return this->name_; }