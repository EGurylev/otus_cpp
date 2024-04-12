#include "test_data.h"
#include <gtest/gtest.h>

#include "file_group.h"

namespace {
auto get_group(const std::string &s) {
    std::unordered_set<std::string> result;

    std::istringstream ss(s);
    std::string token;

    while (std::getline(ss, token)) {
        result.insert(token);
    }

    return result;
}
} // namespace

TEST(BayanTest, DefaultTestRecursive) {
    std::ostringstream oss;
    FileGroupManager manager{
        {fs::path(TEST_DIR + std::string("/bayan_test_cases"))},
        {fs::path(TEST_DIR + std::string("/bayan_test_cases/3"))},
        1,
        "sha1",
        10,
        "",
        1,
        oss};

    manager.find_duplicates();
    manager.print_duplicates();
    const auto result_group{get_group(oss.str())};

    const std::unordered_set<std::string> reference_group{
        TEST_DIR + std::string("/bayan_test_cases/1/file1"),
        TEST_DIR + std::string("/bayan_test_cases/2/file2")};

    EXPECT_EQ(result_group, reference_group);
}

TEST(BayanTest, TestRecursiveWithSplit) {
    std::ostringstream oss;
    FileGroupManager manager{
        {fs::path(TEST_DIR + std::string("/bayan_test_cases/3"))},
        {fs::path("")},
        1,
        "sha1",
        10,
        "",
        1,
        oss};

    manager.find_duplicates();
    manager.print_duplicates();
    const auto result_group{get_group(oss.str())};

    const std::unordered_set<std::string> reference_group{
        TEST_DIR + std::string("/bayan_test_cases/3/file4"),
        TEST_DIR + std::string("/bayan_test_cases/3/file5")};

    EXPECT_EQ(result_group, reference_group);
}

TEST(BayanTest, TestNonRecursiveWithNoFiles) {
    std::ostringstream oss;
    FileGroupManager manager{
        {fs::path(TEST_DIR + std::string("/bayan_test_cases/"))},
        {fs::path("")},
        0,
        "sha1",
        10,
        "",
        1,
        oss};

    manager.find_duplicates();
    manager.print_duplicates();
    std::string result{oss.str()};
    EXPECT_EQ(result.size(), 0);
}

TEST(BayanTest, TestNonRecursiveWithDifferentFiles) {
    std::ostringstream oss;
    FileGroupManager manager{
        {fs::path(TEST_DIR + std::string("/bayan_test_cases/4"))},
        {fs::path("")},
        0,
        "sha1",
        10,
        "",
        1,
        oss};

    manager.find_duplicates();
    manager.print_duplicates();
    std::string result{oss.str()};
    EXPECT_EQ(result.size(), 0);
}

TEST(BayanTest, TestRecursiveWithExluded) {
    std::ostringstream oss;
    FileGroupManager manager{
        {fs::path(TEST_DIR + std::string("/bayan_test_cases"))},
        {fs::path(TEST_DIR + std::string("/bayan_test_cases/3"))},
        1,
        "crc32",
        10,
        "",
        1,
        oss};

    manager.find_duplicates();
    manager.print_duplicates();
    const auto result_group{get_group(oss.str())};

    const std::unordered_set<std::string> reference_group{
        TEST_DIR + std::string("/bayan_test_cases/1/file1"),
        TEST_DIR + std::string("/bayan_test_cases/2/file2")};

    EXPECT_EQ(result_group, reference_group);
}