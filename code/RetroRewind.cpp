#include <MarioKartWii/Race/RaceData.hpp>
#include <PulsarEngine/SlotExpansion/CupsConfig.hpp>
#include <PulsarEngine/Settings/UI/SettingsPanel.hpp>
#include <PulsarEngine/Settings/Settings.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <Network/WiiLink.hpp>
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
    Pulsar::UI::SettingsPanel::radioButtonCount[SETTINGSTYPE_RR2]=4;

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

    //30 FPS
    Pulsar::UI::SettingsPanel::buttonsPerPagePerRow[SETTINGSTYPE_RR2][3]=2;
}


bool System::Is500cc() {
    return RaceData::sInstance->racesScenario.settings.engineClass == CC_50;
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
        return FORCEBRAKEDRIFT_DISABLED;
    }
    return FORCEBRAKEDRIFT_DISABLED;
    }

    HostSettingHostCC System::GetCCMode(){
    const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
    const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
    if (isFroom){
        return GetsInstance()->ccMode;
    }
    return HOSTSETTING_CC_NORMAL;
    }

    System::Gamemode System::GetGameMode(){
        const bool isRegs = Pulsar::CupsConfig::IsRegsSituation();
        const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
        const bool isFroom = gameMode == MODE_PRIVATE_VS;
        const bool isFroomBattle = gameMode == MODE_PRIVATE_BATTLE;
        const bool isTTs = gameMode == MODE_TIME_TRIAL;
        const bool isVS = gameMode == MODE_VS_RACE;
        const bool isOfflineBattle = gameMode == MODE_BATTLE;
        const bool isRegional = gameMode == MODE_PUBLIC_VS || gameMode == MODE_PUBLIC_BATTLE;
            if (isFroom){
                return GetsInstance()->hostMode;
            }
            else if (isRegs){
                return GAMEMODE_ONLINETT;
            }
            else if (isRegional || isFroomBattle || isOfflineBattle){
                return GAMEMODE_DEFAULT;
            }
            else if (isTTs){
                return GAMEMODE_ONLINETT;
            }
            else if (isVS){
                return static_cast<Gamemode>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), SETTINGRR_SCROLLER_GAMEMODES));
            }
            return GAMEMODE_DEFAULT;
    }

    System::TC System::GetTCMode(){
        const bool isRegs = Pulsar::CupsConfig::IsRegsSituation();
        const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
        const bool isFroom = gameMode == MODE_PRIVATE_VS || gameMode == MODE_PRIVATE_BATTLE;
        const bool isRegional = gameMode == MODE_PUBLIC_VS || gameMode == MODE_PUBLIC_BATTLE;
        if (!isRegs){
            if (isFroom){
                return TC_MEGA;
            }
            else if (isRegional){
                return TC_MEGA;
            }
            return TC_MEGA;
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

 void TTsOnline() {
    const GameMode wwMode = RaceData::sInstance->menusScenario.settings.gamemode;
    const System::Gamemode gameMode = System::GetGameMode();
    const bool isBattle = wwMode == MODE_PUBLIC_BATTLE || wwMode == MODE_BATTLE || wwMode == MODE_PRIVATE_BATTLE;
    OnlineTTHook = 0x00;
    if (gameMode == System::GAMEMODE_ONLINETT && !isBattle) {
        OnlineTTHook = 0x00FF0100;
    }
    Pulsar::System::CacheInvalidateAddress(OnlineTTHook);
 }
static PageLoadHook PatchOnlineTT(TTsOnline);

 void BattleStartPos() {
    const GameMode gameMode = RaceData::sInstance->menusScenario.settings.gamemode;
    BattleStartHook = 0x00;
    if (gameMode == MODE_BATTLE || gameMode == MODE_PRIVATE_BATTLE || gameMode == MODE_PUBLIC_BATTLE) {
        BattleStartHook = 0x00FF0100;
    }
    Pulsar::System::CacheInvalidateAddress(BattleStartHook);
 }
static PageLoadHook PatchBattleStartPos(BattleStartPos);

 void FPSPatch() {
    FPSPatchHook = 0x00;
    if (static_cast<RetroRewind::System::FPS>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR2), RetroRewind::System::SETTIGNRR2_RADIO_FPS)) == RetroRewind::System::FPS_HALF) {
        FPSPatchHook = 0x00FF0100;
    }
    Pulsar::System::CacheInvalidateAddress(FPSPatchHook);
 }
