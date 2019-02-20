#include <amxmodx>
#include <engine>
#include <fakemeta>
#include <hamsandwich>

#define PLUGIN "OpenESF"
#define VERSION "1.0"
#define AUTHOR "hleV"

#define MAX_CHARACTER_NAME_LENGTH 64
#define MAX_FORM_NAME_LENGTH 32
#define MAX_PATH_LENGTH 128

#define INVALID_CHARACTER -1
#define BASE_FORM 0

enum _:CHARACTER
{
    CHARACTER_NAME[MAX_CHARACTER_NAME_LENGTH],
    Array:CHARACTER_FORMS
};

enum _:CHARACTER_FORM
{
    CF_NAME[MAX_FORM_NAME_LENGTH],
    CF_MODEL_NAME[MAX_PATH_LENGTH]
};

new const BASE_CLASS_COMMAND[] = "goku";

new Array:Characters;
new PlayerCharacter[MAX_PLAYERS + 1];
new PlayerForm[MAX_PLAYERS + 1];

new CHARACTER_GOKU = INVALID_CHARACTER; // TEST
new CHARACTER_VEGETA = INVALID_CHARACTER; // TEST

public plugin_precache()
{
    SetupCharacters();
}

public plugin_init()
{
    register_plugin(PLUGIN, VERSION, AUTHOR);

    register_message(get_user_msgid("VGUIMenu"), "@OnVguiMenu");

    RegisterHam(Ham_Spawn, "player", "@OnPlayerSpawned", true);
}

public client_disconnected(plr)
{
    PlayerCharacter[plr] = INVALID_CHARACTER;
}

public client_infochanged(plr)
{
    if (PlayerCharacter[plr] != INVALID_CHARACTER)
        RefreshPlayerModel(plr);
}

@OnVguiMenu(msg, dest, ent)
{
    if (get_msg_arg_int(0) == 3)
    {
        SetPlayerCharacter(ent, CHARACTER_GOKU); // TEST

        return PLUGIN_HANDLED;
    }

    return PLUGIN_CONTINUE;
}

@OnPlayerSpawned(plr)
{
    if (!is_user_alive(plr))
        return;
    
    RefreshPlayerModel(plr);
}

SetupCharacters()
{
    Characters = ArrayCreate(CHARACTER);

    CHARACTER_GOKU = CreateCharacter("Goku"); // TEST
    AddForm(CHARACTER_GOKU, "Base", "OpenGoku"); // TEST

    CHARACTER_VEGETA = CreateCharacter("Vegeta"); // TEST
    AddForm(CHARACTER_VEGETA, "Base", "OpenVegeta"); // TEST
}

CreateCharacter(const name[])
{
    new character[CHARACTER];
    copy(character[CHARACTER_NAME], charsmax(character[CHARACTER_NAME]), name);
    character[CHARACTER_FORMS] = ArrayCreate(CHARACTER_FORM);
    ArrayPushArray(Characters, character);

    return ArraySize(Characters) - 1;
}

AddForm(index, const name[], const modelName[])
{
    new character[CHARACTER];
    ArrayGetArray(Characters, index, character);

    new form[CHARACTER_FORM];
    copy(form[CF_NAME], charsmax(form[CF_NAME]), name);
    copy(form[CF_MODEL_NAME], charsmax(form[CF_MODEL_NAME]), modelName);
    ArrayPushArray(character[CHARACTER_FORMS], form);

    precache_model(GetPlayerModelPath(modelName));

    return ArraySize(character[CHARACTER_FORMS]) - 1;
}

SetPlayerCharacter(plr, character)
{
    PlayerCharacter[plr] = character;
    PlayerForm[plr] = BASE_FORM;
    engclient_cmd(plr, BASE_CLASS_COMMAND);
}

RefreshPlayerModel(plr)
{
    new character[CHARACTER], form[CHARACTER_FORM];
    ArrayGetArray(Characters, PlayerCharacter[plr], character);
    ArrayGetArray(character[CHARACTER_FORMS], PlayerForm[plr], form);
    SetPlayerModel(plr, form[CF_MODEL_NAME]);
}

SetPlayerModel(plr, const modelName[])
{
    set_pdata_string(plr, 1420, modelName, -1, 20);
    entity_set_model(plr, GetPlayerModelPath(modelName));
}

GetPlayerModelPath(const modelName[])
{
    new path[MAX_PATH_LENGTH];
    formatex(path, charsmax(path), "models/player/%s/%s.mdl", modelName, modelName);

    return path;
}
