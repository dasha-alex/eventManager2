#include "EventManager.h"
#include <iostream>
#include <chrono>
#include <format>

void sortFile(const std::string filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("Cannot open file for sorting: " + filename);
    }

    std::vector<EventEntry> events;
    std::string line;

    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string dateStr, description;

        iss >> dateStr;
        if (dateStr.size() != 10 || dateStr[2] != '.' || dateStr[5] != '.') {
            if (dateStr.size() == 10 && dateStr[2] == '-' && dateStr[5] == '-') {
                dateStr[2] = '.';
                dateStr[5] = '.';
            }
            else {
                continue;
            }
        }

        try {
            int day = std::stoi(dateStr.substr(0, 2));
            int month = std::stoi(dateStr.substr(3, 2));
            int year = std::stoi(dateStr.substr(6, 4));

            auto date = std::chrono::sys_days{
                std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day}
            };

            std::getline(iss, description);
            if (!description.empty() && description[0] == ' ') {
                description = description.substr(1);
            }

            events.push_back({ date, description, line });
        }
        catch (...) {
            continue; 
        }
    }
    inFile.close();

    std::sort(events.begin(), events.end(), sortFile);

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    for (const auto& event : events) {
        outFile << event.originalLine << "\n";
    }
}

int main() {
    try {
        std::string filename;
        std::cout << "Enter file name: ";
        std::cin >> filename;

        if (!hasTxtExtension(filename)) {
            std::cerr << "Error: File must have .txt extension" << std::endl;
            return 1;
        }

        sortFile(filename);

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
