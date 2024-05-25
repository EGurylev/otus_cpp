#pragma once
#include "database.h"
#include <functional>

namespace database {

class CommandProcessor {
  public:
    std::string process(const std::string &command);

  private:
    std::string process_insert(const std::vector<std::string> &tokens);
    std::string process_truncate(const std::vector<std::string> &tokens);
    std::string process_intercestion() const;
    std::string process_symmetric_difference() const;

    database::TwoTableDataBase database_;
};

} // namespace database