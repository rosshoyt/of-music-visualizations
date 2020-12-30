#include "GUISubComponent.h"

GUISubComponent::GUISubComponent(GUIComponent* parent) : parent(parent) {}

void GUISubComponent::setupParamGroup() {}

void GUISubComponent::setupParameterListeners() {}

ofParameterGroup& GUISubComponent::getParamGroup() {
	return paramGroup;
}
