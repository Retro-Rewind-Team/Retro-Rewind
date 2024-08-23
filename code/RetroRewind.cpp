#include <MarioKartWii/Race/RaceData.hpp>
#include <PulsarEngine/SlotExpansion/CupsConfig.hpp>
#include <PulsarEngine/Settings/UI/SettingsPanel.hpp>
#include <PulsarEngine/Settings/Settings.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <RetroRewind.hpp>

namespace RetroRewind {
Pulsar::System *System::Create() {
    return new System(); //now Pulsar::sInstance is of type RetroRewind
}
Pulsar::System::Inherit CreateRetroRewind(System::Create); //Create a Inherit that'll get called back by Pulsar::CreatePulsar

void System::AfterInit(){
    ++Pulsar::UI::SettingsPanel::pageCount;
    ++Pulsar::UI::SettingsPanel::pageCount;

    Pulsar::UI::SettingsPanel::radioButtonCount[SETTINGSTYPE_RR]=4;
    Pulsar::UI::SettingsPanel::scrollerCount[SETTINGSTYPE_RR]=1;
    Pulsar::UI::SettingsPanel::radioButtonCount[SETTINGSTYPE_RR2]=5;

    //Transmission
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR][0]=4;

    //Impossible CPU's
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR][1]=2;

    //Kart Restrictions
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR][2]=3;

    //Character Restrictions
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR][3]=4;

    //Gamemode
    Pulsar::UI::SettingsPanel::optionsPerPagePerScroller[SETTINGSTYPE_RR][0]=4;
    
    //Custom Music
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][0]=2;
    
    //Result Display
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][1]=2;
    
    //Brake Drifting
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][2]=2;

    //Friend Room TC Type
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][3]=2;

    //Force No Brake Drift
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][4]=2;
}


bool System::Is500cc() {
    return !Pulsar::CupsConfig::IsRegsSituation() && RaceData::sInstance->racesScenario.settings.engineClass == CC_50;
}

    System::CharacterRestriction System::GetCharacterRestriction(){
    const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
    const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
    if (isFroom){
        return GetsInstance()->charRestrictMode;
    }
    return CHAR_DEFAULTSELECTION;
    }

    System::KartRestriction System::GetKartRestriction(){
    const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
    const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
    if (isFroom){
        return GetsInstance()->kartRestrictMode;
    }
    return KART_DEFAULTSELECTION;
    }

    System::ForceBrakeDrift System::GetBrakeDrift(){
    const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
    const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
    if (isFroom){
        return GetsInstance()->brakeDriftMode;
    }
    return FORCEBRAKEDRIFT_DISABLED;
    }

    System::Gamemode System::GetGameMode(){
        const bool isRegs = Pulsar::CupsConfig::IsRegsSituation();
        const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
        const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
        const bool isRegional = gameMode == MODE_PUBLIC_VS || gameMode == MODE_PUBLIC_BATTLE;
        if (!isRegs){
            if (isFroom){
                return GAMEMODE_DEFAULT;
            }
            else if (isRegional){
                return GAMEMODE_DEFAULT;
            }
            return static_cast<Gamemode>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), SETTINGRR_SCROLLER_GAMEMODES));
        }
        return GAMEMODE_NONE;
    }

    System::TC System::GetTCMode(){
        const bool isRegs = Pulsar::CupsConfig::IsRegsSituation();
        const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
        const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
        const bool isRegional = gameMode == MODE_PUBLIC_VS || gameMode == MODE_PUBLIC_BATTLE;
        if (!isRegs){
            if (isFroom){
                return GetsInstance()->tcMode;
            }
            else if (isRegional){
                return TC_MEGA;
            }
            return static_cast<TC>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR2), SETTINGRR2_RADIO_TC));
        }
        return TC_MEGA;
    }

