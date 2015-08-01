#include "script.h"
#include "keyboard.h"
#include <string>
#include <ctime>

#pragma warning(disable : 4244 4305 4800) // double <-> float conversions (and some bool conversion bullshit that I don't care about)

BOOL niceFlyToggle = true;

Hash silencerHashes[5] = { 0x65EA7EBB, 0x837445AA, 0xA73D4664, 0xC304849A, 0xE608B35E };
Hash magazineHashes[22] = { 0x249A17D5, 0x64F9C62B, 0x7B0033B3, 0x7C8BD10E, 0x86BD7F72, 0x8EC1C979, 0xB3688B0F, 0xD6C59CD6, 0xED265A1C, 0xD67B4F2D, 0xD9D3AC92, 0x33BA12E8, 0x10E6BA2B, 0x350966FB,
	0xBB46E417, 0x334A5203, 0x82158B47, 0xEAC8C270, 0xB1214F9B, 0x91109691, 0xCCFD2AC5, 0x971CF6FD };
Hash gripHash = 0xC164F53;
Hash flashlightHashes[2] = { 0x359B7AAE, 0x7BC4CDDC };
Hash scopeHashes[7] = { 0x9D2FBF29, 0xA0D89C42, 0xAA2C45B4, 0xD2443DDC, 0x3CC6BA57, 0x3C00AFED, 0xBC54DA77 };

bool featureNiceFly = GetPrivateProfileInt("Toggles", "NiceFly", 0, "./Ragdolla.ini");
bool featureGTAIVCarExit = GetPrivateProfileInt("Toggles", "GTAIVCarExit", 0, "./Ragdolla.ini");
bool featureRagdoll = GetPrivateProfileInt("Toggles", "Ragdoll", 0, "./Ragdolla.ini");
bool featureAlwaysRadio = GetPrivateProfileInt("Toggles", "AlwaysRadio", 1, "./Ragdolla.ini");
bool featureAlwaysRadioUpdated = false;
bool featureCarUpgrade = GetPrivateProfileInt("Toggles", "CarUpgrade", 1, "./Ragdolla.ini");
bool featureSilencerToggle = GetPrivateProfileInt("Toggles", "SilencerToggle", 1, "./Ragdolla.ini");
bool featureWeaponSilencer = false;
bool featureWeaponMagazine = false;
bool featureWeaponGrip = false;
bool featureWeaponFlashlight = false;
bool featureWeaponScope = false;

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

bool trainer_switch_pressed()
{
	return IsKeyJustUp(VK_F5);
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	if (a) *a = IsKeyDown(VK_NUMPAD5);
	if (b) *b = IsKeyDown(VK_NUMPAD0) || trainer_switch_pressed() || IsKeyDown(VK_BACK);
	if (up) *up = IsKeyDown(VK_NUMPAD8);
	if (down) *down = IsKeyDown(VK_NUMPAD2);
	if (r) *r = IsKeyDown(VK_NUMPAD6);
	if (l) *l = IsKeyDown(VK_NUMPAD4);
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = { 255, 255, 255, 255 },
		rect_col[4] = { 70, 95, 95, 255 };
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 218;
		rect_col[1] = 242;
		rect_col[2] = 216;

		if (rescaleText) text_scale = 0.40;
	}

	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.50;
		font = 1;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f),
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}

std::string line_as_str(std::string text, bool *pState)
{
	while (text.size() < 18) text += " ";
	return text + (pState ? (*pState ? " [ON]" : " [OFF]") : "");
}

void gtaivCarExit()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	//Player player = PLAYER::GET_PLAYER_PED(playerPed);
	Any playerVehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	//float playerVehicleSpeed = ENTITY::GET_ENTITY_SPEED(playerVehicle);
	Any isPlayerDriving = (VEHICLE::GET_PED_IN_VEHICLE_SEAT(playerVehicle, -1) == playerPed);

	if (isPlayerDriving)
	{
		for (int i = 0; i < 10; i++)
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(playerVehicle, true, true);
			WAIT(0);
		}
		WAIT(240);
		if (CONTROLS::IS_CONTROL_PRESSED(2, 75))
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(playerVehicle, false, true);
		}
		else
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(playerVehicle, true, true);
		}
		//WAIT(1000);
	}
}

