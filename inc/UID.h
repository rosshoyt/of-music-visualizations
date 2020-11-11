#pragma once
#include <string>

class UID {
public:
	UID(std::string uid) : uid(uid) {}

	void setUID(std::string uid) {
		this->uid = uid;
	}
	const std::string& getUID() {
		return uid;
	}

private:
	std::string uid;

};