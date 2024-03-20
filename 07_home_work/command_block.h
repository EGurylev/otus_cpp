#pragma once
#include <chrono>
#include <string>
#include <vector>

class CommandBlock {
  public:
    class Command {
      public:
        Command(size_t time, const std::string &body)
            : time_{time}, body_{body} {}

        size_t time() const { return time_; }

        std::string body() const { return body_; }

      private:
        size_t time_{};
        std::string body_;
    };

    void push(const std::string &command) {
        using namespace std::chrono;
        auto time =
            duration_cast<seconds>(system_clock::now().time_since_epoch())
                .count();
        commands_.emplace_back(time, command);
    }

    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Command;
        const_iterator(std::vector<value_type>::const_iterator it) : it_{it} {}

        const_iterator &operator++() {
            ++it_;
            return *this;
        }

        const value_type &operator*() { return *it_; }
        const value_type *operator->() { return &(*it_); }

        friend bool operator==(const_iterator a, const_iterator b) {
            return a.it_ == b.it_;
        }
        friend bool operator!=(const_iterator a, const_iterator b) {
            return !(a == b);
        }

      private:
        std::vector<value_type>::const_iterator it_;
    };

    const_iterator cbegin() const { return const_iterator(commands_.cbegin()); }

    const_iterator cend() const { return const_iterator(commands_.cend()); }

    void clear() { commands_.clear(); }

    bool empty() const { return commands_.empty(); }

    size_t size() const { return commands_.size(); }

  private:
    std::vector<Command> commands_;
};

std::ostream &operator<<(std::ostream &os, const CommandBlock &cmd_block);
