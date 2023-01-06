#include <data/comment_sys/comment.h>

std::vector<Comment> Comment::all_comment_ = {};

Comment::Comment(const Owner* owner, const Repair* repair,
                 Timeliness timeliness, ServiceAttitude service_attitude,
                 SatisfactionDegree satisfaction_degree)
    : owner_(owner),
      repair_(repair),
      timeliness_(timeliness),
      service_attitude_(service_attitude),
      satisfaction_degree_(satisfaction_degree) {}

auto Comment::get_timeliness() const -> Timeliness { return this->timeliness_; }

auto Comment::AddComment(const Owner* owner, const Repair* repair,
                         Timeliness timeliness,
                         ServiceAttitude service_attitude,
                         SatisfactionDegree satisfaction_degree) -> Comment& {
  Comment::all_comment_.emplace_back(Comment(
      owner, repair, timeliness, service_attitude, satisfaction_degree));
  return Comment::all_comment_.back();
}

auto Comment::FindAll() -> std::vector<const Comment*> {
  std::vector<const Comment*> res;
  for (auto& comment : Comment::all_comment_) {
    res.emplace_back(&comment);
  }
  return res;
}