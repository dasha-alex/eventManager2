#pragma once
#include <chrono>
#include <string>

class Event {
private:
	std::chrono::sys_days date_;
	std::string description_;
public:
	Event(std::chrono::sys_days date, std::string description);
	std::chrono::sys_days getDate()const;
	std::string getDescription()const;

	bool operator < (const Event& other) const;
};
