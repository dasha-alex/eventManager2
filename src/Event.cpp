#include "Event.h"

Event::Event(std::chrono::sys_days date, std::string description) : date_(date), description_(description) {}

std::chrono::sys_days Event::getDate() const {
	return date_;
}

std::string Event::getDescription() const {
	return description_;
}

bool Event::operator < (const Event& other) const {
	return date_ < other.date_;
}
