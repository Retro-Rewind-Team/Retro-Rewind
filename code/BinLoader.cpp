#include <RetroRewind.hpp>
#include <MarioKartWii/Archive/ArchiveRoot.hpp>

namespace RetroRewind {
void *GetCustomKartParam(ArchiveRoot *archive, ArchiveSource type, const char *name, u32 *length){
    const GameMode gameMode = RaceData::sInstance->racesScenario.settings.gamemode;
    if (static_cast<RetroRewind::System::Transmission>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), RetroRewind::System::SETTINGRR_RADIO_TRANSMISSION)) == RetroRewind::System::TRANSMISSION_INSIDEALL)
    {
        name="kartParamAll.bin";
    }
    else if (static_cast<RetroRewind::System::Transmission>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), RetroRewind::System::SETTINGRR_RADIO_TRANSMISSION)) == RetroRewind::System::TRANSMISSION_INSIDEBIKE)
    {
        name="kartParamBike.bin";
    }
    else if (static_cast<RetroRewind::System::Transmission>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), RetroRewind::System::SETTINGRR_RADIO_TRANSMISSION)) == RetroRewind::System::TRANSMISSION_OUTSIDE)
    {
        name="kartParamOut.bin";
    }
    return archive->GetFile(type, name, length);
}
kmCall(0x80591a30, GetCustomKartParam);

void *GetCustomKartAIParam(ArchiveRoot *archive, ArchiveSource type, const char *name, u32 *length){
    const GameMode gameMode = RaceData::sInstance->racesScenario.settings.gamemode;
    if (static_cast<RetroRewind::System::HardAI>(Pulsar::Settings::Mgr::GetSettingValue(static_cast<Pulsar::Settings::Type>(System::SETTINGSTYPE_RR), RetroRewind::System::SETTINGRR_RADIO_HARDAI)) == RetroRewind::System::HARDAI_ENABLED
     && !Pulsar::CupsConfig::IsRegsSituation())
    {
        name="kartAISpdParamRR.bin";
    }
    
    return archive->GetFile(type, name, length);
}
kmCall(0x8073ae9c, GetCustomKartAIParam);

void *GetCustomItemSlot(ArchiveRoot *archive, ArchiveSource type, const char *name, u32 *length){
    const System::Gamemode gameMode = System::GetGameMode();
    if (gameMode == System::GAMEMODE_DEFAULT)
    {
        name="ItemSlotRR.bin";
    }
    else if (gameMode == System::GAMEMODE_RANDOM)
    {
        name="ItemSlotRandom.bin";
    }
    else if (gameMode == System::GAMEMODE_BLAST)
    {
        name="ItemSlotBlast.bin";
    }
    return archive->GetFile(type, name, length);

}
kmCall(0x807bb128, GetCustomItemSlot);
kmCall(0x807bb030, GetCustomItemSlot);
kmCall(0x807bb200, GetCustomItemSlot);
kmCall(0x807bb53c, GetCustomItemSlot);
kmCall(0x807bbb58, GetCustomItemSlot);
}