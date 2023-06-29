#include "Killsults.h"

using namespace std;
Killsults::Killsults() : IModule(0, Category::OTHER, "Insults people you kill lol") {
    registerEnumSetting("Mode", &mode, 0);
    mode.addEntry("Skiddle", 0);
    mode.addEntry("Dominant Lily", 1);
    mode.addEntry("Catgirl", 2);
    mode.addEntry("Anti Pride", 3);
    registerBoolSetting("Sound", &sound, sound);
    //mode.addEntry("Custom", 2);
}


const char* Killsults::getRawModuleName() {
    return "Killsults";
}

const char* Killsults::getModuleName() {
    if (mode.getSelectedValue() == 0) name = string("Killsults ") + string(GRAY) + string("Skiddle");
    if (mode.getSelectedValue() == 1) name = string("Killsults ") + string(GRAY) + string("DominantLily");
    if (mode.getSelectedValue() == 1) name = string("Killsults ") + string(GRAY) + string("Catgirl");
    if (mode.getSelectedValue() == 2) name = string("Killsults ") + string(GRAY) + string("AntiPride");
    return name.c_str();
}

string normalMessages[11] = {
    "Skiddle > Drizzle | Skiddle",
    "Would you rather a Japanese bunch or an English bunch of skids? | Skiddle",
    "Are you listening to my advice? Or are you still being a drizzle fanboy? | Skiddle",
    "Are you a drizzle user and want this client because drizzle sucks? Well sorry but you can't get it. | Skiddle",
    "This client's name is a more, accurate version of Drizzle, therefore it's better. | Skiddle",
    "Were you using a pingspoofer for that speed? Because Drizzle users commonly do that. | Skiddle",
    "Skiddle's bypasses don't silent flag, and Drizzle's do! | Skiddle",
    "Hello, Drizzle user! Oh, goodbye... | Skiddle",
    "Was DarkNBTHax forgetting how to paste the cause of your loss? | Skiddle",
    "bro i need your help polaris just got leaked",
    "[Skiddle] Detected a Drizzle user in this hvh! | Skiddle"
};

string lilyMessages[14] = {
    "mommy owns you | Sukumi",
    "i'm gonna fuck the shit out of you whore | Sukumi",
    "you're my dumb little pet | Sukumi",
    "i'm gonna rail you until you cant walk | Sukumi",
    "i'm gonna put a collar on you and write my name on it | Sukumi",
    "you're so pathetic | Sukumi",
    "you're mommys little whore | Sukumi",
    "you're my little fucktoy | Sukumi",
    "you are just an item for my pleasure | Sukumi",
    "dumb whore | Sukumi",
    "im gonna tie you up | Sukumi",
    "you can't do anything about it little pet | Sukumi",
    "you're my dumb slut | Sukumi",
    "bro i need your help polaris just got leaked"
};

string cheaterMessages[20] = {
    "meow :3",
    "you're so obviously a bottom",
    "aww, you're so cute",
    "catgirl powers, nya~",
    "you're such a cutie",
    "grr i bite",
    "i request headpats pwease uwu",
    "that's so gay but sweet~",
    "we should take estrogen together :3",
    "in need of a collar?",
    "i am a catgirl! meow!",
    "cute",
    "uwu",
    "you said you like cat girls right? well i am a cat girl now~ does that make you like me?",
    "i will bite you grrr",
    "you're so sweet awwww",
    "i am not a bottom wdym? :3",
    "bro i need your help polaris just got leaked",
    "buy me cat ears plz >.<",
    "userrank is a self paster >.<"
};

string sigmaMessages[2] = {
    "Eat My",
    "Funny Funny Abstractional"
};

string AntiPride[17] = {
    "Being Gay is gay",
    "Don't be gay like ZeroBacki be trans like Lilycat",
    "Pride month shouldn't exist",
    "Gay monkey man",
    "We here at Sakumi inc hate the gays",
    "The reason why you died? Because you support LGBTQ",
    "AntiBanCheat? More like AutoBanCheat #LAWL",
    "Average men liker",
    "PU^^^JP is definetly gay. We dont like him.",
    "Sub to _xNotToxic",
    "Sub to @HackingMCBE",
    "Sub to @enzoquidev",
    "Sub to Halalware",
    "Not to start any trouble, but lgbtq is kinda gay.",
    "bro i need your help polaris just got leaked",
    "Sub to lilycat"

};

void Killsults::onEnable() {
    killed = false;
}

void Killsults::onPlayerTick(C_Player* plr) {
    auto player = g_Data.getLocalPlayer();
    if (player == nullptr) return;

    int random = 0;
    srand(time(NULL));
    if (killed) {
        C_TextPacket textPacket;
        PointingStruct* level = player->pointingStruct;
        vec3_t* pos = player->getPos();
        if (sound) {
            level->playSound("random.orb", *pos, 1, 1);
            level->playSound("firework.blast", *pos, 1, 1);
        }
        switch (mode.getSelectedValue()) {
        case 0: // Skiddle
            random = rand() % 11;
            textPacket.message = TextHolder(lilyMessages[random]);
            break;
        case 1: // Dominant Lily
            random = rand() % 14;
            textPacket.message = TextHolder(normalMessages[random]);
            break;
        case 2: // Catgirl
            random = rand() % 20;
            textPacket.message = TextHolder(cheaterMessages[random]);
            break;
        case 3: //Anti Pride
            random = rand() % 17;
            textPacket.message = TextHolder(AntiPride[random]);
            break;
        }
        textPacket.sourceName.setText(player->getNameTag()->getText());
        textPacket.xboxUserId = to_string(player->getUserId());
        g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
        killed = false;
    }
}
