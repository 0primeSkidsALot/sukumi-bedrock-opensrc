#include "Speed.h"
#include "../pch.h"

bool testOnGround = false;
float speedMax = 0.69f; // inf value
float speedMin = 0.59f; // inf value
int packetsSent = 0;
int hivegroundticks = 0;
int enabledTicks = 0;
bool fullstop;
bool noslabs = false;
int damageMotion = 0;
int strafeTime = 0;
float random2 = 0.f;
float random3 = 0.f;
float fricspeed = 0.f;
//the j

using namespace std;
Speed::Speed() : IModule(0, Category::MOVEMENT, "Increases your speed") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla",0);
	mode.addEntry("Flareon",1);
	mode.addEntry("FlareonHop",2);
	mode.addEntry("FlareonLow",3);
	mode.addEntry("FlareonLow2",4);
	mode.addEntry("FlareonClip",5);
	mode.addEntry("Ground",6);
	mode.addEntry("NoGround",7);
	// Vanilla
	registerFloatSetting("Height", &height, height, 0.000001f, 0.40f);
	// All Settings
	registerFloatSetting("Speed", &speed, speed, 0.2f, 2.f);
	registerIntSetting("Timer", &timer, timer, 1, 69);

	// Friction
	registerFloatSetting("SpeedMax", &speedMax, speedMax, 0.f, 2.f);
	registerFloatSetting("SpeedMin", &speedMin, speedMin, 0.f, 2.f);
	registerFloatSetting("Duration", &duration, duration, 0.5f, 1.05f);
	//registerBoolSetting("Rotate", &rotate, rotate);
	registerBoolSetting("TimerBoost", &dboost, dboost);
	registerIntSetting("TimerMin", &timerMin, timerMin, 15.f, 50.f);
	registerIntSetting("TimerMax", &timerMax, timerMax, 15.f, 50.f);
	//registerBoolSetting("DamageBoost", &damageboost, damageboost);
	//registerIntSetting("DamageTime", &bypasstime, bypasstime, 1, 20);
	//registerIntSetting("DamageSpeed", &damagespeed, damagespeed, 0, 10); //only beta
	//registerIntSetting("DamageTimer", &dmgtimer, dmgtimer, 1, 40);
	//registerFloatSetting("Boostvalue", &boostValue, boostValue, 0.f, 10.f);
}

const char* Speed::getRawModuleName() {
	return "Speed";
}

const char* Speed::getModuleName() {
	name = string("Speed ") + string(GRAY) + mode.GetEntry(mode.getSelectedValue()).GetName();
	return name.c_str();
}

void Speed::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	enabledTicks = 0;
	damageMotion = 0;
	oldx = player->currentPos.x;
	oldz = player->currentPos.z;
	animYaw = player->yaw;
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Speed::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	player->stepHeight = 0.3f;//defaultnoslab

	if (enabledTicks > 10) enabledTicks++;
	else enabledTicks = 0;

	if (player->onGround) { groundTicks++; offGroundTicks = 0; }
	else { offGroundTicks++; groundTicks = 0; }

	//if (noslabs) player->stepHeight = 0.3f;

	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);

	// HiveLow
	switch (mode.getSelectedValue()) {
	case 3: // Cubecraft
		ticks++;
		break;
	}
	if (dboost) {
		if (player->onGround) {
			g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
			//moduleMgr->getModule<Blink>()->setEnabled(false);
			//auto notification = g_Data.addNotification("Speed:", "IKA HA BAKA"); notification->duration = 3;
		}

		if (GameData::isKeyDown(*input->spaceBarKey))
			g_Data.getClientInstance()->minecraft->setTimerSpeed(randomFloat(timerMin, timerMax));
		if (GameData::isKeyDown(*input->spaceBarKey)) {
			if (player->onGround) {
				if (GameData::isKeyDown(*input->spaceBarKey))
					g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
				//moduleMgr->getModule<Blink>()->setEnabled(false);
				//auto notification = g_Data.addNotification("Speed:", "INSANE BYPASS"); notification->duration = 3;
			}
			else
			{
				//moduleMgr->getModule<Blink>()->setEnabled(true);
			}
		}

		if (!GameData::isKeyDown(*input->spaceBarKey)) {
			if (GameData::isKeyDown(*input->spaceBarKey))
				g_Data.getClientInstance()->minecraft->setTimerSpeed(20);
		}
	}

	if (damageboost && damageMotion != 0) { //damageboost lol
		//MoveUtil::setSpeed(damageMotion + speed * (boostValue / 10));
	}
}



