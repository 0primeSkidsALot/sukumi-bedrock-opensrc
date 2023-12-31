#include "TargetHUD.h"
#include "../pch.h"
int teststfu = 0;
int teststfu2 = 0;

using namespace std;
TargetHUD::TargetHUD() : IModule(0, Category::VISUAL, "Displays information about your target") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("New", 0);
	mode.addEntry("Old", 1);
	mode.addEntry("Simple", 2);
	mode.addEntry("Novoline", 3);
	mode.addEntry("Pulsive", 4);
	mode.addEntry("Rise", 5);
	mode.addEntry("Zephyr", 6);
	mode.addEntry("Grasscutter", 7);
	mode.addEntry("Sukumi", 8);
	//	mode.addEntry("Unfinished", 8);
	mode.addEntry("Moon", 9);
	mode.addEntry("SukumiNew", 10);
	mode.addEntry("Vestige", 11);
	registerBoolSetting("Animation", &animation, animation);
	registerBoolSetting("Items", &showItems, showItems);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	//registerFloatSetting("Pos", &pos, pos, 0, 15);
}
const char* TargetHUD::getModuleName() { return ("TargetHUD"); }

#pragma region TargetList
static bool entityChanged = false;
static vector<C_Entity*> targetList;
void findPlayers_TargetHUD(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity->getNameTag()->getTextLength() <= 1) return;
	if (!TargetUtil::isValidTarget(currentEntity)) return;
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < 8) targetList.push_back(currentEntity);
}
#pragma endregion

#pragma region TargetArray
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
#pragma endregion

void TargetHUD::onTick(C_GameMode* gm) {
	targetList.clear();
	g_Data.forEachEntity(findPlayers_TargetHUD);
	targetListEmpty = targetList.empty();
	sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty()) entityChanged = true;
	else entityChanged = false;
	displayhealth = 0;
	displayhealth2 = 0;
}

