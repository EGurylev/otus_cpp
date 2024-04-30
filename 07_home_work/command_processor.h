#pragma once
#include "command_block.h"
#include "loggers.h"
#include <memory>
#include <stack>

class IObservable {
  public:
    virtual ~IObservable() = default;

    virtual void subscribe(std::shared_ptr<IObserver> observer) = 0;
};

class CommandProcessor : public IObservable {
  public:
    struct BlockEdge {
        BlockEdge(char value) : value{value} {}
        char value;
        operator char() const { return value; }
    };

    CommandProcessor(size_t block_size, std::istream &is, BlockEdge open = '{',
                     BlockEdge close = '}');

    ~CommandProcessor();

    void process();

    void subscribe(std::shared_ptr<IObserver> observer) override;

  private:
    bool is_opening(const std::string &command) const;

    bool is_closing(const std::string &command) const;

    void process_opening();

    void process_closing();

    void update();

    size_t block_size_{};
    std::istream &is_;
    std::stack<BlockEdge> edges_stack_;
    CommandBlock cmd_buff_;
    BlockEdge open_{'{'};
    BlockEdge close_{'}'};
    std::vector<std::shared_ptr<IObserver>> subsribers_;
};