System::WeightClass System::GetWeightClass(const CharacterId id){
    switch (id)
    {
        case BABY_MARIO:
        case BABY_LUIGI:
        case BABY_PEACH:
        case BABY_DAISY:
        case TOAD:
        case TOADETTE:
        case KOOPA_TROOPA:
        case DRY_BONES:
            return LIGHTWEIGHT;
        case MARIO:
        case LUIGI:
        case PEACH:
        case DAISY:
        case YOSHI:
        case BIRDO:
        case DIDDY_KONG:
        case BOWSER_JR:
            return MEDIUMWEIGHT;
        case WARIO:
        case WALUIGI:
        case DONKEY_KONG:
        case BOWSER:
        case KING_BOO:
        case ROSALINA:
        case FUNKY_KONG:
        case DRY_BOWSER:
            return HEAVYWEIGHT;
        default:
            return MIIS;
    }
}

//Battle codes
//Allow all vehicles in battle
kmWrite32(0x805DE7B4, 0x38000000);
kmWrite32(0x80553F98, 0x3880000A);
kmWrite32(0x8084FEF0, 0x48000044);
kmWrite32(0x80860A90, 0x38600000);

//Visual codes
//HUD Color [Spaghetti Noppers]
kmWrite32(0x80895CC0, 0x00FF00FD);
kmWrite32(0x80895CC4, 0x00FF00FD);
kmWrite32(0x80895CC8, 0x00FF00FD);
kmWrite32(0x80895CCC, 0x00FF00FD);
kmWrite32(0x80895CD0, 0x00FF00FD);
kmWrite32(0x80895CD4, 0x00FF0046);
kmWrite32(0x80895CD8, 0x00FF00FF);
kmWrite32(0x80895CDC, 0x00FF00FF);
kmWrite32(0x80895CE0, 0x00FF00FF);
kmWrite32(0x80895CE4, 0x00FF00FF);
kmWrite32(0x80895CE8, 0x00FF00FF);
kmWrite32(0x80895CEC, 0x00FF0046);

//No Sun Filter [Anarion]
kmWrite8(0x8025739F, 0x00);
kmWrite8(0x80256F7F, 0x00);
kmWrite8(0x802575DF, 0x00);
kmWrite8(0x802572BF, 0x00);

//Remove Background Blur [Davidevgen]
kmWrite32(0x80258184, 0x30);
kmWrite32(0x80257E64, 0x30);
kmWrite32(0x80257B24, 0x30);
kmWrite32(0x80257F44, 0x30);

//Online codes
//Instant Voting Roulette Decide [Ro]
kmWrite32(0x80643BC4, 0x60000000);
kmWrite32(0x80643C2C, 0x60000000);

//No Disconnect on Countdown [_tZ]
kmWrite32(0x80655578, 0x60000000);

//Change VR Limit [XeR]
kmWrite16(0x8052D286, 0x00007530);
kmWrite16(0x8052D28E, 0x00007530);
kmWrite16(0x8064F6DA, 0x00007530);
kmWrite16(0x8064F6E6, 0x00007530);
kmWrite16(0x8085654E, 0x00007530);
kmWrite16(0x80856556, 0x00007530);
kmWrite16(0x8085C23E, 0x00007530);
kmWrite16(0x8085C246, 0x00007530);
kmWrite16(0x8064F76A, 0x00007530);
kmWrite16(0x8064F776, 0x00007530);
kmWrite16(0x808565BA, 0x00007530);
kmWrite16(0x808565C2, 0x00007530);
kmWrite16(0x8085C322, 0x00007530);
kmWrite16(0x8085C32A, 0x00007530);

//Don't Lose VR While Disconnecting [Bully]
kmWrite32(0x80856560, 0x60000000);

//Mushroom Glitch Fix [Leseratte]
kmWrite8(0x807BA077, 0x00000000);

