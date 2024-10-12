#include <kamek.hpp>
#include <PulsarEngine/PulsarSystem.hpp>
#include <PulsarEngine/Settings/Settings.hpp>
#include <MarioKartWii/GlobalFunctions.hpp>
#include <MarioKartWii/System/Identifiers.hpp>

extern u32 OnlineTTHook;
extern u32 DataRateHook;
extern u32 FPSPatchHook;
extern u32 BattleStartHook;
extern u32 DolphinCheat;
extern u32 MainDolCheat;

namespace RetroRewind {
class System : public Pulsar::System {
public:
    static bool Is500cc();

    u8 SetPackROOMMsg() override {
        kartRestrictMode = static_cast<KartRestriction>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), SETTINGRR_RADIO_KARTSELECT));
        hostMode = static_cast<Gamemode>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), SETTINGRR_SCROLLER_GAMEMODES));
        charRestrictMode = static_cast<CharacterRestriction>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), SETTINGRR_RADIO_CHARSELECT));
        ccMode = static_cast<HostSettingHostCC>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(Pulsar::Settings::SETTINGSTYPE_HOST), SETTINGHOST_RADIO_CC));

        u8 ret = kartRestrictMode + (hostMode << 2) + (charRestrictMode << 4) + (ccMode << 6);

        return ret;
    }

    void ParsePackROOMMsg(u8 msg) override {
        kartRestrictMode = static_cast<KartRestriction>(msg & 0b11);
        hostMode = static_cast<Gamemode>((msg & 0b1100) >> 2);
        charRestrictMode = static_cast<CharacterRestriction>((msg & 0b110000) >> 4);
        ccMode = static_cast<HostSettingHostCC>((msg & 0b11000000) >> 6);
    }

    enum ExtraSettingType{
        SETTINGSTYPE_RR = 3,
        SETTINGSTYPE_RR2 = 4
    };

    enum ExtraRRettings{
        SETTINGRR_RADIO_TRANSMISSION = 0,
        SETTINGRR_RADIO_HARDAI = 1,
        SETTINGRR_RADIO_KARTSELECT = 2,
        SETTINGRR_RADIO_CHARSELECT = 3,
        SETTINGRR_SCROLLER_GAMEMODES = 0 + 6
    };

    enum RR2Settings{
        SETTINGRR2_RADIO_CTMUSIC = 0,
        SETTINGRR2_RADIO_TIMES = 1,
        SETTINGRR2_RADIO_BRAKEDRIFT = 2,
        SETTIGNRR2_RADIO_FPS = 3,
        SETTINGRR2_RADIO_DATARATE = 4
    };

    enum Transmission{
        TRANSMISSION_DEFAULT,
        TRANSMISSION_INSIDEALL,
        TRANSMISSION_INSIDEBIKE,
        TRANSMISSION_OUTSIDE
    };

    enum HardAI{
        HARDAI_DISABLED,
        HARDAI_ENABLED
    };

    enum CTMusic{
        CTMUSIC_DISABLED,
        CTMUSIC_ENABLED
    };

    enum Times{
        TIMES_DISABLED,
        TIMES_ENABLED
    };

    enum BrakeDrift{
        BRAKEDRIFT_DISABLED,
        BRAKEDRIFT_ENABLED
    };

    enum ForceBrakeDrift{
        FORCEBRAKEDRIFT_DISABLED,
        FORCEBRAKEDRIFT_ENABLED
    };

    enum Gamemode{
        GAMEMODE_DEFAULT,
        GAMEMODE_RANDOM,
        GAMEMODE_ONLINETT,
        GAMEMODE_BLAST,
        GAMEMODE_NONE
    };

    enum FPS{
        FPS_DEFAULT,
        FPS_HALF
    };

    enum KartRestriction{
        KART_DEFAULTSELECTION,
        KART_KARTONLY,
        KART_BIKEONLY
    };

    enum CharacterRestriction{
        CHAR_DEFAULTSELECTION,
        CHAR_LIGHTONLY,
        CHAR_MEDIUMONLY,
        CHAR_HEAVYONLY
    };

    enum WeightClass{
        LIGHTWEIGHT,
        MEDIUMWEIGHT,
        HEAVYWEIGHT,
        MIIS,
        ALLWEIGHT
    };

    enum TC{
        TC_MEGA,
        TC_DEFAULT
    };

    enum DateRate{
        DATARATE_DISABLED,
        DATARATE_ENABLED
    };

    enum CharButtonId{
        BUTTON_BABY_MARIO,
        BUTTON_BABY_LUIGI,
        BUTTON_TOAD,
        BUTTON_TOADETTE,
        BUTTON_BABY_PEACH,
        BUTTON_BABY_DAISY,
        BUTTON_KOOPA_TROOPA,
        BUTTON_DRY_BONES,
        BUTTON_MARIO,
        BUTTON_LUIGI,
        BUTTON_YOSHI,
        BUTTON_BIRDO,
        BUTTON_PEACH,
        BUTTON_DAISY,
        BUTTON_DIDDY_KONG,
        BUTTON_BOWSER_JR,
        BUTTON_WARIO,
        BUTTON_WALUIGI,
        BUTTON_KING_BOO,
        BUTTON_ROSALINA,
        BUTTON_DONKEY_KONG,
        BUTTON_FUNKY_KONG,
        BUTTON_BOWSER,
        BUTTON_DRY_BOWSER,
        BUTTON_MII_A,
        BUTTON_MII_B
    };

    enum BMGIDS{
        RADIO_TRANSMISSION_TITLE = 0x3002,
        RADIO_TRANSMISSION_INSIDEBIKE = 0x3032,
        RADIO_TRANSMISSION_INSIDEALL = 0x3031,
        RADIO_TRANSMISSION_OUTSIDE = 0x3030,
        RADIO_TRANSMISSION_INSIDEBIKE_DESC = 0x3302,
        RADIO_TRANSMISSION_INSIDEALL_DESC = 0x3301,
        RADIO_TRANSMISSION_OUTSIDE_DESC = 0x3300,
        RADIO_NOSOUND_TITLE = 0x3001,
        RADIO_NOSOUND_DISABLED = 0x3020,
        RADIO_NOSOUND_ENABLED = 0x3021,
        RADIO_NOSOUND_DISABLED_DESC = 0x3200,
        RADIO_NOSOUND_ENABLED_DESC = 0x3201,
        RADIO_HARDAI_TITLE = 0x3003,
        RADIO_HARDAI_DISABLED = 0x3040,
        RADIO_HARDAI_ENABLED = 0x3041,
        RADIO_HARDAI_DISABLED_DESC = 0x3400,
        RADIO_HARDAI_ENABLED_DESC = 0x3401
    };

    KartRestriction kartRestrictMode;
    Gamemode hostMode;
    CharacterRestriction charRestrictMode;
    WeightClass weight;
    ForceBrakeDrift brakeDriftMode;
    TC tcMode;
    HostSettingHostCC ccMode;

    u32 noRaceProgressionTimer[4];
    float lastRaceCompletion[4];
    
    static Pulsar::System *Create(); //My Create function, needs to return Pulsar
    static System *GetsInstance(){return static_cast<System *>(sInstance);} //for ease of use, optional
    static KartRestriction GetKartRestriction();
    static WeightClass GetWeightClass(CharacterId);
    static CharacterRestriction GetCharacterRestriction();
    static Gamemode GetGameMode();
    static ForceBrakeDrift GetBrakeDrift();
    static TC GetTCMode();
    static HostSettingHostCC GetCCMode();
    
    void AfterInit() override;
};
} // namespace RetroRewind