void carModder()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Any player = PLAYER::GET_PLAYER_PED(playerPed);
	BOOL playerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	if (playerExists) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
			Any veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, true);
			VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
			//VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120); //color
			VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, true); //turbo
			VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, true); //xenon
			VEHICLE::SET_VEHICLE_MOD(veh, 0, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 0) - 1, true); //spoiler
			VEHICLE::SET_VEHICLE_MOD(veh, 1, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 1) - 1, true); //frontbumper
			VEHICLE::SET_VEHICLE_MOD(veh, 2, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 2) - 1, true); //rearbumper
			VEHICLE::SET_VEHICLE_MOD(veh, 3, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 3) - 1, true); //side skirt
			VEHICLE::SET_VEHICLE_MOD(veh, 4, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 4) - 1, true); //exhaust
			VEHICLE::SET_VEHICLE_MOD(veh, 5, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 5) - 1, true); //frame
			VEHICLE::SET_VEHICLE_MOD(veh, 6, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 6) - 1, true); //grill
			VEHICLE::SET_VEHICLE_MOD(veh, 7, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 7) - 1, true); //hood
			VEHICLE::SET_VEHICLE_MOD(veh, 8, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 8) - 1, true); //fender
			VEHICLE::SET_VEHICLE_MOD(veh, 9, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 9) - 1, true); //right fender
			VEHICLE::SET_VEHICLE_MOD(veh, 10, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 10) - 1, true); //roof
			VEHICLE::SET_VEHICLE_MOD(veh, 11, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 11) - 1, true); //engine
			VEHICLE::SET_VEHICLE_MOD(veh, 12, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 12) - 1, true); //brakes
			VEHICLE::SET_VEHICLE_MOD(veh, 13, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 13) - 1, true); //tranny
			//VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, true); //horn
			VEHICLE::SET_VEHICLE_MOD(veh, 15, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 15) - 1, true); //suspension
			VEHICLE::SET_VEHICLE_MOD(veh, 16, VEHICLE::GET_NUM_VEHICLE_MODS(veh, 16) - 1, true); //armor
			//printf("Armor Mods:%d\n", VEHICLE::GET_NUM_VEHICLE_MODS(veh, 16));
			//VEHICLE::SET_VEHICLE_MOD(veh, 23, 19, true); //front wheels
			//VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, 3, 0, 0);
			//VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, 3, 0);
			VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6); //rims
			VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 1);
			set_status_text("Car Fully Upgraded");
		}
	}

}

void ragdoller()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL playerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	if (!playerExists) return;
	PED::SET_PED_CAN_RAGDOLL(playerPed, true);
	PED::SET_PED_TO_RAGDOLL(playerPed, 0, 0, 0, false, false, false);
}

void niceFly()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	//Any player = PLAYER::GET_PLAYER_PED(playerPed);
	if (CONTROLS::IS_CONTROL_PRESSED(0, 21))
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		if (!WEAPON::HAS_PED_GOT_WEAPON(playerPed, 4222310262, 0))
		{
			WEAPON::GIVE_WEAPON_TO_PED(playerPed, 4222310262, 1, true, true);
		}
		if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed) > 1.5)
		{
			if (PED::GET_PED_PARACHUTE_STATE(playerPed) != 3)
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 0, 0, 10000, 0, 0, 0, 0, 0, true, false, false, false, false);
			}
			if (CONTROLS::IS_CONTROL_PRESSED(0, 31))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 0, 0, 0, 10000, 0, 0, 0, 0, true, false, false, false, false);
			}
		}
	}
}

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

void update_features()
{
	update_status_text();

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	if (featureNiceFly && CONTROLS::IS_CONTROL_PRESSED(0, 21))
		niceFly();

	if (featureGTAIVCarExit && CONTROLS::IS_CONTROL_PRESSED(2, 75))
		gtaivCarExit();

	if (featureRagdoll && IsKeyDown(0x55)) // Keyboard 'U'
		ragdoller();

	if (featureAlwaysRadioUpdated)
	{
		if (bPlayerExists && !featureAlwaysRadio)
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		featureAlwaysRadioUpdated = false;
	}
	if (featureAlwaysRadio)
	{
		if (bPlayerExists)
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
	}

	if (featureCarUpgrade && IsKeyJustUp(0x12))
		carModder();

	Hash playerWeapon = WEAPON::GET_SELECTED_PED_WEAPON(playerPed);

	if (featureSilencerToggle && IsKeyJustUp(0x4F)) //keyboard 'O' (O as in your mom saying "oh my god ken gimme dat cack")
	{
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
}

int activeLineIndexWeapons = 0;

void process_weaponupgrade_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 6;

	std::string caption = "Weapon Upgrades";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Silencer EZ Toggle (O)", &featureSilencerToggle, NULL },
		{ "Silencer Attachment", &featureWeaponSilencer, NULL },
		{ "Magazine Attachment", &featureWeaponMagazine, NULL },
		{ "Grip Attachment", &featureWeaponGrip, NULL },
		{ "Flashlight Attachment", &featureWeaponFlashlight, NULL },
		{ "Scope Attachment", &featureWeaponScope, NULL }
	};
	DWORD waitTime = 150;
	while (true)
	{
		weaponSanityCheck();
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeapons)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeapons].text, lines[activeLineIndexWeapons].pState),
				lineWidth + 1.0, 11.0, 56.0 + activeLineIndexWeapons * 36.0, 0.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWeapons)
			{
			case 1:
			{
				if (!featureWeaponSilencer)
					WritePrivateProfileString("Toggles", "SilencerToggle", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "SilencerToggle", " 0", "./Ragdolla.ini");
				toggleSilencer();
				featureWeaponSilencer = !featureWeaponSilencer;
				break;
			}
			case 2:
			{
				toggleMagazine();
				featureWeaponMagazine = !featureWeaponMagazine;
				break;
			}
			case 3:
			{
				toggleGrip();
				featureWeaponGrip = !featureWeaponGrip;
				break;
			}
			case 4:
			{
				toggleFlashlight();
				featureWeaponFlashlight = !featureWeaponFlashlight;
				break;
			}
			case 5:
			{
				toggleScope();
				featureWeaponScope = !featureWeaponScope;
				break;
			}
			default:
				if (lines[activeLineIndexWeapons].pState)
					*lines[activeLineIndexWeapons].pState = !(*lines[activeLineIndexWeapons].pState);
				if (lines[activeLineIndexWeapons].pUpdated)
					*lines[activeLineIndexWeapons].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWeapons == 0)
						activeLineIndexWeapons = lineCount;
					activeLineIndexWeapons--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWeapons++;
						if (activeLineIndexWeapons == lineCount)
							activeLineIndexWeapons = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexMain = 0;

