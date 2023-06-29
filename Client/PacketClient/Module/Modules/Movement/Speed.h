#pragma once
#include "../../../../Utils/HMath.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Speed : public IModule {
private:
	int offGroundTicks = 0;
	int groundTicks = 0;
	int timerMin = 35;
	int timerMax = 35;
	int bypasstime = 7;
public:
	float effectiveSpeed = 0.5f;
	float duration = 1.f;
	float height = 0.40f;
	float speed = 0.5f;
	float oldx;
	float oldz;
	float animYaw = 0.0;
	float boostValue = 7;
	int damagespeed = 2;
	int dmgtimer = 20;
	int timer = 20;
	int ticks = 0;
	int flareonticks = 0;
	bool dboost = true;
	bool damageboost = true;
	bool rotate;
	bool fullstop;
	// Hive
	float speedFriction = 0.65f;
	float maxSpeed = 0.98883f;
	float maxSpeed2 = 0.88991f;
	bool preventKick = false;

	std::string name = "Speed";
	SettingEnum mode = this;

	inline std::vector<PlayerAuthInputPacket*>* getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };
	inline std::vector<C_MovePlayerPacket*>* getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	std::vector<PlayerAuthInputPacket*> PlayerAuthInputPacketHolder;
	std::vector<C_MovePlayerPacket*> MovePlayerPacketHolder;
	virtual void onMove(C_MoveInputHandler* input);
	virtual void onSendPacket(C_Packet* packet);
	virtual const char* getRawModuleName();
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	Speed();
};