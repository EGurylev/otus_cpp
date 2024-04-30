#pragma once
#include "loggers.h"
#include <condition_variable>
#include <thread>

namespace async {

class AsyncConsoleLogger : public IObserver {
  public:
    AsyncConsoleLogger(std::ostream &os);
    ~AsyncConsoleLogger();
    void update(const CommandBlock &cmd_block) override;
    void stop();

  private:
    void process();

    std::ostream &os_;
    CommandBlock cmd_block_;
    std::mutex m_;
    std::condition_variable cv_;
    std::thread thread_;
    bool stop_{};
};

} // namespace async