#include "database_command_processor.h"
#include <sstream>
#include <vector>

#include <iostream>

namespace database {

std::string
CommandProcessor::process_insert(const std::vector<std::string> &tokens) {
    database::Status status{};
    const std::size_t id{static_cast<std::size_t>(std::stoll(tokens.at(2)))};
    const std::string name{tokens.at(3)};
    if (tokens.at(1) == "A") {
        status = database_.insert_to_a(id, name);
    } else if (tokens.at(1) == "B") {
        status = database_.insert_to_b(id, name);
    } else {
        throw std::logic_error("Incorrect table name");
    }
    if (status == database::Status::error_duplicate) {
        return "ERR duplicate " + std::to_string(id);
    }
    return "OK";
}

std::string
CommandProcessor::process_truncate(const std::vector<std::string> &tokens) {
    if (tokens.at(1) == "A") {
        database_.drop_a();
    } else if (tokens.at(1) == "B") {
        database_.drop_b();
    } else {
        throw std::logic_error("Incorrect table name");
    }
    return "OK";
}

std::string CommandProcessor::convert_rows(const std::vector<Row> &rows) const {
    std::string result;
    for (auto &row : rows) {
        result += std::string(row) + "\n";
    }
    result += "OK";
    return result;
}

std::string CommandProcessor::process_intercestion() const {
    return convert_rows(database_.intersection());
}

std::string CommandProcessor::process_symmetric_difference() const {
    return convert_rows(database_.symmetric_difference());
}

std::string CommandProcessor::process(const std::string &command) {
    std::string trimmed_command{command};
    trimmed_command.pop_back(); // remove last '\n'
    std::stringstream ss(trimmed_command);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(std::move(token));
    }
    if (tokens.empty()) {
        return "";
    }

    try {
        if (tokens.front() == "INSERT") {
            return process_insert(tokens);
        } else if (tokens.front() == "TRUNCATE") {
            return process_truncate(tokens);
        } else if (tokens.front() == "INTERSECTION") {
            return process_intercestion();
        } else if (tokens.front() == "SYMMETRIC_DIFFERENCE") {
            return process_symmetric_difference();
        } else {
            throw std::logic_error("Wrong command");
        }

    } catch (std::out_of_range &e) {
        return "ERR command incomplete";
    } catch (std::exception &e) {
        return "ERR " + std::string(e.what());
    }
    return "OK";
}

} // namespace database