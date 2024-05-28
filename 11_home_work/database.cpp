#include "database.h"

namespace database {

Status TwoTableDataBase::insert(std::size_t id, const std::string &name,
                                std::map<std::size_t, std::string> &table) {
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

std::vector<Row> TwoTableDataBase::intersection() const {
    std::vector<Row> result;
    for (auto &pair_a : table_a_) {
        if (auto itb{table_b_.find(pair_a.first)}; itb != table_b_.end()) {
            result.emplace_back(pair_a.first, pair_a.second, itb->second);
        }
    }
    return result;
}

std::vector<Row> TwoTableDataBase::symmetric_difference() const {
    std::vector<Row> result;

    for (auto &pair_a : table_a_) {
        if (auto itb{table_b_.find(pair_a.first)}; itb == table_b_.end()) {
            result.emplace_back(pair_a.first, pair_a.second, "");
        }
    }
    for (auto &pair_b : table_b_) {
        if (auto ita{table_a_.find(pair_b.first)}; ita == table_a_.end()) {
            result.emplace_back(pair_b.first, "", pair_b.second);
        }
    }
    return result;
}

void TwoTableDataBase::drop_a() { table_a_.clear(); }

void TwoTableDataBase::drop_b() { table_b_.clear(); }

} // namespace database
