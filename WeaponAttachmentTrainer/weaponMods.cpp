#include "weaponMods.h"

Hash silencerHashes[5] = { 0x65EA7EBB, 0x837445AA, 0xA73D4664, 0xC304849A, 0xE608B35E };
Hash magazineHashes[22] = { 0x249A17D5, 0x64F9C62B, 0x7B0033B3, 0x7C8BD10E, 0x86BD7F72, 0x8EC1C979, 0xB3688B0F, 0xD6C59CD6, 0xED265A1C, 0xD67B4F2D, 0xD9D3AC92, 0x33BA12E8, 0x10E6BA2B, 0x350966FB,
0xBB46E417, 0x334A5203, 0x82158B47, 0xEAC8C270, 0xB1214F9B, 0x91109691, 0xCCFD2AC5, 0x971CF6FD };
Hash gripHash = 0xC164F53;
Hash flashlightHashes[2] = { 0x359B7AAE, 0x7BC4CDDC };
Hash scopeHashes[7] = { 0x9D2FBF29, 0xA0D89C42, 0xAA2C45B4, 0xD2443DDC, 0x3CC6BA57, 0x3C00AFED, 0xBC54DA77 };

void toggleSilencer()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int a = 0; a < 5; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, silencerHashes[a]))
		{
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, silencerHashes[a]))
			{
				WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, playerWeapon, silencerHashes[a]);
				set_status_text("Silencer Removed");
			}
			else
			{
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, playerWeapon, silencerHashes[a]);
				set_status_text("Silencer Attached");
			}
		}
	}
}

void toggleMagazine()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int a = 0; a < 22; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, magazineHashes[a]))
		{
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, magazineHashes[a]))
			{
				WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, playerWeapon, magazineHashes[a]);
				set_status_text("Extended Magazine Removed");
			}
			else
			{
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, playerWeapon, magazineHashes[a]);
				set_status_text("Extended Magazine Attached");
			}
		}
	}
}

void toggleGrip()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, gripHash))
	{
		if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, gripHash))
		{
			WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, playerWeapon, gripHash);
			set_status_text("Grip Removed");
		}
		else
		{
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, playerWeapon, gripHash);
			set_status_text("Grip Attached");
		}
	}
}

void toggleFlashlight()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int a = 0; a < 2; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, flashlightHashes[a]))
		{
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, flashlightHashes[a]))
			{
				WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, playerWeapon, flashlightHashes[a]);
				set_status_text("Flashlight Removed");
			}
			else
			{
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, playerWeapon, flashlightHashes[a]);
				set_status_text("Flashlight Attached");
			}
		}
	}
}

void toggleScope()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	for (int a = 0; a < 7; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, scopeHashes[a]))
		{
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, scopeHashes[a]))
			{
				WEAPON::REMOVE_WEAPON_COMPONENT_FROM_PED(playerPed, playerWeapon, scopeHashes[a]);
				set_status_text("Scope Removed");
			}
			else
			{
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(playerPed, playerWeapon, scopeHashes[a]);
				set_status_text("Scope Attached");
			}
		}
	}
}

void weaponSanityCheck()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	bool silencerSanity = false;
	bool magazineSanity = false;
	bool flashlightSanity = false;
	bool scopeSanity = false;

	for (int a = 0; a < 5; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, silencerHashes[a]))
		{
			silencerSanity = true;
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, silencerHashes[a]))
				featureWeaponSilencer = true;
			else
				silencerSanity = false;
			featureWeaponSilencer = false;
		}
	}
	featureWeaponSilencer = silencerSanity;

	for (int a = 0; a < 22; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, magazineHashes[a]))
		{
			magazineSanity = true;
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, magazineHashes[a]))
				featureWeaponMagazine = true;
			else
				magazineSanity = false;
			featureWeaponMagazine = false;
		}
	}
	featureWeaponMagazine = magazineSanity;

	if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, gripHash))
		featureWeaponGrip = true;
	else
		featureWeaponGrip = false;

	for (int a = 0; a < 2; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, flashlightHashes[a]))
		{
			flashlightSanity = true;
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, flashlightHashes[a]))
				featureWeaponFlashlight = true;
			else
				flashlightSanity = false;
			featureWeaponFlashlight = false;
		}
	}
	featureWeaponFlashlight = flashlightSanity;

	for (int a = 0; a < 7; a = a + 1)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(playerWeapon, scopeHashes[a]))
		{
			scopeSanity = true;
			if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(playerPed, playerWeapon, scopeHashes[a]))
				featureWeaponScope = true;
			else
				scopeSanity = false;
			featureWeaponScope = false;
		}
	}
	featureWeaponScope = scopeSanity;

}
