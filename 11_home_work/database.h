#pragma once
#include <string>
#include <unordered_map>

namespace database {

enum class Status { ok, error_duplicate };

class TwoTableDataBase {
  public:
    Status insert_to_a(std::size_t id, const std::string &name);
    Status insert_to_b(std::size_t id, const std::string &name);
    void drop_a();
    void drop_b();

  private:
    Status insert(std::size_t id, const std::string &name,
                  std::unordered_map<std::size_t, std::string> &table);
    std::unordered_map<std::size_t, std::string> table_a_;
    std::unordered_map<std::size_t, std::string> table_b_;
};

} // namespace database