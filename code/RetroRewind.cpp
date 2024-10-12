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