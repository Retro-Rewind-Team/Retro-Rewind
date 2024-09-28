#include <kamek.hpp>
#include <MarioKartWii/Item/ItemBehaviour.hpp>
#include <MarioKartWii/Item/Obj/ItemObj.hpp>
#include <PulsarEngine/Settings/Settings.hpp>
#include <PulsarEngine/SlotExpansion/CupsConfig.hpp>
#include <RetroRewind.hpp>

namespace RetroRewind {
namespace Race {
    static void ChangeBlueOBJProperties(Item::ObjProperties* dest, const Item::ObjProperties& rel) {
        new (dest) Item::ObjProperties(rel);
        if(System::GetGameMode() == System::GAMEMODE_BLAST) {
            dest->limit = 25;
        }
    else if (System::GetGameMode() == System::GAMEMODE_RANDOM){
        dest->limit = 5;
    }
    }

    kmCall(0x80790b74, ChangeBlueOBJProperties);

    static void ChangeBillOBJProperties(Item::ObjProperties* dest, const Item::ObjProperties& rel){
    new (dest) Item::ObjProperties(rel);
    if (System::GetGameMode() == System::GAMEMODE_RANDOM){
        dest->limit = 25;
    }
    else if (System::GetGameMode() == System::GAMEMODE_BLAST){
        dest->limit = 5;
    }

}

    kmCall(0x80790bf4, ChangeBillOBJProperties);

    static void ChangeBombOBJProperties(Item::ObjProperties* dest, const Item::ObjProperties& rel){
    new (dest) Item::ObjProperties(rel);
    if(System::GetGameMode() == System::GAMEMODE_RANDOM){
        dest->limit = 20;
    }
    else if (System::GetGameMode() == System::GAMEMODE_BLAST){
        dest->limit = 25;
    }
}

    kmCall(0x80790bb4, ChangeBombOBJProperties);
} // namespace Race
} // namespace RetroRewind