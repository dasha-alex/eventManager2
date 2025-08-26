#include "EventManager.h"
#include <iostream>
#include <chrono>
#include <format>

int main() {
    try {
        std::string filename;
        std::cout << "Enter file name: ";
        std::cin >> filename;
        EventManager manager(filename);

        auto today = std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );

        auto upcoming = manager.getUpcomingEvents(today);

        if (upcoming.empty()) {
            std::cout << "No upcoming events." << std::endl;
        }
        else {
            for (const auto& e : upcoming) {
                auto ymd = std::chrono::year_month_day(e.getDate());
                std::cout
                    << std::format("{:02}.{:02}.{:04} ",
                        unsigned(ymd.day()),
                        unsigned(ymd.month()),
                        int(ymd.year()))
                    << e.getDescription() << "\n";
            }
        }

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
