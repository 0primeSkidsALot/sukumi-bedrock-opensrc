#include "Interface.h"
#include "../pch.h"
#include <iomanip>

Interface::Interface() : IModule(0, Category::VISUAL, "Displays the HUD") {
	registerEnumSetting("Color", &color, 0);
	color.addEntry("Rainbow", 0);
	color.addEntry("Astolfo", 1);
	color.addEntry("Wave", 2);
	color.addEntry("RGBWave", 3);
	registerBoolSetting("ArmorHUD", &armorHUD, armorHUD);
	registerBoolSetting("Release", &release, release);
	registerBoolSetting("Info", &info, info);
	registerBoolSetting("Average BPS", &avgBps, avgBps);
	registerBoolSetting("nerdindicator", &nerd, nerd);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
	registerIntSetting("Seperation", &spacing, spacing, 5, 500);

	registerFloatSetting("Red", &r, r, 0.f, 255.f);
	registerFloatSetting("Green", &g, g, 0.f, 255.f);
	registerFloatSetting("Blue", &b, b, 0.f, 255.f);
	registerFloatSetting("Red2", &r2, r2, 0.f, 255.f);
	registerFloatSetting("Green2", &g2, g2, 0.f, 255.f);
	registerFloatSetting("Blue2", &b2, b2, 0.f, 255.f);
	shouldHide = true;
}

const char* Interface::getModuleName() {
	return ("Interface");
}

void Interface::onTick(C_GameMode* gm) {
	shouldHide = true;

	if (!avgBps)
	{
		currBps = "0.0";
		if (!bpsQueue.empty())
		{
			bpsQueue.clear();
		}
		tick = 0;
	}
	else
	{
		auto* player = g_Data.getLocalPlayer();
		if (player == nullptr)
		{
			currBps = "0.0";
			if (!bpsQueue.empty())
			{
				bpsQueue.clear();
			}
			tick = 0;
			return;
		}

		if (tick >= 20)
		{
			float total = 0.f;
			if (!bpsQueue.empty())
			{
				for (int i = 0; i < bpsQueue.size(); i++)
				{
					if (i > 20)
						break;
					total += bpsQueue.at(i);
					bpsQueue.erase(bpsQueue.begin());
				}
				total /= (bpsQueue.size() > 20 ? 20 : bpsQueue.size());
				total = truncf(total * 10) / 10;
			}
			std::stringstream ss;
			ss << std::fixed << std::setprecision(1) << total;
			currBps = ss.str();
			//currBps = std::to_string(total);
			tick = 0;
			//bpsQueue.clear();

			bpsQueue.push_back(std::stof(
				std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))
			));
			return;
		}
		bpsQueue.push_back(std::stof(
			std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))
		));
		tick++;
	}
}

void Interface::onEnable()
{
	bpsQueue.clear();
	currBps = "0.0";
	tick = 0;
}