void process_main_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 6;

	std::string caption = "Maarek's Awesome Shit";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Nice Fly", &featureNiceFly, NULL },
		{ "GTAIV Car Exit", &featureGTAIVCarExit, NULL },
		{ "Ragdoll (U)", &featureRagdoll, NULL },
		{ "Always Radio", &featureAlwaysRadio, &featureAlwaysRadioUpdated },
		{ "Car Upgrade (ALT)", &featureCarUpgrade, NULL },
		{ "Weapon Upgrades", NULL, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 15.0, 18.0, 0.0, 5.0, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMain)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState),
					lineWidth, 9.0, 60.0 + i * 36.0, 0.0, 9.0, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexMain].text, lines[activeLineIndexMain].pState),
				lineWidth + 1.0, 11.0, 56.0 + activeLineIndexMain * 36.0, 0.0, 7.0, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMain)
			{
			case 0:
				if (!featureNiceFly)
					WritePrivateProfileString("Toggles", "NiceFly", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "NiceFly", " 0", "./Ragdolla.ini");
				goto GHETTOJUMP;
			case 1:
				if (!featureGTAIVCarExit)
					WritePrivateProfileString("Toggles", "GTAIVCarExit", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "GTAIVCarExit", " 0", "./Ragdolla.ini");
				goto GHETTOJUMP;
			case 2:
				if (!featureRagdoll)
					WritePrivateProfileString("Toggles", "Ragdoll", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "Ragdoll", " 0", "./Ragdolla.ini");
				goto GHETTOJUMP;
			case 3:
				if (!featureAlwaysRadio)
					WritePrivateProfileString("Toggles", "AlwaysRadio", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "AlwaysRadio", " 0", "./Ragdolla.ini");
				goto GHETTOJUMP;
			case 4:
				if (!featureCarUpgrade)
					WritePrivateProfileString("Toggles", "CarUpgrade", " 1", "./Ragdolla.ini");
				else
					WritePrivateProfileString("Toggles", "CarUpgrade", " 0", "./Ragdolla.ini");
				goto GHETTOJUMP;
			case 5:
				weaponSanityCheck();
				process_weaponupgrade_menu();
				break;
GHETTOJUMP: //this is so bad
			default:
				if (lines[activeLineIndexMain].pState)
					*lines[activeLineIndexMain].pState = !(*lines[activeLineIndexMain].pState);
				if (lines[activeLineIndexMain].pUpdated)
					*lines[activeLineIndexMain].pUpdated = true;
			}
			waitTime = 200;
		}
		else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		}
		else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexMain == 0)
				activeLineIndexMain = lineCount;
			activeLineIndexMain--;
			waitTime = 150;
		}
		else
		if (bDown)
		{
			menu_beep();
			activeLineIndexMain++;
			if (activeLineIndexMain == lineCount)
				activeLineIndexMain = 0;
			waitTime = 150;
		}
	}
}

void main()
{
	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			process_main_menu();
		}
		update_features();
		WAIT(0);
	}
}

void ScriptMain()
{
//	AllocConsole();
//	freopen("CONIN$", "r", stdin);
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
	srand(GetTickCount());
	main();
}