#include "Animations.h"
#include "../../../../Memory/Hooks.h"


void* SmoothSwing = (void*)FindSignature("0F 84 ? ? ? ? 48 8B 56 ? 48 85 D2 74 ? 48 8B 02");
void* TapAddress = (void*)FindSignature("F3 0F 51 F0 0F 28 C8");

using namespace std;
Animations::Animations() : IModule(0, Category::VISUAL, "Changes the swing/hitting animation") {
	registerBoolSetting("FluxSwing", &smoothSwing, smoothSwing);
	registerBoolSetting("TapSwing", &sswing, sswing);
	registerBoolSetting("SlowSwing", &clean, clean);
	registerIntSetting("swingspeed", &SwingSpeed, SwingSpeed, 1, 270);
	//registerIntSetting("ResetSwing", &resetswing, resetswing, 0, 270);
	mode.addEntry(EnumEntry("1.7", 0))
		.addEntry(EnumEntry("Slide", 1));
	registerEnumSetting("BlockMode", &mode, 0);
	registerFloatSetting("X", &xPos, xPos, -3.f, 3.f);
	registerFloatSetting("Y", &yPos, yPos, -3.f, 3.f);
	registerFloatSetting("Z", &zPos, zPos, -3.f, 3.f);
}

const char* Animations::getModuleName() {
	return "Animations";
}

void Animations::onEnable() {
}

void Animations::onTick(C_GameMode* gm) {
	auto killaura = moduleMgr->getModule<Killaura>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (smoothSwing || !smoothSwing && !sswing && shouldBlock)	Utils::nopBytes((unsigned char*)SmoothSwing, 6);
	else Utils::patchBytes((unsigned char*)((uintptr_t)SmoothSwing), (unsigned char*)"\x0F\x84\x95\x02\x00\x00", 6);
	if (sswing)
		Utils::nopBytes((BYTE*)TapAddress, 4); else
		Utils::patchBytes((BYTE*)((uintptr_t)TapAddress), (BYTE*)"\xF3\x0F\x51\xF0", 4);

	auto slot = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
	shouldBlock = slot != nullptr && slot->item != nullptr && slot->getItem()->isWeapon() && g_Data.isRightClickDown() || slot != nullptr && slot->item != nullptr && slot->getItem()->isWeapon() && !killaura->targetListEmpty && killaura->isEnabled();
	isAttacking = g_Data.isLeftClickDown() || !killaura->targetListEmpty && killaura->isEnabled();
}

float swingSpeed = 0.f;

void Animations::onPlayerTick(C_Player* plr) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (clean) {
		if (hiveSpeedIndex >= resetswing) {
			useswing = false;
			hiveSpeedIndex = 0;
		}
		resetswing = SwingSpeed;
		float SwingArray[500];
		for (int i = 0; i < resetswing; i++) {
			SwingArray[i] = i * 0.01;
		}
		if (useswing && clean || isAttacking) {
			float SwingSpeedArray = SwingArray[hiveSpeedIndex++ % SwingSpeed];
			{
				float* speedAdr = reinterpret_cast<float*>(reinterpret_cast<__int64>(player) + 0x79C);
				*speedAdr = SwingSpeedArray;
			}
		}
	}

	if (swingSpeed >= 2005) swingSpeed = 340;
	else swingSpeed += 1.f;
	if (swingSpeed <= 339) swingSpeed = 340;

	if (isAttacking && shouldBlock && mode.getSelectedValue() == 1 && !g_Hooks.isThirdPerson) { float* speedAdr = reinterpret_cast<float*>(reinterpret_cast<__int64>(player) + 0x79C); *speedAdr = swingSpeed; }
}

void Animations::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_AnimatePacket>()) {
		useswing = true;
	}
}

void Animations::onDisable() {
	Utils::patchBytes((BYTE*)((uintptr_t)TapAddress), (BYTE*)"\xF3\x0F\x51\xF0", 4);
	Utils::patchBytes((BYTE*)((uintptr_t)SmoothSwing), (BYTE*)"\x0F\x84\x95\x02\x00\x00", 6);
}
