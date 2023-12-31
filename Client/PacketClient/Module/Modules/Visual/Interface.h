#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Interface : public IModule {
public:
	std::string clientName = "Skiddle";
	std::string releaseStr = "kukluxklan.club";
	std::string versionStr = "V1.1";

	std::string& getClientName() { return clientName; };
public:
	// Settings
	bool keystrokes = false;
	float saturation = 1.f;
	bool armorHUD = false;
	bool release = false;
	bool tabGUI = false;
	int opacity = 0;
	bool info = true;
	int spacing = 70;
	bool avgBps = false;
	bool nerd = false;
	std::string currBps = "";
	std::vector<float> bpsQueue;
	int tick = 0;

	float scale = 1.f;

	// Positions
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float releaseX = 2.5;
	float releaseY = 2.5;
	float armorX = 2.5;
	float armorY = 60;
	float RegenX = windowSize.x / 2;
	float RegenY = windowSize.y - 60;
	float fpsX = 2.5;
	float fpsY = windowSize.y - 40;
	float bpsX = 2.5f;
	float bpsY = windowSize.y - 30;
	float posX = 2.5f;
	float posY = windowSize.y - 20;
	float nerdX = 2.5f;
	float nerdY = windowSize.y - 20;

	// Colors
	SettingEnum color = this;
	float r = 255.f, g = 255.f, b = 255.f;
	float r2 = 32.f, g2 = 32.f, b2 = 32.f;

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	Interface();
};