void TargetHUD::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI) {

		teststfu++;
		teststfu2++;

		if (entityChanged && animation) {
			positionX += ((windowSize.x / 2 + 10) - positionX) * 0.045f; positionY += ((windowSize.y / 2 + 10) - positionY) * 0.045f;
		}
		else { positionX = windowSize.x; positionY = windowSize.y; }
		if (!targetListEmpty) {
			auto absorbtion = targetList[0]->getAbsorption();
			string percent = "100%";
			auto interfaceColor = ColorUtil::interfaceColor(1);
			vec3_t* pos = targetList[0]->getPos();
			auto health = targetList[0]->getHealth();
			int inthealth = health;
			int healthcolor = inthealth * 12.75;
			int Absorbtionhp = ((int)targetList[0]->getAbsorption());
			int percentage = (health / 20) * 100;
			string displaypercent = to_string(percentage);
			if (displayhealth > health) displayhealth -= ((displayhealth - health) / 15);
			else if (displayhealth < health) displayhealth += ((health - displayhealth) / 15);
			if (displayhealth > health) displayhealth -= ((displayhealth - health) / 15);
			else if (displayhealth < health) displayhealth += ((health - displayhealth) / 15);
			if (displayhealth2 > Absorbtionhp) displayhealth2 -= ((displayhealth2 - Absorbtionhp) / 15);
			else if (displayhealth2 < Absorbtionhp) displayhealth2 += ((Absorbtionhp - displayhealth2) / 15);
			if (displayhealth2 > Absorbtionhp) displayhealth2 -= ((displayhealth2 - Absorbtionhp) / 15);
			else if (displayhealth2 < Absorbtionhp) displayhealth2 += ((Absorbtionhp - displayhealth2) / 15);
			string smoothtext = to_string(inthealth) + ".0 health left";
			float steveOpacity = (targetList[0]->damageTime * 80) / 8;
			float dist = (*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
			float defaultRectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			float novolineRectHeight = (3.25f, 3.25f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			float zephyrRectHeight = (3.0f, 3.0f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			if (showItems && mode.getSelectedValue() == 1) defaultRectHeight = (5, 2) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			string position = "Position: " + to_string((int)floorf(pos->x)) + " " + to_string((int)floorf(pos->y)) + " " + to_string((int)floorf(pos->z));
			string distance = string(GRAY) + "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
			string distancev2 = "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10)) + "m";
			string targetName = targetList[0]->getNameTag()->getText();
			targetName = Utils::sanitize(targetName);
			targetName = targetName.substr(0, targetName.find('\n'));

			if (!targetListEmpty) {
				if (mode.getSelectedValue() == 0) {
					string distance = string(GRAY) + "         Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
					string name = string(GRAY) + "         Name: " + string(RESET);
					string rawName = targetList[0]->getNameTag()->getText();
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					if (showItems) testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), false);
					vec4_t healthRect = vec4_t(testRect.x + 39, testRect.y + 32, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 34);
					if (showItems) healthRect = vec4_t(testRect.x + 4, testRect.y + 44, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 47);
					if (targetList[0]->damageTime > 1) {
						DrawUtils::drawRectangle(healthRect, MC_Color(255, 0, 0), 0.3f);
						DrawUtils::fillRectangleA(healthRect, MC_Color(255, 0, 0, steveOpacity));
					}
					else {
						DrawUtils::drawRectangle(healthRect, MC_Color(0, 255, 0), 0.3);
						DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
					}
					vec4_t absorbtionRect = vec4_t(testRect.x + 39, testRect.y + 26, testRect.x + absorbtion * 4.f, testRect.y + 29);
					if (showItems) absorbtionRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + absorbtion * 4.f, testRect.y + 41);
					vec4_t absorbtionSubRect = vec4_t(testRect.x + 39, testRect.y + 28, testRect.x + targetLen - 4.f, testRect.y + 30);
					if (showItems) absorbtionSubRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + targetLen - 4.f, testRect.y + 41);
					DrawUtils::fillRectangleA(absorbtionSubRect, MC_Color(25, 25, 25, 255));

					if (absorbtion > 1.f) {
						DrawUtils::drawRectangle(absorbtionRect, MC_Color(255, 230, 0), 0.3);
						DrawUtils::fillRectangleA(absorbtionRect, MC_Color(255, 230, 0, 255));
					}

					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX + 5, positionY + 5), vec2_t(30, 30), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 5, 35 + positionX, positionY + 35), MC_Color(255, 0, 0, steveOpacity));
					}
				}

				if (mode.getSelectedValue() == 4) { // pulsive
					float defaultRectHeight = (5.5, 5.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX + 5, positionY + 2), vec2_t(30, 30), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 5, testRect.y + 38, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 40);
					DrawUtils::fillRoundRectangle(healthRect, MC_Color(0, 255, 0, 100), true);
				}


				if (mode.getSelectedValue() == 10) { // Sukumi
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					float defaultRectHeight = (5.5, 4.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "ABCDEFGHJIKLMNOPQ";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 9;
					int maxsize = 31;
					int minsize = 25;
					if (damageTime > 1) {
						if (damageTime > 5) {
							if (size > minsize) size -= ((size - minsize) / smoothing);
							else if (size < minsize) size += ((minsize - size) / smoothing);
						}
						else {
							if (size > maxsize) size -= ((size - maxsize) / smoothing);
							else if (size < maxsize) size += ((maxsize - size) / smoothing);
						}
					}
					float size2 = size / 30;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX + 3, positionY + 1), vec2_t(25, 25), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 3, testRect.y + 30, testRect.x + ((targetLen - 4) / 19.85) * health, testRect.y + 32);
					//DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					DrawUtils::fillRoundRectangleGradient(healthRect, 2);
					DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 2);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					static bool a[16] = { false };
					static float randomx[16] = { 0 };
					static float randomy[16] = { 0 };

					// rise particle animation
					float imgsize = 25;
					vec4_t riseRect = vec4_t(positionX, positionY, positionX + 140, positionY + 255);
					for (int i = 0; i < 16; i++) {
						if (targetList[0]->damageTime > 1) {
							if (!a[i]) {
								randomx[i] = randomFloat(riseRect.x + 4.5, riseRect.x + imgsize + 5.5);
								randomy[i] = randomFloat(riseRect.y + 2, riseRect.y + imgsize + 2.5);
								a[i] = true;
							}
							randomx[i] *= (i % 8 < 4) ? 1.0005f : 0.9995f;
							randomy[i] *= (i % 4 < 2) ? 1.0005f : 0.9995f;
							float size2 = (i % 2 == 0) ? 1.1f : (i % 4 == 1) ? 0.7f
								: (i % 4 == 2) ? 0.8f
								: 0.9f;
							DrawUtils::drawCircleFilled(vec2_t(randomx[i], randomy[i]), vec2_t(size2, size2), MC_Color(interfaceColor), 1);
							DrawUtils::drawCircleFilled(vec2_t(randomx[i], randomy[i]), vec2_t(size2, size2), MC_Color(interfaceColor), 1);
						}
						else {
							a[i] = false;
						}
					}
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 3, positionY + 1, 28 + positionX, positionY + 26), MC_Color(255, 0, 0, steveOpacity));
					}
				}


				if (mode.getSelectedValue() == 5) { // Rise
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					float defaultRectHeight = (5.5, 5.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "ABCDEFGHJIKLMNOPQ";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 3;
					int smoothing2 = 15;
					int maxsize = 30;
					int minsize = 27;
					int redsize1 = 35 + positionX;
					int redsize2 = positionY + 32;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/alex.png", vec2_t(positionX + 5, positionY + 2), vec2_t(30, 30), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 5, testRect.y + 35, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 42);
					//DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					DrawUtils::fillRoundRectangleGradient(healthRect, 2);
					DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 2);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 2, 35 + positionX, positionY + 32), MC_Color(255, 0, 0, steveOpacity));
					}
				}
				if (mode.getSelectedValue() == 7) { // Grasscutter
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					float defaultRectHeight = (5.5, 4.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "ABCDEFGHJIKLMNOPQ";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 3;
					int smoothing2 = 15;
					int maxsize = 30;
					int minsize = 27;
					int redsize1 = 35 + positionX;
					int redsize2 = positionY + 32;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/alex.png", vec2_t(positionX + 5, positionY + 2), vec2_t(30, 30), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 39, testRect.y + 28, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 30);
					//DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					DrawUtils::fillRoundRectangleGradient(healthRect, 2);
					DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 2);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 2, 35 + positionX, positionY + 32), MC_Color(255, 0, 0, steveOpacity));
					}
				}
				if (mode.getSelectedValue() == 8) { // Sukumi
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					float defaultRectHeight = (5.5, 4.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "ABCDEFGHJIKLMNOPQ";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 9;
					int maxsize = 31;
					int minsize = 25;
					if (damageTime > 1) {
						if (damageTime > 5) {
							if (size > minsize) size -= ((size - minsize) / smoothing);
							else if (size < minsize) size += ((minsize - size) / smoothing);
						}
						else {
							if (size > maxsize) size -= ((size - maxsize) / smoothing);
							else if (size < maxsize) size += ((maxsize - size) / smoothing);
						}
					}
					float size2 = size / 30;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX + 3, positionY + 1), vec2_t(32, 32), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 37, testRect.y + 22, testRect.x + 2 + ((targetLen - 4) / 20) * health, testRect.y + 32.5);
					//DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					DrawUtils::fillRoundRectangleGradient(healthRect, 2);
					DrawUtils::fillRectangle(healthRect, interfaceColor, 2);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					static bool a[16] = { false };
					static float randomx[16] = { 0 };
					static float randomy[16] = { 0 };

					// rise particle animation
					float imgsize = 26;
					vec4_t riseRect = vec4_t(positionX, positionY, positionX + 140, positionY + 32);
					for (int i = 0; i < 16; i++) {
						if (targetList[0]->damageTime > 1) {
							if (!a[i]) {
								randomx[i] = randomFloat(riseRect.x + 4.5, riseRect.x + imgsize + 5.5);
								randomy[i] = randomFloat(riseRect.y + 2, riseRect.y + imgsize + 2.5);
								a[i] = true;
							}
							randomx[i] *= (i % 8 < 4) ? 1.0005f : 0.9995f;
							randomy[i] *= (i % 4 < 2) ? 1.0005f : 0.9995f;
							float size2 = (i % 2 == 0) ? 1.1f : (i % 4 == 1) ? 0.7f
								: (i % 4 == 2) ? 0.8f
								: 0.9f;
							DrawUtils::drawCircleFilled(vec2_t(randomx[i], randomy[i]), vec2_t(size2, size2), MC_Color(199, 155, 229, 255), 1);
							DrawUtils::drawCircleFilled(vec2_t(randomx[i], randomy[i]), vec2_t(size2, size2), MC_Color(255, 105, 179, 255), 1);
						}
						else {
							a[i] = false;
						}
					}
					if (targetList[0]->damageTime > 1) {
						//DrawUtils::fillRectangleA(vec4_t(positionX + 1, positionY + 1, 35 + positionX, positionY + 32), MC_Color(255, 0, 0, steveOpacity));
					}
				}


				if (mode.getSelectedValue() == 3) { // novoline
					float targetLen = 37.f + DrawUtils::getTextWidth(&targetName, 1);
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + novolineRectHeight);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), false);
					vec4_t healthRect = vec4_t(testRect.x + 35, testRect.y + 20, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 30);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					DrawUtils::drawText(healthTextPos, &percent, MC_Color(255, 255, 255), 1, 1, true);
					float opacity = (targetList[0]->damageTime * 80) / 8;
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(healthRect, MC_Color(255, 0, 0, opacity));
					}
					else {
						DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
					}
					DrawUtils::drawSteve(vec4_t(positionX + 2.f, positionY + 1.f, positionX, positionY));
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 2.f, positionY, 32.f + positionX, positionY + 32.f), MC_Color(255, 0, 0, opacity));
					}
				}

				if (mode.getSelectedValue() == 6) { // Zephyr
					string fakename = "ABCDEFGHJIKLMNOPQRS";
					float targetLen = 37.f + DrawUtils::getTextWidth(&fakename, 1);
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + zephyrRectHeight);
					DrawUtils::setColor(0, 0, 0, opacity);
					//DrawUtils::fillRectangle(vec2_t(testRect.x, testRect.y), vec2_t(testRect.z, testRect.w));
					DrawUtils::fillRectangleA(testRect, MC_Color(0, 0, 0, opacity));
					vec4_t healthRect = vec4_t(testRect.x + 35, testRect.y + 20, testRect.x + ((targetLen - 4) / 20) * health, testRect.y + 30);
					float opacity = (targetList[0]->damageTime * 80) / 8;
					if (targetList[0]->damageTime > 1) {
						//DrawUtils::fillRectangleA(vec4_t(positionX + 2.f, positionY, 32.f + positionX, positionY + 32.f), MC_Color(255, 0, 0, opacity));
						DrawUtils::fillRectangleA(testRect, MC_Color(200, 0, 0, opacity));
					}
					vec4_t linepos = vec4_t(testRect.x, testRect.w, testRect.z, testRect.w + 1);
					MC_Color color = ColorUtil::interfaceColor(1);
					DrawUtils::fillRectangleA(linepos, color);
				}

				if (mode.getSelectedValue() == 10) {
					int maxHealth = 20;
					float green = (maxHealth - (float)health) / maxHealth;
					float red = 1.0f - green;
					int dmg;
					dmg = (int)(red * 100.f);
					string percent = to_string(dmg);
					string name = string(GRAY) + "         Name: " + string(RESET);
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					string distance = "         Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
					string smoothtext = string(RESET) + percent + "%";
					DrawUtils::drawText(vec2_t(testRect.x + 60, testRect.y + 5), &targetName, MC_Color(interfaceColor), 1.0, 1.0, true);
					DrawUtils::drawText(vec2_t(testRect.x + -5, testRect.y + 5), &name, MC_Color(100, 100, 100), 1.0, 1.0, true);
					DrawUtils::drawText(vec2_t(testRect.x + 30, testRect.y + 15), &smoothtext, MC_Color(interfaceColor), 1.0, 1.0, true);
				}


				if (mode.getSelectedValue() == 1) {
					if (showItems) defaultRectHeight = (7, 2) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					string name = targetList[0]->getNameTag()->getText();
					name = Utils::sanitize(name);
					string namestr = "Name: " + name;
					namestr = namestr.substr(0, namestr.find('\n'));
					if (namestr.length() < position.length()) targetLen = DrawUtils::getTextWidth(&position, 1) + 6.5;
					else targetLen = DrawUtils::getTextWidth(&namestr, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 10);
					if (showItems) testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight * 2.5f);
					DrawUtils::fillRectangleA(testRect, MC_Color(0, 0, 0, opacity));
					vec4_t healthRect = vec4_t(testRect.x + 4, testRect.y + 30, testRect.x + targetLen - 4, testRect.y + 35);
					if (showItems) healthRect = vec4_t(testRect.x + 4, testRect.y + 40, testRect.x + targetLen - 4, testRect.y + 45);
					float opacity = (targetList[0]->damageTime * 80) / 8;
					if (targetList[0]->damageTime > 1) {
						DrawUtils::drawRectangle(healthRect, MC_Color(255, 0, 0), 0.3);
						DrawUtils::fillRectangleA(healthRect, MC_Color(255, 0, 0, 90));
					}
					else {
						DrawUtils::drawRectangle(healthRect, MC_Color(0, 255, 0), 0.3);
						DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
					}
				}
				if (mode.getSelectedValue() == 9) { // moon
					float defaultRectHeight = (40, 40);
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					targetLen = 150.5;
					vec4_t testRect = vec4_t(positionX - 1, positionY - 1, targetLen + positionX + 1, positionY + defaultRectHeight + 1);
					DrawUtils::fillRectangleA(testRect, MC_Color(30, 30, 30, opacity));
					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX, positionY), vec2_t(40, 40), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t gayRect = vec4_t(testRect.x + 41.5, testRect.y + 35.5, testRect.x + 12, testRect.y + 40.5);
					DrawUtils::fillRectangleA(gayRect, MC_Color(0, 0, 0, 160));
					if (Absorbtionhp == 0) {
						vec4_t healthRect = vec4_t(testRect.x + 42, testRect.y + 36, testRect.x + 42 + ((86.5) / 20) * health, testRect.y + 40);
						DrawUtils::fillRectangleA(healthRect, MC_Color(255 - healthcolor, healthcolor, 0));
					}

					if (Absorbtionhp >= 0.1) {
						vec4_t healthRect = vec4_t(testRect.x + 42, testRect.y + 36, testRect.x + 42 + ((86.5) / 10) * Absorbtionhp, testRect.y + 40);
						DrawUtils::fillRectangleA(healthRect, MC_Color(200, 200, 10, 255));
					}

				}
				if (mode.getSelectedValue() == 11) { // Grasscutter
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					float defaultRectHeight = (3.5, 5.0) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "ABCDEFGHJIKLMNOPQ";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 3;
					int smoothing2 = 15;
					int maxsize = 30;
					int minsize = 27;
					int redsize1 = 35 + positionX;
					int redsize2 = positionY + 32;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length()) targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX - 25, positionY + defaultRectHeight - 11);
					vec4_t testRect4 = vec4_t((testRect.x) + 4, (testRect.y) + 2, (testRect.z) - 4, (testRect.w) - 2);
					if (19 > 0) DrawUtils::drawGlow(testRect4, MC_Color(0, 0, 0, 255), 0.75 / 19, 19, 4);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawImage("textures/entity/alex.png", vec2_t(positionX + 84, positionY + 2), vec2_t(35, 35), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					vec4_t healthRect = vec4_t(testRect.x + 4, testRect.y + 28, testRect.x + ((targetLen - 4) / 35) * health, testRect.y + 37);
					//DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					DrawUtils::fillRoundRectangleGradient(healthRect, 2);
					DrawUtils::fillRectangle(healthRect, interfaceColor, 2);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y + 1.f);
					if (targetList[0]->damageTime > 1) {
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 2, 35 + positionX, positionY + 32), MC_Color(255, 0, 0, steveOpacity));
					}
				}
			}

			for (auto& i : targetList) {
				if (mode.getSelectedValue() == 0) {
					string namestr = string(GRAY) + "Name: " + string(RESET) + targetName;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight);
					vec2_t namePos = vec2_t(testRect.x + 39, testRect.y + 5);
					vec2_t distPos = vec2_t(testRect.x + 39, testRect.y + 15);
					vec2_t armorPos = vec2_t(testRect.x + 39, testRect.y + 25);
					DrawUtils::drawText(distPos, &distance, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(namePos, &namestr, MC_Color(255, 255, 255), 1, 1, true);
					if (i->getEntityTypeId() == 319 && showItems) {
						static float constexpr opacity = 10;
						float scale = 3 * 0.26f;
						float spacing = scale + 15.f + 2;
						auto* player = reinterpret_cast<C_Player*>(targetList[0]);
						for (int t = 0; t < 4; t++) {
							C_ItemStack* stack = player->getArmor(t);
							if (stack->isValid()) {
								DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
								armorPos.x += scale * spacing;
							}
						}
						C_PlayerInventoryProxy* supplies = player->getSupplies();
						C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
						if (item->isValid()) DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
					}
				}

				if (mode.getSelectedValue() == 3) {
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + novolineRectHeight);
					DrawUtils::drawText(vec2_t(testRect.x + 35, testRect.y + 5), &targetName, MC_Color(255, 255, 255), 1, 1, true);
				}

				// Zephyr
				if (mode.getSelectedValue() == 6) {
					static AbsorptionAttribute attribute = AbsorptionAttribute();
					int goldheart = targetList[0]->getMutableAttribute(&attribute)->currentValue;
					string textdist = "Distance: " + to_string(int(dist));
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + novolineRectHeight);
					DrawUtils::drawText(vec2_t(testRect.x + 2, testRect.y + 5), &targetName, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(vec2_t(testRect.x + 2, testRect.y + 15), &textdist, MC_Color(255, 255, 255), 1, 1, true);
				}

				if (mode.getSelectedValue() == 4) {
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					DrawUtils::drawText(vec2_t(testRect.x + 40, testRect.y + 15), &targetName, MC_Color(255, 255, 255), 1, 1, true);
				}


				if (mode.getSelectedValue() == 7) {
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					string distance = "         Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
					DrawUtils::drawText(vec2_t(testRect.x + 40, testRect.y + 3), &targetName, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(vec2_t(testRect.x + 4, testRect.y + 15), &distance, MC_Color(interfaceColor), 1, 1, true);
				}
				if (mode.getSelectedValue() == 11) {
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					string distance = "         " + to_string(inthealth) + " HP";
					DrawUtils::drawText(vec2_t(testRect.x + 4, testRect.y + 3), &targetName, MC_Color(255, 255, 255), 0.9, 0.9, true);
					DrawUtils::drawText(vec2_t(testRect.x + -28, testRect.y + 15), &distance, MC_Color(255, 255, 255), 0.9, 0.9, true);
				}

				if (mode.getSelectedValue() == 8) {
					auto interfaceColor = ColorUtil::interfaceColor(1); //Niggers?
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					string distance = "         Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
					string smoothtext = "HP: " + to_string(inthealth);
					DrawUtils::drawText(vec2_t(testRect.x + 37, testRect.y + 2), &targetName, MC_Color(255, 255, 255), 0.7, 0.7, true);
					DrawUtils::drawText(vec2_t(testRect.x + 12, testRect.y + 15.5), &distance, MC_Color(255, 255, 255), 0.7, 0.7, true);
					DrawUtils::drawText(vec2_t(testRect.x + 37, testRect.y + 9), &smoothtext, MC_Color(255, 255, 255), 0.7, 0.7, true);
				}

				if (mode.getSelectedValue() == 5) {
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					DrawUtils::drawText(vec2_t(testRect.x + 40, testRect.y + 15), &targetName, MC_Color(255, 255, 255), 1, 1, true);
				}

				if (mode.getSelectedValue() == 9) {
					string gaytext = string(YELLOW) + to_string(Absorbtionhp) + string(GRAY) + "/" + string(YELLOW) + "10";
					string notgaytext = string(GRAY) + "/" + string(GREEN) + "20";
					string stringhealth = to_string(inthealth);
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					string distance = "Dist: " + string(GRAY) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10)) + string("m");
					int damageTime = targetList[0]->damageTime;
					std::string hurtTime = "Hurt:" + string(GRAY) + std::to_string(damageTime);
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					DrawUtils::drawText(vec2_t(testRect.x + 42, testRect.y + 2), &targetName, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(vec2_t(testRect.x + 42, testRect.y + 11), &distance, MC_Color(255, 255, 255), 0.8, 1, true);
					DrawUtils::drawText(vec2_t(testRect.x + 42, testRect.y + 19), &hurtTime, MC_Color(255, 255, 255), 0.8, 1, true);

					if (Absorbtionhp == 0) {
						DrawUtils::drawText(vec2_t(testRect.x + 138, testRect.y + 35), &notgaytext, MC_Color(255, 255, 255), 0.7, 1, true);
						DrawUtils::drawText(vec2_t(testRect.x + 130, testRect.y + 35), &stringhealth, MC_Color(255 - healthcolor, healthcolor, 0), 0.7, 1, true);
					}
					if (Absorbtionhp >= 0.1) {
						DrawUtils::drawText(vec2_t(testRect.x + 133, testRect.y + 35), &gaytext, MC_Color(255, 255, 255), 0.7, 1, true);
					}

				}

				// Old
				if (mode.getSelectedValue() == 1) {
					string name = targetList[0]->getNameTag()->getText();
					name = Utils::sanitize(name);
					string namestr = "Name: " + name;
					namestr = namestr.substr(0, namestr.find('\n'));
					static const float rectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					if (namestr.length() < position.length()) targetLen = DrawUtils::getTextWidth(&position, 1) + 6.5;
					else targetLen = DrawUtils::getTextWidth(&namestr, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + rectHeight);
					vec2_t namePos = vec2_t(testRect.x + 4, testRect.y + 5);
					vec2_t distPos = vec2_t(testRect.x + 4, testRect.y + 15);
					vec2_t armorPos = vec2_t(testRect.x + 4, testRect.y + 25);
					DrawUtils::drawText(distPos, &position, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(namePos, &namestr, MC_Color(255, 255, 255), 1, 1, true);
					if (i->getEntityTypeId() == 319 && showItems) {
						static float constexpr opacity = 10;
						float scale = 3 * 0.26f;
						float spacing = scale + 15.f + 2;
						auto* player = reinterpret_cast<C_Player*>(targetList[0]);
						for (int t = 0; t < 4; t++) {
							C_ItemStack* stack = player->getArmor(t);
							if (stack->isValid()) {
								DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
								armorPos.x += scale * spacing;
							}
						}
						C_PlayerInventoryProxy* supplies = player->getSupplies();
						C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
						if (item->isValid()) DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
					}
				}

				// Simple
				if (mode.getSelectedValue() == 2) {
					// Simple
					if (mode.getSelectedValue() == 2) {
						static const float rectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
						float targetLen = 37.f + DrawUtils::getTextWidth(&targetName, 1);
						vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + rectHeight);
						vec2_t namePos = vec2_t(testRect.x + 4, testRect.y + 5);
						vec2_t distPos = vec2_t(testRect.x + 4, testRect.y + 15);
						vec2_t armorPos = vec2_t(testRect.x + 4, testRect.y + 25);
						DrawUtils::drawText(distPos, &distance, MC_Color(255, 255, 255), 1, 1, true);
						DrawUtils::drawText(namePos, &targetName, MC_Color(255, 255, 255), 1, 1, true);
						if (i->getEntityTypeId() == 319 && showItems) {
							static float constexpr opacity = 10;
							float scale = 3 * 0.26f;
							float spacing = scale + 15.f + 2;
							auto* player = reinterpret_cast<C_Player*>(targetList[0]);
							for (int t = 0; t < 4; t++) {
								C_ItemStack* stack = player->getArmor(t);
								if (stack->isValid()) {
									DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
									armorPos.x += scale * spacing;
								}
							}
							C_PlayerInventoryProxy* supplies = player->getSupplies();
							C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
							if (item->isValid()) DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
						}
					}
				}
			}
		}

		if (clickGUI->isEnabled() && !clickGUI->hasOpenedGUI && targetList.size() == 0) { // if theres no targets use local player
			auto absorbtion = 0.f/*g_Data.getLocalPlayer()->getAttribute(&attribute)->currentValue*/;
			float dist = 0;
			float defaultRectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			string distance = string(GRAY) + "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
			string name = string(GRAY) + "Name: " + string(RESET);
			string rawName = g_Data.getLocalPlayer()->getNameTag()->getText();
			rawName = Utils::sanitize(rawName);
			string nlelel = string(GRAY) + "Name:          " + string(RESET) + rawName;
			rawName = name + rawName; rawName = rawName.substr(0, rawName.find('\n'));
			targetLen = DrawUtils::getTextWidth(&nlelel, 1) + 6.5;
			vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
			if (showItems) testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight);
			DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), false);
			vec4_t healthRect = vec4_t(testRect.x + 39, testRect.y + 32, testRect.x + ((targetLen - 4) / 20) * 20, testRect.y + 34);
			if (showItems) healthRect = vec4_t(testRect.x + 4, testRect.y + 44, testRect.x + ((targetLen - 4) / 20) * 20, testRect.y + 47);
			vec2_t namePos = vec2_t(testRect.x + 39, testRect.y + 5);
			vec2_t distPos = vec2_t(testRect.x + 39, testRect.y + 15);
			DrawUtils::drawText(distPos, &distance, MC_Color(255, 255, 255), 1, 1, true);
			DrawUtils::drawText(namePos, &rawName, MC_Color(255, 255, 255), 1, 1, true);
			DrawUtils::drawRectangle(healthRect, MC_Color(0, 255, 0), 0.3);
			DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
			vec4_t absorbtionRect = vec4_t(testRect.x + 39, testRect.y + 26, testRect.x + absorbtion * 4.f, testRect.y + 29);
			if (showItems) absorbtionRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + absorbtion * 4.f, testRect.y + 41);
			vec4_t absorbtionSubRect = vec4_t(testRect.x + 39, testRect.y + 28, testRect.x + targetLen - 4.f, testRect.y + 30);
			if (showItems) absorbtionSubRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + targetLen - 4.f, testRect.y + 41);
			DrawUtils::fillRectangleA(absorbtionSubRect, MC_Color(25, 25, 25, 255));
			if (absorbtion > 1.f) {
				DrawUtils::drawRectangle(absorbtionRect, MC_Color(255, 230, 0), 0.3);
				DrawUtils::fillRectangleA(absorbtionRect, MC_Color(255, 230, 0, 255));
			}
			DrawUtils::drawSteve(vec4_t(positionX + 5, positionY + 5, positionX, positionY + 20));
		}
	}
}
