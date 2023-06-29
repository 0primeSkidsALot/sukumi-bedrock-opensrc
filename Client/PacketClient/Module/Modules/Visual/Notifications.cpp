#include "Notifications.h"
#include "../pch.h"

Notifications::Notifications() : IModule(0, Category::VISUAL, "Displays notifications") {
	registerEnumSetting("Theme", &mode, 0);
	mode.addEntry("Sukumi", 0);
	mode.addEntry("Solaris", 1);
	mode.addEntry("Lucid", 2);
	registerBoolSetting("ShowToggle", &showToggle, showToggle);
	//registerBoolSetting("Color", &color, color);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerEnumSetting("Animation", &animation, 0);
	animation.addEntry("Fade", 0);
	animation.addEntry("Normal", 1);

	shouldHide = true;
}

const char* Notifications::getModuleName() {
	return ("Notifications");
}
