#pragma once
#include <map>
#include <string>
#include <vector>

namespace database {

struct Row {
    std::size_t id;
    std::string name_a;
    std::string name_b;

    operator std::string() const {
        std::string result{std::to_string(id) + "," + name_a + "," + name_b};
        return result;
    }
};

enum class Status { ok, error_duplicate };

class TwoTableDataBase {
  public:
    Status insert_to_a(std::size_t id, const std::string &name);
    Status insert_to_b(std::size_t id, const std::string &name);
    void drop_a();
    void drop_b();
    std::vector<Row> intersection() const;
    std::vector<Row> symmetric_difference() const;

  private:
    Status insert(std::size_t id, const std::string &name,
                  std::map<std::size_t, std::string> &table);
    std::map<std::size_t, std::string> table_a_;
    std::map<std::size_t, std::string> table_b_;
};

} // namespace database