#pragma once
#include ".././../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Animations : public IModule {
private:
    bool reset = false;
    bool swing = true;
    bool useswing = false;

public:
    bool test = false;
    bool shouldBlock = false;
    bool isAttacking = false;
    bool smoothSwing = false;
    bool cswing = false;
    bool smswing = false;
    bool sswing = false;
    bool clean = false;
    int SwingSpeed = 106;
    int resetswing = 0;
    float tests = 0.f;
    int hiveSpeedIndex = 0;
    int renderTimer = 6;
    float s = 0.f;
    float xo = 0.f;
    float RotatePosition = 0.f;
    // Unused
    bool translate = true;
    bool rotate = false;
    float xRotate = 0.f, yRotate = 0.f, zRotate = 0.f;

    bool scale = true;
    float xScale = 1.f, yScale = 1.f, zScale = 1.f;
    float xPos = 0.f, yPos = 0.f, zPos = 0.f;

    SettingEnum mode = this;

    virtual void onPlayerTick(C_Player* plr);
    virtual const char* getModuleName();
    virtual void onTick(C_GameMode* gm);
    virtual void onSendPacket(C_Packet* packet) override;
    virtual void onDisable();
    virtual void onEnable();
    Animations();
};