static PageLoadHook PatchFPS(FPSPatch);

//Simple Cheat code crash [Cats4Life]
void CodeCrash() {
    if(DolphinCheat == 0x00000001) Pulsar::Debug::FatalError("Please disable all cheat codes.");
    else if(MainDolCheat == 0x9421FF58) Pulsar::Debug::FatalError("Report this crash to ZPL's Rewind Hideaway.");
}
static PageLoadHook CRASH_HOOK(CodeCrash);
} // namespace RetroRewind

namespace Visual {

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

//Force 30 FPS [Vabold]
kmWrite32(0x80554224, 0x3C808000);
kmWrite32(0x80554228, 0x88841204);
kmWrite32(0x8055422C, 0x48000044);

} // namespace Visual

namespace AntiCrash {

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

}

namespace HybridDrift {

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

}

namespace Online {

//Allow All Vehicles in Battle Mode [Nameless, Scruffy]
kmWrite32(0x80553F98, 0x3880000A);
kmWrite32(0x8084FEF0, 0x48000044);
kmWrite32(0x80860A90, 0x38600000);

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

}

namespace TTOnline {

//Online TT Codes [Melg and co.]
asmFunc GetTTCycle0() {
    ASM(
        nofralloc;
  li  r0, 0x2;
  blr;
    )
}
kmCall(0x80554b68, GetTTCycle0);

asmFunc GetTTCycle1() {
    ASM(
        nofralloc;
loc_0x0:
  lis       r11, 0x8000;
  li        r0, 0x1;
  stb       r0, 0x11FC(r11);
  lhz       r0, 0x6C(r29);

end:
  blr;
    )
}
kmCall(0x805A7414, GetTTCycle1);

asmFunc GetTTCycle2() {
    ASM(
        nofralloc;
loc_0x0:
  lis       r11, 0x8000;
  li        r0, 0;
  stb       r0, 0x11FC(r11);
  li        r0, 0x1;

end:
  blr;
    )
}
kmCall(0x805334B0, GetTTCycle2);

extern "C" void Update__10ObjectsMgrFv(void*);
asmFunc GetTTCycle3() {
    ASM(
        nofralloc;
loc_0x0:
  lis       r12, 0x8000;
  lbz       r12, 0x11FC(r12);
  cmpwi     r12, 0;
  beq+      loc_0x14;
  blr;    

loc_0x14:
  stwu      r1, -0x80(r1);
  lis       r12, Update__10ObjectsMgrFv@ha;
  addi      r12, r12, Update__10ObjectsMgrFv@l;
  addi      r12, r12, 0x4;
  mtctr     r12;
  bctr;
    )
}
kmBranch(0x8082A8F4, GetTTCycle3);

asmFunc TTStartPosition() {
    ASM(
        nofralloc;
loc_0x0:
  li  r0, 0x0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq check_battle;
  li  r0, 0x2;
  b end;

check_battle:
  lis r12, 0x8000;
  lbz r12, 0x1208(r12);
  cmpwi r12, 0;
  beq end;
  li  r0, 0x4;

end:
  blr;
    )
}
kmCall(0x80536304, TTStartPosition);

asmFunc TripleShrooms() {
    ASM(
        nofralloc;
loc_0x0:
  li  r0, 0x0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq end;
  li  r0, 0x1;

end:
  blr;
    )
}
kmCall(0x807997D8, TripleShrooms);

asmFunc NoCollision() {
    ASM(
        nofralloc;
loc_0x0:
  li r6, 0x0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq end;
  li r6, 0x1;

end:
  blr;
    )
}
kmCall(0x8056F85C, NoCollision);

asmFunc NoItems() {
    ASM(
        nofralloc;
loc_0x0:
  li r0, 0x0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq end;
  li r0, 0x2;

end:
  blr;
    )
}
kmCall(0x8082A4D4, NoItems);

asmFunc NoSlipStream() {
    ASM(
        nofralloc;
loc_0x0:
  cmpwi  r3, 0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi cr7, r12, 0;
  beq cr7, end;
  crclr 2;

end:
  blr;
    )
}
kmCall(0x80587000, NoSlipStream);

asmFunc ItemVanish() {
    ASM(
        nofralloc;
loc_0x0:
  lfs f0, 0x0(r31);
  fcmpo cr0, f1, f0;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi cr7, r12, 0;
  beqlr cr7;
  cmpwi r0, 0xA;

end:
  blr;
    )
}
kmCall(0x8079F748, ItemVanish);

asmFunc GhostRacer1() {
    ASM(
        nofralloc;
loc_0x0:
  lwz r0, 0x0B70 (r5);
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq end;
  li r0, 0x2;

end:
  blr;
    )
}
kmCall(0x805B15C0, GhostRacer1);

extern "C" void sInstance__8RaceData(void*);
asmFunc GhostRacer2() {
    ASM(
        nofralloc;
loc_0x0:
  lis r7, sInstance__8RaceData@ha;
  lis r12, 0x8000;
  lbz r12, 0x1202(r12);
  cmpwi r12, 0;
  beq end;
  lis r7, sInstance__8RaceData@ha;
  lwz r12, sInstance__8RaceData@l(r7);
  add r12, r12, r0;
  lwz r12, 56(r12);
  cmpwi r12, 0x0;
  beq- loc_0x1C;
  li r5, 0x1;

loc_0x1C:
  blr;
    
end:
  blr;
    )
}
kmCall(0x80576B58, GhostRacer2);

extern "C" void sInstance__Q25Input7Manager(void*);
asmFunc ComboSwitch() {
  ASM(
      nofralloc;
loc_0x0:
  lis       r12, 0x8000;
  lbz       r12, 0x1202(r12);
  cmpwi     r12, 0;
  beq       original;
  lis       r3, sInstance__Q25Input7Manager@ha;
  lis       r12, 0x8150;
  lwz       r5, 0x1634(r12);
  cmpwi     r5, 0;
  beq-      loc_0x9C;
  lwz       r5, 0x16BC(r5);
  cmpwi     r5, 0x78;
  bge+      loc_0x2C;
  li        r11, 0x1;
  stb       r11, 0x162F(r12);
  b         loc_0x9C;

loc_0x2C:
  lhz       r7, 0x1C(r28);
  cmpwi     r5, 0x177;
  blt+      loc_0x50;
  lis       r11, 0x8000;
  lbz       r5, 0x1634(r11);
  cmpwi     r5, 0x1;
  beq-      loc_0x50;
  lbz       r5, 0x3C(r28);
  li        r6, 0;

loc_0x50:
  lwz       r6, sInstance__Q25Input7Manager@l(r3);
  lbz       r6, 0x61(r6);
  cmpwi     r6, 0x4;
  bne-      loc_0x64;
  li        r7, 0x1608;

loc_0x64:
  cmpwi     r6, 0x8;
  bne-      loc_0x70;
  li        r7, 0x1617;

loc_0x70:
  cmpwi     r6, 0x20;
  bne+      loc_0x7C;
  li        r7, 0xF16;

loc_0x7C:
  cmpwi     r6, 0x40;
  bne+      loc_0x88;
  li        r7, 0x1620;

loc_0x88:
  cmpwi     r6, 0x10;
  bne+      loc_0x94;
  li        r7, 0xD1E;

loc_0x94:
  stw       r7, 0x1630(r12);
  sth       r7, 0x1C(r28);

loc_0x9C:
  blr;

original:
  lis       r3, sInstance__Q25Input7Manager@ha;
  blr;
  )
}
kmCall(0x80660FF0, ComboSwitch);

asmFunc ComboSwitch2() {
  ASM(
      nofralloc;
loc_0x0:
  lis       r12, 0x8000;
  lbz       r12, 0x1202(r12);
  cmpwi     r12, 0;
  beq       original;
  lis       r3, 0x8150;
  stw       r28, 0x1634(r3);
  li        r3, 0x190;
  blr;

original:
  li	      r3, 180;
  blr;
  )
}
kmCall(0x80643C88, ComboSwitch2);

}

namespace VRSystem {

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

}