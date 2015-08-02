#ifndef WEAPONMODS
#define WEAPONMODS

#include "drawMenu.h"

static Hash silencerHashes[5] = { 0x65EA7EBB, 0x837445AA, 0xA73D4664, 0xC304849A, 0xE608B35E };
static Hash magazineHashes[22] = { 0x249A17D5, 0x64F9C62B, 0x7B0033B3, 0x7C8BD10E, 0x86BD7F72, 0x8EC1C979, 0xB3688B0F, 0xD6C59CD6, 0xED265A1C, 0xD67B4F2D, 0xD9D3AC92, 0x33BA12E8, 0x10E6BA2B, 0x350966FB,
	0xBB46E417, 0x334A5203, 0x82158B47, 0xEAC8C270, 0xB1214F9B, 0x91109691, 0xCCFD2AC5, 0x971CF6FD };
static Hash gripHash = 0xC164F53;
static Hash flashlightHashes[2] = { 0x359B7AAE, 0x7BC4CDDC };
static Hash scopeHashes[7] = { 0x9D2FBF29, 0xA0D89C42, 0xAA2C45B4, 0xD2443DDC, 0x3CC6BA57, 0x3C00AFED, 0xBC54DA77 };
static Hash skinHashes[12] = { 0x4ead7533, 0x377cd377, 0xd89b9658, 0x9b76c72c, 0xd7391086, 0x77b8ab2f, 0x7a6a7b7b, 0x27872c90, 0x487aae09, 0x85a64df9, 0x4032b5e7, 0x161e9241 };

extern bool featureWeaponSilencer;
extern bool featureWeaponMagazine;
extern bool featureWeaponGrip;
extern bool featureWeaponFlashlight;
extern bool featureWeaponScope;
extern bool featureWeaponTint[];
extern bool featureWeaponSkinDLC;

void toggleSilencer();
void toggleMagazine();
void toggleGrip();
void toggleFlashlight();
void toggleScope();
void toggleWeaponSkin();
void changeWeaponTint(int tintSelection);
void checkWeaponTint();
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

/* 
	Additional notes
	----------------
	DLC Weapon Skin Component Hashes:
	excerpt from gunclub_shop.c4

	auto sub_26bcf(auto a_0) {
		switch (a_0) {
		case ${weapon_assaultrifle}:
			return 0x4ead7533;
		case ${weapon_advancedrifle}:
			return 0x377cd377;
		case ${weapon_carbinerifle}:
			return 0xd89b9658;
		case ${weapon_appistol}:
			return 0x9b76c72c;
		case ${weapon_pistol}:
			return 0xd7391086;
		case 0x99aeeb3b: 		//pistol .50
			return 0x77b8ab2f;
		case ${weapon_heavypistol}:
			return 0x7a6a7b7b;
		case ${weapon_smg}:
			return 0x27872c90;
		case ${weapon_microsmg}:
			return 0x487aae09;
		case ${weapon_sawnoffshotgun}:
			return 0x85a64df9;
		case ${weapon_sniperrifle}:
			return 0x4032b5e7;
		case 0xc734385a:		//marksman rifle
			return 0x161e9241;
		}
		return 0;
	}
*/
