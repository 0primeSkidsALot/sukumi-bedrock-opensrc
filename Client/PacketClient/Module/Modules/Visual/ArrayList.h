#pragma once
#include "../../Utils/DrawUtils.h"
#include "../../Utils/ColorUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class ArrayList : public IModule {
private:
	SettingEnum mode = this;
	SettingEnum animation = this;

	float spacing = 0.f;
public:
	int opacity = 150;
	int arraycoloropa = 150;
	int whiteopacity = 0;
	float alpha2 = 0.01f;
	float test = -17.83f;
	float test2 = 15.65f;
	bool glow = false;
	int ray = 16;
	float radius = 20.9;
	bool focused = false;
	bool invert = false;
	bool modes = true;
	bool keybinds = false;
	bool lowercase = false;
	SettingEnum Fonts = this;
	// Positions
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float positionX = windowSize.x;
	float positionY = 0.f;
	float offsets = 0.f;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	ArrayList();
};
