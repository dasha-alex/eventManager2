#pragma once

#include "Event.h"

#include <vector>
#include <string>

class EventManager {
public:
	EventManager(const std::string& filename);

	std::vector<Event> getUpcomingEvents(std::chrono::sys_days date, size_t count = 7);
private:
	std::vector <Event> events;
	void loadFromFile(const std::string& filename);
	std::chrono::sys_days parseDate(const std::string& dateStr);
};
