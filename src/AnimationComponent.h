#pragma once
#include <string>

class AnimationComponent {
public:
	AnimationComponent(std::string uid) : uid(uid) {

	}

	const std::string& getUID() {
		return uid;
	}
	void setMenuXY(int x, int y) {
		menuX = x, menuY = y;
	}
	void setBackgroundColor(ofColor color) {
		backgroundColor = color;
	}

	ofColor getBackgroundColor() {
		return backgroundColor;
	}
private:
	std::string uid;

protected:
	int menuX, menuY;

	ofColor backgroundColor;
	


};