//Remove WW Button [Chadderz]
kmWrite16(0x8064B982, 0x00000005);
kmWrite32(0x8064BA10, 0x60000000);
kmWrite32(0x8064BA38, 0x60000000);
kmWrite32(0x8064BA50, 0x60000000);
kmWrite32(0x8064BA5C, 0x60000000);
kmWrite16(0x8064BC12, 0x00000001);
kmWrite16(0x8064BC3E, 0x00000484);
kmWrite16(0x8064BC4E, 0x000010D7);
kmWrite16(0x8064BCB6, 0x00000484);
kmWrite16(0x8064BCC2, 0x000010D7);

//Allow WFC on Wiimmfi Patched ISOs
kmWrite32(0x800EE3A0, 0x2C030000);
kmWrite32(0x800ECAAC, 0x7C7E1B78);

//Ultra Uncut [MrBean35000vr + Chadderz]
asmFunc GetUltraUncut() {
    ASM(
        nofralloc;
loc_0x0:
  lbz       r3, 0x1C(r29);
  cmplwi    r3, 0x1;
  ble+      loc_0x10;
  mr        r0, r30;

loc_0x10:
  cmplw     r30, r0;
    )
}
kmCall(0x8053511C, GetUltraUncut);

//VR System Changes [MrBean35000vr]
//Multiply VR difference by 2 [Winner]
asmFunc GetVRScaleWin() {
    ASM(
    li r5, 2;
    divw r3, r3, r5;
    extsh r3, r3;
    )
}
kmCall(0x8052D150, GetVRScaleWin);

//Cap VR loss from one victorious opponent between 0 and -8.
asmFunc GetCapVRLoss() {
    ASM(
  lwz       r3, 0x14(r1);
  cmpwi     r3, -8;
  bge       0f;
  li        r3, -8;
  b         1f;
  0:;
  cmpwi     r3, 0;
  ble       1f;
  li        r3, 0;
  1:;
    )
}
kmCall(0x8052D260, GetCapVRLoss);

//Cap VR gain from one defeated opponent between 2 and 12.
asmFunc GetCapVRGain() {
    ASM(
  lwz       r3, 0x14(r1);
  cmpwi     r3, 2;
  bge       0f;
  li        r3, 2;
  b         1f;
  0:;
  cmpwi     r3, 12;
  ble       1f;
  li        r3, 12;
  1:;
    )
}
kmCall(0x8052D1B0, GetCapVRGain);

//Code by Ismy and CLF78.
asmFunc GetHybridDrift1() {
    ASM(
lwz r0, 0x14(r3);
lwz r12, 0(r28);
lwz r12, 0(r12);
lwz r12, 0x14(r12);
lwz r12, 0(r12);
lwz r12, 0x4(r12);
cmpwi r12, 2;
beqlr;
li r0, 0;
blr;
    )
}
kmCall(0x8057930C, GetHybridDrift1);
kmCall(0x80578DCC, GetHybridDrift1);

asmFunc GetHybridDrift3() {
    ASM(
lwz r0, 0x14(r3);
rlwinm. r12, r0, 0, 18, 18;
beq end2;
ori r0, r0, 0x10;
stw r0, 0x14(r3);
end2:
lwz r0, 0x4(r3);
blr;
    )
}
kmCall(0x8057DFA8, GetHybridDrift3);

asmFunc GetHybridDrift4() {
    ASM(
lwz r0, 0x14(r4);
rlwinm. r12, r0, 0, 18, 18;
beq end2;
ori r0, r0, 0x10;
stw r0, 0x14(r4);
end2:
lwz r0, 0x4(r4);
blr;
    )
}
kmCall(0x8057E018, GetHybridDrift4);

asmFunc GetHybridDrift6() {
    ASM(
  rlwinm.   r0,r0,0,27,27;
  li        r0, 0;
  stw       r0, 0x1C8(r3);
    )
}
kmCall(0x8057E108, GetHybridDrift6);

asmFunc GetHybridDrift7() {
    ASM(
lwz r3, 0x4(r30);
andi. r4, r3, 0x84;
beq end;
lwz r4, 0x14(r30);
rlwinm r4, r4, 0, 28, 26;
stw r4, 0x14(r30);
rlwinm r3, r3, 0, 4, 2;
stw r3, 0x4(r30);
end:
mr r3, r30;
blr;
    )
}
kmCall(0x80594AA8, GetHybridDrift7);

