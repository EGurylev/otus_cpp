#pragma once
#include "command_block.h"
#include <fstream>
#include <memory>

class IObserver {
  public:
    virtual ~IObserver() = default;

    virtual void update(CommandBlock &cmd_block) = 0;
};

class ConsoleLogger : public IObserver {
  public:
    ConsoleLogger(std::ostream &os);
    void update(CommandBlock &cmd_block) override;

  private:
    std::ostream &os_;
};

class FileLogger : public IObserver {
  public:
    FileLogger(const std::string &name_prefix);
    void update(CommandBlock &cmd_block) override;

  private:
    std::string name_prefix_;
};