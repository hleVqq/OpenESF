#include <amxmodx>
#include <engine>
#include <fakemeta>
#include <hamsandwich>
#include <json>

new Array:CharacterData;
new CharacterMenu;

enum _:CHARACTER_DATA
{
	CHARACTER_NAME[32], 
	Array:CHARACTER_FORMS
};

enum _:FORM_DATA
{
	FORM_NAME[32],
	FORM_MODEL_NAME[32],
	FORM_POWERUP_COLOR[3]
};

new PlayerCharacter[MAX_PLAYERS + 1] = {-1, ...};
new PlayerForm[MAX_PLAYERS + 1];
new bool:Developer[MAX_PLAYERS + 1];

public plugin_precache()
{
	LoadCharacters();
}

public plugin_init()
{
	register_plugin("OpenESF", "1.0", "hleV");

	register_message(get_user_msgid("VGUIMenu"), "@OnVguiMenu");
	register_message(get_user_msgid("Powerup"), "@OnPowerup");

	register_logevent("@OnJoinedTeam", 3, "1=joined team");

	RegisterHam(Ham_Spawn, "player", "@OnPlayerSpawn", true);

	CreateCharacterMenu();

	register_clcmd("say /test", "@OnSayTest");
}

@OnSayTest(plr)
{
	if (!IsDeveloper(plr))
		return PLUGIN_CONTINUE;
	
	// Test stuff

	return PLUGIN_HANDLED;
}

public client_disconnected(plr)
{
	ResetCharacter(plr);
}

public client_infochanged(plr)
{
	CacheDeveloperStatus(plr);

	if (is_user_alive(plr) && HasCharacter(plr))
		RefreshPlayerModel(plr);
}

@OnVguiMenu(msg, dest, ent)
{
	new menu = get_msg_arg_int(1);

	if (menu == 3)
	{
		RequestFrame("@OnClassMenu", ent);

		return PLUGIN_HANDLED;
	}

	return PLUGIN_CONTINUE;
}

@OnPowerup(msg, dest, ent)
{
	new plr = get_msg_arg_int(1);

	new cData[CHARACTER_DATA], fData[FORM_DATA];
	GetPlayerCharacterData(plr, cData, fData);

	for (new i; i < 3; i++)
		set_msg_arg_int(i + 2, ARG_BYTE, fData[FORM_POWERUP_COLOR][i]);
}

@OnJoinedTeam()
{
	new team[2];
	read_logargv(2, team, charsmax(team));

	if (team[0] != 'S')
		return;
	
	new logUser[80], name[32], team[2];
    read_logargv(0, logUser, charsmax(logUser));
    parse_loguser(logUser, name, charsmax(name));

    new plr = get_user_index(name);
	ResetCharacter(plr);
}

@OnPlayerSpawn(plr)
{
	if (!is_user_alive(plr))
		return;
	
	RefreshPlayerModel(plr);
}

@OnClassMenu(plr)
{
	UpdateCharacterMenuExitOption(plr);
	menu_display(plr, CharacterMenu);
}

@OnCharacterMenu(plr, menu, item)
{
	if (!is_user_connected(plr))
		return;
	
	if (item < 0)
		return;
	
	PlayerCharacter[plr] = item;
	PlayerForm[plr] = 0;
	engclient_cmd(plr, "goku");
}

@OnCharacterMenuPage(plr, status)
{
	UpdateCharacterMenuExitOption(plr);
}

LoadCharacters()
{
	server_print("[OpenESF] Reading character files...");
	CharacterData = ArrayCreate(CHARACTER_DATA);

	new path[128];
	get_localinfo("amxx_datadir", path, charsmax(path));
	add(path, charsmax(path), "/OpenEsf/Characters");

	new Array:files = ArrayCreate(128);
	GetFiles(path, files, ".json");

	for (new i, size = ArraySize(files); i < size; i++)
	{
		ArrayGetString(files, i, path, charsmax(path));
		server_print(path);
		AddCharacterFromFile(path);
	}
}

