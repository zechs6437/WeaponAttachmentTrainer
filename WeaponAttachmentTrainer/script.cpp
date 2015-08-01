#include "weaponMods.h"
#include <ctime>

#pragma warning(disable : 4244 4305 4800) // double <-> float conversions (and some bool conversion bullshit that I don't care about)

bool featureSilencerToggle = GetPrivateProfileInt("Toggles", "SilencerToggle", 1, "./WeaponAttachmentTrainer.ini");
int trainerToggleHotkey = GetPrivateProfileInt("Hotkeys", "WATHotkey", 0x74, "./WeaponAttachmentTrainer.ini"); // Default 'F5'
int silencerToggleHotkey = GetPrivateProfileInt("Hotkeys", "SilencerHotkey", 0x4F, "./WeaponAttachmentTrainer.ini"); // Default 'O' (O as in your mom saying "oh my god ken gimme dat cack")

bool featureWeaponSilencer = false;
bool featureWeaponMagazine = false;
bool featureWeaponGrip = false;
bool featureWeaponFlashlight = false;
bool featureWeaponScope = false;

void update_features()
{
	update_status_text();

	if (featureSilencerToggle && IsKeyJustUp(silencerToggleHotkey))
	{
		toggleSilencer();
	}
}

int activeLineIndexWeapons = 0;

void process_weaponupgrade_menu()
{
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

