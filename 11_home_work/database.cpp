#include "database.h"

namespace database {

Status
TwoTableDataBase::insert(std::size_t id, const std::string &name,
                         std::unordered_map<std::size_t, std::string> &table) {
    Status status{};
    if (auto it{table.find(id)}; it == table.end()) {
        table.insert({id, name});
        status = Status::ok;
    } else {
        status = Status::error_duplicate;
    }
    return status;
}

Status TwoTableDataBase::insert_to_a(std::size_t id, const std::string &name) {
    return insert(id, name, table_a_);
}
Status TwoTableDataBase::insert_to_b(std::size_t id, const std::string &name) {
    return insert(id, name, table_b_);
}

void TwoTableDataBase::drop_a() {
    table_a_.clear();
}

void TwoTableDataBase::drop_b() {
    table_a_.clear();
}

} // namespace database
