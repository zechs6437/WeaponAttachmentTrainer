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

bool featureSilencerToggle = GetPrivateProfileInt("Toggles", "SilencerToggle", 1, "./WeaponAttachmentTrainer.ini");
int trainerToggleHotkey = GetPrivateProfileInt("Hotkeys", "WATHotkey", 0x74, "./WeaponAttachmentTrainer.ini"); // Default 'F5'
int silencerToggleHotkey = GetPrivateProfileInt("Hotkeys", "SilencerHotkey", 0x4F, "./WeaponAttachmentTrainer.ini"); // Default 'O' (O as in your mom saying "oh my god ken gimme dat cack")

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
	return IsKeyJustUp(trainerToggleHotkey);
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
	int text_col[4] = { 0xF0, 0xF0, 0xF0, 255 },
		rect_col[4] = { 0x20, 0x20, 0x20, 255 };
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0x20;
		text_col[1] = 0x20;
		text_col[2] = 0x20;

		rect_col[0] = 0xB9;
		rect_col[1] = 0xB1;
		rect_col[2] = 0xA8;

		if (rescaleText) text_scale = 0.40;
	}

	if (title)
	{
		rect_col[0] = 0x00;
		rect_col[1] = 0x46;
		rect_col[2] = 0x87;

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

	if (featureSilencerToggle && IsKeyJustUp(silencerToggleHotkey)) //keyboard 'O' (O as in your mom saying "oh my god ken gimme dat cack")
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
}

int activeLineIndexWeapons = 0;

void process_weaponupgrade_menu()
{
	//const float lineWidth = 250.0;
	const float lineWidth = 300.0;
	const int lineCount = 6;

	std::string caption = "Weapon Attachment Trainer by Gumshoe";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "Silencer EZ Toggle (Default: 'O')", &featureSilencerToggle, NULL },
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
				case 0:
				{
					if (featureSilencerToggle)
						WritePrivateProfileString("Toggles", "SilencerToggle", " 0", "./WeaponAttachmentTrainer.ini");
					else
						WritePrivateProfileString("Toggles", "SilencerToggle", " 1", "./WeaponAttachmentTrainer.ini");
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

void main()
{
	while (true)
	{
		if (trainer_switch_pressed())
		{
			menu_beep();
			process_weaponupgrade_menu();
		}
		update_features();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

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
