#include <gtest/gtest.h>

#include "database_command_processor.h"

TEST(CommandDatabaseProcessorTest, TestInsert) {
    database::CommandProcessor processor;

    EXPECT_TRUE(processor.process("INSERT A 0 name1\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 0 name1\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 1 name2\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 0 name2\n") == "ERR duplicate 0");
    EXPECT_TRUE(processor.process("INSERT WRONG_TABLE_NAME 0 name2\n") ==
                "ERR Incorrect table name");
    EXPECT_TRUE(processor.process("INSERT A 2\n") == "ERR command incomplete");
    EXPECT_TRUE(processor.process("INSERT B\n") == "ERR command incomplete");
}

TEST(CommandDatabaseProcessorTest, TestDrop) {
    database::CommandProcessor processor;

    EXPECT_TRUE(processor.process("TRUNCATE A\n") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE B\n") == "OK");

    EXPECT_TRUE(processor.process("INSERT A 0 name1\n") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE A\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 0 name1\n") == "OK");

    EXPECT_TRUE(processor.process("INSERT B 0 name1\n") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE B\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 0 name1\n") == "OK");
}

TEST(CommandDatabaseProcessorTest, TestIntersectionAndSymmetricDifference) {
    database::CommandProcessor processor;

    EXPECT_EQ(processor.process("INTERSECTION\n"), "OK");
    EXPECT_EQ(processor.process("SYMMETRIC_DIFFERENCE\n"), "OK");

    EXPECT_TRUE(processor.process("INSERT A 0 lean\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 1 sweater\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 2 frank\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 3 violation\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 4 quality\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 5 precision\n") == "OK");

    EXPECT_TRUE(processor.process("INSERT B 3 proposal\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 4 example\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 5 lake\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 6 flour\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 7 wonder\n") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 8 selection\n") == "OK");

    std::string reference_intersection{
        "3,violation,proposal\n4,quality,example\n5,precision,lake\nOK"};
    EXPECT_EQ(processor.process("INTERSECTION\n"), reference_intersection);

    std::string reference_symmetric_difference{
        "0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection\nOK"};
    EXPECT_EQ(processor.process("SYMMETRIC_DIFFERENCE\n"),
              reference_symmetric_difference);
}