#include <gtest/gtest.h>
#include "EventManager.h"
#include <chrono>
#include <fstream>
#include <cstdio>

using namespace std::chrono;

class EventManagerTest : public ::testing::Test {
protected:
    std::string filename = "test_events.txt";

    void SetUp() override {
        std::ofstream file(filename);
        file << "15.08.2025 Project kickoff meeting\n";
        file << "20-08-2025 Doctor appointment\n";
        file << "01.09.2025 Start new course\n";
    }

    void TearDown() override {
        std::remove(filename.c_str());
    }
};

TEST_F(EventManagerTest, DefisInput) {
    std::string filename2 = "defis_events.txt";

    std::ofstream file2(filename2);
    file2 << "15.08.2026 Project kickoff meeting\n";
    file2 << "20.08.2026 Doctor appointment\n";
    file2 << "01.09.2026 Start new course\n";

    EventManager manager(filename2);
    auto events = manager.getUpcomingEvents(sys_days{ year{2025} / month{8} / day{10} }); 

    ASSERT_EQ(events.size(), 3);

    EXPECT_EQ(events[0].getDescription(), "Project kickoff meeting");
    EXPECT_EQ(events[1].getDescription(), "Doctor appointment");
    EXPECT_EQ(events[2].getDescription(), "Start new course");

    sys_days date1 = sys_days{ year{2026} / month{8} / day{15} };
    sys_days date2 = sys_days{ year{2026} / month{8} / day{20} };
    sys_days date3 = sys_days{ year{2026} / month{9} / day{1} };

    EXPECT_EQ(events[0].getDate(), date1);
    EXPECT_EQ(events[1].getDate(), date2);
    EXPECT_EQ(events[2].getDate(), date3);

    std::remove(filename2.c_str());
}

TEST_F(EventManagerTest, LoadsEventsFromFile) {
    EventManager manager(filename);

    auto events = manager.getUpcomingEvents(sys_days{year{2025}/month{8}/day{10}});
    ASSERT_EQ(events.size(), 3);

    EXPECT_EQ(events[0].getDescription(), "Project kickoff meeting");
    EXPECT_EQ(events[1].getDescription(), "Doctor appointment");
    EXPECT_EQ(events[2].getDescription(), "Start new course");

    sys_days date1 = sys_days{ year{2025} / month{8} / day{15} };
    sys_days date2 = sys_days{ year{2025} / month{8} / day{20} };
    sys_days date3 = sys_days{ year{2025} / month{9} / day{1} };

    EXPECT_EQ(events[0].getDate(), date1);
    EXPECT_EQ(events[1].getDate(), date2);
    EXPECT_EQ(events[2].getDate(), date3);
}

TEST_F(EventManagerTest, FiltersUpcomingEvents) {
    EventManager manager(filename);

    auto events = manager.getUpcomingEvents(sys_days{year{2025}/month{8}/day{18}});
    ASSERT_EQ(events.size(), 2);

    EXPECT_EQ(events[0].getDescription(), "Doctor appointment");
    EXPECT_EQ(events[1].getDescription(), "Start new course");
}

TEST_F(EventManagerTest, LimitsNumberOfEvents) {
    EventManager manager(filename);

    auto events = manager.getUpcomingEvents(sys_days{year{2025}/month{8}/day{10}}, 2);
    ASSERT_EQ(events.size(), 2);
}

TEST_F(EventManagerTest, ReturnsEmptyIfNoUpcomingEvents) {
    std::string filename3 = "empty_events.txt";
    std::ofstream file3(filename3); 
    file3.close();

    EventManager manager(filename3);
    auto events = manager.getUpcomingEvents(sys_days{ year{2026} / month{1} / day{1} });
    EXPECT_TRUE(events.empty());

    std::remove(filename3.c_str()); 
}