void Speed::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	bool pressed = MoveUtil::keyPressed();
	if (!pressed) MoveUtil::stop(false);
	player->setSprinting(true);

	if (mode.getSelectedValue() == 0) {
		static bool useVelocity = false;
		// eat my absctrionalie
		if (height >= 0.385) { if (player->onGround && pressed) player->jumpFromGround(); useVelocity = false; }
		else useVelocity = true;
		if (height <= 0.04 && !input->isJumping) { player->velocity.y += height; useVelocity = false; }

		if (player->onGround && useVelocity && pressed && !input->isJumping) player->velocity.y = height;
		MoveUtil::setSpeed(speed);
	}

	if (mode.getSelectedValue() == 1) {
		speedFriction *= 0.9280610828399658f;
		if (pressed) {
			if (player->onGround) {
				player->jumpFromGround();
				speedFriction = randomFloat(0.5285087823867798f, 0.49729517102241516f);
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}

	if (mode.getSelectedValue() == 2) {
		speedFriction *= 0.9300610828399658f, 91533857423598;
		if (pressed) {
			if (player->onGround) {
				player->jumpFromGround();
				g_Data.getClientInstance()->getMoveTurnInput()->isJumping;
				speedFriction = randomFloat(0.4985087823867798f, 0.45729517102241516f);
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}

	if (damageboost) {
		if (pressed) {
			if (player->onGround) {
				random3 = 0 - random2;
				fricspeed = randomFloat(random2, random3);
				MoveUtil::setSpeed(speed + fricspeed);
			}

			if (damageMotion != 0 && damageMotion >= 0.15f) {
				MoveUtil::setSpeed(damageMotion + speed * (damagespeed / 10));
				g_Data.getClientInstance()->minecraft->setTimerSpeed(dmgtimer);
				damageMotion = 0;
				strafeTime = 0;
			}

			if (MoveUtil::isMoving()) {
				if (strafeTime <= bypasstime) {
					strafeTime++;
					MoveUtil::setSpeed(player->velocity.magnitudexz());
				}
				else
				{
					g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
				}
			}
		}
	}

	if (mode.getSelectedValue() == 4) {
		speedFriction *= 0.7400610828399658f;
		if (pressed) {
			if (player->onGround) {
				player->jumpFromGround();
				speedFriction = randomFloat(0.505087823867798f, 0.612729517102241516f);
				player->fallDistance;
				g_Data.getClientInstance()->minecraft->setTimerSpeed(18.f);
				player->velocity.y = 0.2;
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}






	//glizzyspeed/tpish kinda shitty idk
	if (mode.getSelectedValue() == 5) {
		speedFriction *= 0.9400610828399658f;
		if (pressed) {
			if (player->onGround) {
				player->jumpFromGround();
				g_Data.getClientInstance()->minecraft->setTimerSpeed(45.f);
				speedFriction = randomFloat(0.5085087823867798f, 0.47729517102241516f);
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}



	if (mode.getSelectedValue() == 7) {
		static bool useVelocity = false;
		flareonticks++;
		// eat my absctrionalie
		if (height >= 0.385) { if (player->onGround && pressed) { player->jumpFromGround(); useVelocity = false; } }
		else useVelocity = true;
		if (height <= 0.04 && !input->isJumping) { player->velocity.y += height; useVelocity = false; }

		if (flareonticks >= 10 && speedFriction >= 0.4) {
			speedFriction = 0.2;
		}
		else speedFriction *= duration;
		if (pressed) {
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = height;
				random3 = 0 - random2;
				fricspeed = randomFloat(random2, random3);
				speedFriction = speed + fricspeed;
				MoveUtil::stop(false);
				flareonticks = 0;
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}

	if (mode.getSelectedValue() == 6) {
		static bool useVelocity = false;
		// eat my absctrionalie
		if (height >= 0.385) {
			if (player->onGround && pressed) {
				//player->setRot(vec2_t(yaw, player->pitch));
				player->jumpFromGround();
				useVelocity = false;
			}
		}
		else useVelocity = true;
		if (height <= 0.04 && !input->isJumping) {
			//player->setRot(vec2_t(yaw, player->pitch));
			player->jumpFromGround();
			player->velocity.y += height;
			useVelocity = false;
		}

		if (speedFriction > 0.25) speedFriction *= duration;
		if (pressed) {
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = height;
				random3 = 0 - random2;
				fricspeed = randomFloat(random2, random3);
				speedFriction = speed + fricspeed;
			}
			else MoveUtil::setSpeed(speedFriction);
		}
	}

	// Credits to Dort for this
	if (mode.getSelectedValue() == 3) {
		float calcYaw = (player->yaw + 90) * (PI / 180);
		float c = cos(calcYaw);
		float s = sin(calcYaw);

		vec2_t moveVec2D = { input->forwardMovement, -input->sideMovement };
		moveVec2D = { moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c };
		vec3_t moveVec;

		if (!pressed)
			MoveUtil::stop(false);

		static bool useVelocity = false;
		if (0.5000000059604645 >= 0.285) {
			if (player->onGround && pressed) player->jumpFromGround();
			useVelocity = false;
		}
		else
			useVelocity = true;

		if (0.5000000059604645 <= 0.03 && !input->isJumping) {
			player->velocity.y += 0.5000000059604645;
			useVelocity = false;
		}

		speedFriction *= 0.9346240322113025;
		if (pressed) {
			player->setSprinting(true);
			if (player->onGround) {
				if (useVelocity && !input->isJumping) player->velocity.y = 0.5000000059604645;
				speedFriction = randomFloat(0.409079251289367, 0.490846795992851257);
			}
			else
				MoveUtil::setSpeed(speedFriction);
			if (player->onGround) {
				moveVec.y = player->velocity.y = 0.35f;
			}
		}
	}
}





void Speed::onSendPacket(C_Packet* packet) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || input == nullptr) return;
	//if (!rotate) return;
//	movePacket->yaw = animYaw;
	//movePacket->headYaw = animYaw;
}

void Speed::onDisable() {
	g_Data.getLocalPlayer()->stepHeight = 0.7f;
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	MoveUtil::stop(false);
	preventKick = false;
	packetsSent = 0;
}
