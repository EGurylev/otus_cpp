#include <gtest/gtest.h>

#include "command_processor.h"

TEST(BulkTest, StaticBlock) {
    std::istringstream iss("cmd1\ncmd2\ncmd3\ncmd4\ncmd5");
    std::ostringstream oss;
    CommandProcessor cmd_proc(3, iss, oss);
    cmd_proc.process();
    std::string result{oss.str()};
    EXPECT_EQ(result,
              std::string("bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5\n"));
}

TEST(BulkTest, MixedBlocks) {
    std::istringstream iss(
        std::string("cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\n") +
        "cmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}\n{\ncmd10\ncmd11");
    std::ostringstream oss;
    CommandProcessor cmd_proc(3, iss, oss);
    cmd_proc.process();
    std::string result{oss.str()};
    EXPECT_EQ(result,
              std::string("bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5, "
                          "cmd6, cmd7, cmd8, cmd9\nbulk: cmd10, cmd11\n"));
}

TEST(BulkTest, NonDefaultBlockEdges) {
    std::istringstream iss("cmd1\n[\ncmd2\ncmd3\n]\n");
    std::ostringstream oss;
    CommandProcessor cmd_proc(5, iss, oss, '[', ']');
    cmd_proc.process();
    std::string result{oss.str()};
    EXPECT_EQ(result, "bulk: cmd1\nbulk: cmd2, cmd3\n");
}