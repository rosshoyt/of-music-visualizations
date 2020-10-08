#pragma once
#include <string>

class AnimationComponent {
public:
	AnimationComponent(std::string uid) : uid(uid) {

	}

	virtual void drawGUI() = 0;

	void drawBackground() {}

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