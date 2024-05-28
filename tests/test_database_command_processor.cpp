#include <gtest/gtest.h>

#include "database_command_processor.h"

TEST(CommandDatabaseProcessorTest, TestInsert) {
    database::CommandProcessor processor;

    EXPECT_TRUE(processor.process("INSERT A 0 name1") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 0 name1") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 1 name2") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 0 name2") == "ERR duplicate 0");
    EXPECT_TRUE(processor.process("INSERT WRONG_TABLE_NAME 0 name2") ==
                "ERR Incorrect table name");
    EXPECT_TRUE(processor.process("INSERT A 2") == "ERR command incomplete");
    EXPECT_TRUE(processor.process("INSERT B") == "ERR command incomplete");
}

TEST(CommandDatabaseProcessorTest, TestDrop) {
    database::CommandProcessor processor;

    EXPECT_TRUE(processor.process("TRUNCATE A") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE B") == "OK");

    EXPECT_TRUE(processor.process("INSERT A 0 name1") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE A") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 0 name1") == "OK");

    EXPECT_TRUE(processor.process("INSERT B 0 name1") == "OK");
    EXPECT_TRUE(processor.process("TRUNCATE B") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 0 name1") == "OK");
}

TEST(CommandDatabaseProcessorTest, TestIntersectionAndSymmetricDifference) {
    database::CommandProcessor processor;

    EXPECT_EQ(processor.process("INTERSECTION"), "OK");
    EXPECT_EQ(processor.process("SYMMETRIC_DIFFERENCE"), "OK");

    EXPECT_TRUE(processor.process("INSERT A 0 lean") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 1 sweater") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 2 frank") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 3 violation") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 4 quality") == "OK");
    EXPECT_TRUE(processor.process("INSERT A 5 precision") == "OK");

    EXPECT_TRUE(processor.process("INSERT B 3 proposal") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 4 example") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 5 lake") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 6 flour") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 7 wonder") == "OK");
    EXPECT_TRUE(processor.process("INSERT B 8 selection") == "OK");

    std::string reference_intersection{
        "3,violation,proposal\n4,quality,example\n5,precision,lake\nOK"};
    EXPECT_EQ(processor.process("INTERSECTION"), reference_intersection);

    std::string reference_symmetric_difference{
        "0,lean,\n1,sweater,\n2,frank,\n6,,flour\n7,,wonder\n8,,selection\nOK"};
    EXPECT_EQ(processor.process("SYMMETRIC_DIFFERENCE"),
              reference_symmetric_difference);
}