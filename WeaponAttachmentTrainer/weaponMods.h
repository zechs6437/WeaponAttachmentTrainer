#ifndef WEAPONMODS
#define WEAPONMODS

#include "drawMenu.h"

extern bool featureWeaponSilencer;
extern bool featureWeaponMagazine;
extern bool featureWeaponGrip;
extern bool featureWeaponFlashlight;
extern bool featureWeaponScope;

void toggleSilencer();
void toggleMagazine();
void toggleGrip();
void toggleFlashlight();
void toggleScope();
void weaponSanityCheck();

#endif

//Below are my notes 'n shit
//Credit to DaBOSS54320 on GTAForums for the attachment hashes http://gtaforums.com/topic/807165-weapon-component-hashes/
/*
BOOL HAS_PED_GOT_WEAPON_COMPONENT(Ped ped, Hash weaponHash, Hash componentHash)
BOOL IS_PED_WEAPON_COMPONENT_ACTIVE(Ped ped, Hash weaponHash, Hash componentHash)
BOOL _CAN_WEAPON_HAVE_COMPONENT(Hash weaponHash, Hash componentHash)
void REMOVE_WEAPON_COMPONENT_FROM_PED(Ped ped, Hash weaponHash, Hash componentHash)
void GIVE_WEAPON_COMPONENT_TO_PED(Ped ped, Hash weaponHash, Hash componentHash)
Hash GET_SELECTED_PED_WEAPON(Ped ped)


Advanced scope - 0xBC54DA77: Sniper Rifle, Heavy Sniper,
Extended clip - 0x249A17D5: AP Pistol,
Extended clip - 0x64F9C62B: Heavy Pistol,
Extended clip - 0x7B0033B3: SNS Pistol,
Extended clip - 0x7C8BD10E: Special Carbine,
Extended clip - 0x86BD7F72: Assault Shotgun,
Extended clip - 0x8EC1C979: Advanced Rifle,
Extended clip - 0xB3688B0F: Bullpup Rifle,
Extended clip - 0xD6C59CD6: Combat MG,
Extended clip - 0xED265A1C: Pistol,
Extended clip - 0xD67B4F2D: Combat Pistol,
Extended clip - 0xD9D3AC92: .50 Pistol,
Extended clip - 0x33BA12E8: Vintage Pistol,
Extended clip - 0x10E6BA2B: Micro SMG,
Extended clip - 0x350966FB: SMG,
Extended clip - 0xBB46E417: Assault SMG,
Extended clip - 0x334A5203: Combat PDW,
Extended clip - 0x82158B47: MG,
Extended clip - 0xEAC8C270: Gusenberg,
Extended clip - 0xB1214F9B: Assault Rifle,
Extended clip - 0x91109691: Carbine Rifle,
Extended clip - 0xCCFD2AC5: Marksman Rifle,
Extended clip - 0x971CF6FD: Heavy Shotgun,
Flashlight - 0x359B7AAE: Pistol, Combat Pistol, AP Pistol, Heavy Pistol, .50 Pistol, Micro SMG,
Flashlight - 0x7BC4CDDC: SMG, Assault SMG, Combat PDW, Assault Rifle, Carbine Rifle, Advanced Rifle, Special Carbine, Bullpup Rifle, Pump Shotgun, Assault Shotgun, Bullpup Shotgun, Heavy Shotgun, Marksman Rifle, Grenade Launcher, Smoke Grenade Launcher,
Grip - 0xC164F53: Combat PDW, Assault Rifle, Carbine Rifle, Special Carbine, Bullpup Rifle, Combat MG, Assault Shotgun, Bullpup Shotgun, Heavy Shotgun, Marksman Rifle, Grenade Launcher, Smoke Grenade Launcher,
Regular clip - 0xC5A12F80: Bullpup Rifle,
Regular clip - 0xC6C7E581: Special Carbine,
Regular clip - 0xD4A969A: Heavy Pistol,
Regular clip - 0xE1FFB34A: Combat MG,
Regular clip - 0xF8802ED9: SNS Pistol,
Scope - 0x9D2FBF29: Micro SMG, Assault SMG, Assault Rifle,
Scope - 0xA0D89C42: Carbine Rifle, Special Carbine, Combat MG,
Scope - 0xAA2C45B4: Combat PDW, Advanced Rifle, Bullpup Rifle, Grenade Launcher, Smoke Grenade Launcher,
Scope - 0xD2443DDC: Sniper Rifle, Heavy Sniper,
Scope - 0x3CC6BA57: SMG,
Scope - 0x3C00AFED: MG,
Suppressor - 0x65EA7EBB: Pistol,
Suppressor - 0x837445AA: Carbine Rifle, Advanced Rifle, Bullpup Rifle, Assault Shotgun, Marksman Rifle,
Suppressor - 0xA73D4664: .50 Pistol, Micro SMG, Assault SMG, Assault Rifle, Special Carbine, Bullpup Shotgun, Heavy Shotgun, Sniper Rifle,
Suppressor - 0xC304849A: Combat Pistol, AP Pistol, Heavy Pistol, Vintage Pistol, SMG,
Suppressor - 0xE608B35E: Pump Shotgun,
Special finish - 0x27872C90: SMG,
Special finish - 0xD7391086: Pistol,
Special finish - 0x9B76C72C: AP Pistol,
Special finish - 0x487AAE09: Micro SMG,
Special finish - 0x85A64DF9: Sawed-off Shotgun,
Special finish - 0x377CD377: Advanced Rifle,
Special finish - 0xD89B9658: Carbine Rifle,
Special finish - 0x4EAD7533: Assault Rifle,
Special finish - 0x4032B5E7: Sniper Rifle,
Special finish - 0x77B8AB2F: .50 Pistol,
Special finish - 0x7A6A7B7B: Heavy Pistol,
Special finish - 0x161E9241: Marksman Rifle,
??? - 0x9BC64089: Sniper Rifle,
*/