AddCharacterFromFile(const path[])
{
	new JSON:cJson = json_parse(path, true);

	if (cJson == Invalid_JSON)
		return;
	
	new cData[CHARACTER_DATA];
	json_object_get_string(cJson, "name", cData[CHARACTER_NAME], charsmax(cData[CHARACTER_NAME]));
	cData[CHARACTER_FORMS] = ArrayCreate(FORM_DATA);

	new JSON:fJson = json_object_get_value(cJson, "forms");

	for (new i = 0, count = json_array_get_count(fJson), JSON:form, JSON:powerupColorJson, fData[FORM_DATA]; i < count; i++)
	{
		form = json_array_get_value(fJson, i);

		if (json_object_get_bool(form, "disabled"))
			continue;

		json_object_get_string(form, "name", fData[FORM_NAME], charsmax(fData[FORM_NAME]));
		json_object_get_string(form, "modelName", fData[FORM_MODEL_NAME], charsmax(fData[FORM_MODEL_NAME]));

		powerupColorJson = json_object_get_value(form, "powerupColor");

		for (new j; j < 3; j++)
			fData[FORM_POWERUP_COLOR][j] = json_array_get_number(powerupColorJson, j);
		
		json_free(powerupColorJson);

		ArrayPushArray(cData[CHARACTER_FORMS], fData);
		precache_model(GetPlayerModelPath(fData[FORM_MODEL_NAME]));
	}

	ArrayPushArray(CharacterData, cData);
	json_free(fJson);
	json_free(cJson);
}

CreateCharacterMenu()
{
	CharacterMenu = menu_create("Character Menu", "@OnCharacterMenu");

	for (new i, count = ArraySize(CharacterData), cData[CHARACTER_DATA]; i < count; i++)
	{
		ArrayGetArray(CharacterData, i, cData);
		menu_additem(CharacterMenu, cData[CHARACTER_NAME]);
	}

	menu_setprop(CharacterMenu, MPROP_PAGE_CALLBACK, "@OnCharacterMenuPage");
}

RefreshPlayerModel(plr)
{
	new fData[FORM_DATA];
	GetPlayerCharacterData(plr, _, fData);
	SetPlayerModel(plr, fData[FORM_MODEL_NAME]);
}

GetFiles(path[], Array:files, const ext[] = "")
{
	new extLen = strlen(ext);

	new file[32], FileType:type;
	new dir = open_dir(path, file, charsmax(file), type);

	if (dir)
	{
		new len, addedPath[128];

		do
		{
			if (type == FileType_Unknown || equal(file, ".") || equal(file, ".."))
				continue;
			
			formatex(addedPath, charsmax(addedPath), "%s/%s", path, file);

			switch (type)
			{
				case FileType_File:
				{
					len = strlen(file);

					if (!extLen || (len > extLen && equali(file[len - extLen], ext)))
						ArrayPushString(files, addedPath);
				}
				case FileType_Directory: GetFiles(addedPath, files, ext);
			}
		}
		while (next_file(dir, file, charsmax(file), type));

		close_dir(dir);
	}
}

SetPlayerModel(plr, const modelName[])
{
	set_pdata_string(plr, 1420, modelName, -1, 20);
	entity_set_model(plr, GetPlayerModelPath(modelName));
}

GetPlayerModelPath(const modelName[])
{
	static path[128];
	formatex(path, charsmax(path), "models/player/%s/%s.mdl", modelName, modelName);

	return path;
}

UpdateCharacterMenuExitOption(plr)
{
	new bool:enable = HasCharacter(plr);
	menu_setprop(CharacterMenu, MPROP_EXIT, enable ? MEXIT_ALL : MEXIT_NEVER);
}

bool:CacheDeveloperStatus(plr)
{
	new mode[2];
	get_user_info(plr, "dev", mode, charsmax(mode));
	Developer[plr] = mode[0] == '1';

	return IsDeveloper(plr);
}

/**
 * Resets player's character to invalid.
 * Call this when e.g. player is manually moved to spectator and no longer has character.
 */
ResetCharacter(plr)
{
	PlayerCharacter[plr] = -1;
}

/**
 * Checks whether player is developer .
 *
 * @note  Player's developer status must be cached prior via function CacheDeveloperStatus()
 */
bool:IsDeveloper(plr)
{
	return Developer[plr];
}

/**
 * Checks whether player has selected character.
 */
bool:HasCharacter(plr)
{
	return PlayerCharacter[plr] != -1;
}

/**
 * Gets player's character index.
 *
 * @return  Character index (-1 if character no selected)
 */
GetPlayerCharacter(plr)
{
	return PlayerCharacter[plr];
}

/**
 * Gets player character's form index.
 *
 * @return  Character index (-1 if character no selected)
 */
GetPlayerForm(plr)
{
	return PlayerForm[plr];
}

/**
 * Gets player character and form data.
 *
 * @param cData  (Optional) Character data
 * @param fData  (Optional) Form data
 */
GetPlayerCharacterData(plr, cData[CHARACTER_DATA] = {-1}, fData[FORM_DATA] = {-1})
{
	if (cData[0] != -1)
		ArrayGetArray(CharacterData, GetPlayerCharacter(plr), cData);

	if (fData[0] != -1)
		ArrayGetArray(cData[CHARACTER_FORMS], GetPlayerForm(plr), fData);
}
