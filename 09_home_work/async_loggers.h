#pragma once
#include "loggers.h"
#include <condition_variable>
#include <queue>
#include <thread>

namespace async {

class AsyncConsoleLogger : public IObserver {
  public:
    AsyncConsoleLogger(std::ostream &os);
    void update(const CommandBlock &cmd_block) override;
    void stop();

  private:
    void process();

    std::ostream &os_;
    std::queue<CommandBlock> command_blocks_;
    bool stop_{};

    std::mutex m_;
    std::condition_variable cv_;
    std::jthread thread_;
};

class AsyncDuoFileLogger : public IObserver {
  public:
    AsyncDuoFileLogger(const std::string &name_prefix);
    void update(const CommandBlock &cmd_block) override;
    void stop();

  private:
    void write();

    std::string name_prefix_;
    std::queue<CommandBlock> command_blocks_;
    std::size_t block_counter_{};
    bool stop_{};

    std::jthread first_thread_;
    std::jthread second_thread_;
    std::mutex m_;
    std::condition_variable cv_;
};

} // namespace async