void Interface::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void Interface::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	static int index = 0;
	int curIndex = -index * spacing;
	auto hudColor = ColorUtil::interfaceColor(curIndex);

	if (!clickGUI->hasOpenedGUI) {
		if (armorHUD) {
			vec4_t testRect = vec4_t(armorX, armorY, 50 + armorX, armorY + 20);
			vec2_t armorPos = vec2_t(testRect.x, testRect.y);
			static float constexpr opacity = 10;
			float scale = 3 * 0.26f;
			float spacing = scale + 15.f + 2;
			for (int t = 0; t < 4; t++) {
				C_ItemStack* stack = player->getArmor(t);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, armorPos, 1, 1, false);
					armorPos.x += scale * spacing;
				}
			}
			C_PlayerInventoryProxy* supplies = player->getSupplies();
			C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			if (item->isValid()) DrawUtils::drawItem(item, armorPos, opacity, 1, item->isEnchanted());
		}

		if (release) {
#ifdef _DEBUG
			releaseStr;
#else
			releaseStr;
#endif

			float len = DrawUtils::getTextWidth(&releaseStr, 1) + 6.5;
			vec4_t fpsRectPos = vec4_t(releaseX, releaseY + 7, len + releaseX, releaseY + 20);
			DrawUtils::drawGradientText(vec2_t(fpsRectPos.x + 3, fpsRectPos.y + 3.5), &releaseStr, 1, 1, true);
		}
		//Regen
		if (moduleMgr->getModule<Regen>()->healthvisual)
		{
			if (moduleMgr->getModule<Regen>()->destroy || moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
				std::string regenCount = "Regenerating (" + std::to_string((int)player->getAbsorption()) + "/10)";
				float lColor = DrawUtils::getTextWidth(&regenCount, 1) + 6.5;
				vec4_t RegenRectPos = vec4_t(RegenX, RegenY + 7, lColor + RegenX, RegenY + 20);
				vec2_t regenCountPos = vec2_t(RegenRectPos.x + 3, RegenRectPos.y + 3.5);
				DrawUtils::drawGradientText(regenCountPos, &regenCount, 1, 1, true);
			}
		}


		if (info) {
			auto hudColor = ColorUtil::interfaceColor(curIndex);

			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			float lFPS = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;
			vec4_t fpsRectPos = vec4_t(fpsX, fpsY + 7, lFPS + fpsX, fpsY + 20);
			vec2_t fpsTextPos = vec2_t(fpsRectPos.x + 3, fpsRectPos.y + 3.5);
			DrawUtils::drawGradientText(fpsTextPos, &fpsText, 1, 1, true);
			DrawUtils::fillRoundRectangle(fpsRectPos, MC_Color(0, 0, 0, opacity), false);

			// Speed (BPS)
			auto player = g_Data.getLocalPlayer();
			std::string speedText;
			if (!avgBps)
			{
				speedText = "BPS: " + std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 100) - ((int)player->getBlocksPerSecond() * 100));
			}
			else
			{
				speedText = "BPS: " + currBps;
			}
			float lSpeed = DrawUtils::getTextWidth(&speedText, 1) + 6.5;
			vec4_t speedRectPos = vec4_t(bpsX, bpsY + 6, lSpeed + bpsX, bpsY + 20);
			vec2_t speedPos = vec2_t(speedRectPos.x + 3, speedRectPos.y + 3.5);
			DrawUtils::drawGradientText(speedPos, &speedText, 1, 1, true);
			DrawUtils::fillRoundRectangle(speedRectPos, MC_Color(0, 0, 0, opacity), false);

			// Position
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string position = "XYZ: " + std::to_string((int)floorf(currPos->x)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z));
			float lPos = DrawUtils::getTextWidth(&position, 1) + 6.5;
			vec4_t rectPos = vec4_t(posX, posY + 6, lPos + posX, posY + 20);
			vec2_t textPos = vec2_t(rectPos.x + 3, rectPos.y + 3);
			DrawUtils::drawGradientText(textPos, &position, 1, 1, true);
			DrawUtils::fillRoundRectangle(vec4_t(posX, posY + 6, lPos + posX, posY + 20), MC_Color(0, 0, 0, opacity), false);
		}
		if (nerd) {
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string position = "X: "+ std::string(GRAY) +std::to_string((int)floorf(currPos->x)) + ", " + std::string(RESET) + "Y: " + std::string(GRAY) + std::to_string((int)floorf(currPos->y)) + ", " + std::string(RESET) + "Z: " + std::string(GRAY) + std::to_string((int)floorf(currPos->z)) + ", " + std::string(RESET) + "BP/S: " + std::string(GRAY) + std::to_string((int)player->getBlocksPerSecond()) + "." + std::to_string((int)(player->getBlocksPerSecond() * 100) - ((int)player->getBlocksPerSecond() * 100));
			float lnerd = DrawUtils::getTextWidth(&position, 1) + 6.5;
			vec4_t rectnerd = vec4_t(nerdX, nerdY + 6, lnerd + nerdX, nerdY + 20);
			vec2_t textnerd = vec2_t(rectnerd.x + 3, rectnerd.y + 3);
			DrawUtils::drawText(textnerd, &position, ColorUtil::interfaceColor(1), 1, 1, true);
			DrawUtils::fillRectangleA(vec4_t(nerdX, nerdY + 7, lnerd + nerdX, nerdY + 18), MC_Color(0, 0, 0, opacity));
		}
	}
}

void Interface::onDisable() {
	setEnabled(true);
}