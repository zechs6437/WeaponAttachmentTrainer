#include "weaponMods.h"

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
