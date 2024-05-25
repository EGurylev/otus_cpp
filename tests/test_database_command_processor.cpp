#include <gtest/gtest.h>

#include "database_command_processor.h"

TEST(CommandProcessorTest, TestInsert) {
    database::CommandProcessor processor;

    EXPECT_TRUE(processor.process("INSERT A 0 name1") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 0 name1") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 1 name2") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 0 name2") == "ERR duplicate 0");
    EXPECT_TRUE(processor.process("INSERT WRONG_TABLE_NAME 0 name2") ==
                "ERR Incorrect table name");
    EXPECT_TRUE(processor.process("INSERT A 2") == "ERR command incoplete");
    EXPECT_TRUE(processor.process("INSERT B") == "ERR command incoplete");
}