kmWrite8(0x8059450E, 0x00000020);
kmWrite32(0x80594A60, 0x60000000);
kmWrite32(0x805A35BC, 0x38600000);
kmWrite16(0x80745AB0, 0x00004800);
kmWrite32(0x808CB70A, 0x00000000);

//Mii Outfit C Anti-Crash
kmWrite8(0x8089089D, 0x00000062);
kmWrite8(0x808908A9, 0x00000062);
kmWrite8(0x808908E5, 0x00000062);
kmWrite8(0x808908F1, 0x00000062);
kmWrite8(0x8089092D, 0x00000062);
kmWrite8(0x80890939, 0x00000062);

//Anti Online Item Delimiters [Ro]
asmFunc GetItemDelimiterShock() {
    ASM(
        nofralloc;
loc_0x0:
  mflr r12;
  cmpwi     r7, 0x1;
  bne+ validLightning;
  addi r12, r12, 0x12C;
  mtlr r12;
  blr;
validLightning:
  mulli r29, r3, 0xF0;
  blr;
    )
}
kmCall(0x807B7C34, GetItemDelimiterShock);

asmFunc GetItemDelimiterBlooper() {
    ASM(
        nofralloc;
loc_0x0:
  mflr r12;
  cmpwi     r7, 0x1;
  bne+ validBlooper;
  addi r12, r12, 0x1A8;
  mtlr r12;
  blr;
validBlooper:
  addi r11, r1, 0x50;
  blr;
    )
}
kmCall(0x807A81C0, GetItemDelimiterBlooper);

asmFunc GetItemDelimiterPOW() {
    ASM(
        nofralloc;
loc_0x0:
mflr r12;
  cmpwi     r7, 0x1;
  bne+ validPOW;
  addi r12, r12, 0x48;
  mtlr r12;
  blr;
validPOW:
  mr r30, r3;
  blr;
    )
}
kmCall(0x807B1B44, GetItemDelimiterPOW);

//Anti Mii Crash
asmFunc AntiWiper() {
    ASM(
        nofralloc;
loc_0x0:
  cmpwi r4, 0x6;
  ble validMii;
  lhz r12, 0xE(r30);
  cmpwi r12, 0x0;
  bne validMii;
  li r31, 0x0;
  li r4, 0x6;
validMii:
  mr r29, r4;
  blr;
    )
}
kmCall(0x800CB6C0, AntiWiper);
kmWrite32(0x80526660, 0x38000001); //Credits to Ro for the last line.

//Anti Item Collission Crash [Marioiscool246]
extern "C" void __ptmf_test(void*);
asmFunc AntiItemColCrash() {
    ASM(
        nofralloc;

    loc_0x0:
        stwu r1, -0xC(r1);
        stw r31, 8(r1);
        mflr r31;

        addi r3, r29, 0x174;
        bl __ptmf_test;

        cmpwi r3, 0;
        bne end;

        addi r31, r31, 0x14;

    end:
        mtlr r31;
        lwz r31, 8(r1);
        addi r1, r1, 0xC;
        mr r3, r29;
        blr;
    )
}
kmCall(0x807A1A54, AntiItemColCrash);

////Online TT Codes
////TT Start Position
//kmWrite32(0x80536304, 0x38000002);
//
////Start with Triple Shrooms
//kmWrite32(0x807997D8, 0x38000001);
//
////No Kart/Bike Collision
//kmWrite32(0x8056F874, 0x48000008);
//
////No Item Boxes
//kmWrite32(0x8082A4DC, 0x48000010);
//
////Disable Slip Stream
//kmWrite32(0x80587000, 0x2C030001);
//
////Item Vanish
//kmWrite32(0x8079F744, 0x2C00000A);
//kmWrite32(0x8079F748, 0xFD810040);

} // namespace RetroRewind