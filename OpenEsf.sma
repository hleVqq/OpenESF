#include <amxmodx>
#include <engine>

#include "OpenEsf/GameFixes.inc"
#include "OpenEsf/File.inc"
#include "OpenEsf/PlayerModel.inc"
#include "OpenEsf/Character.inc"
#include "OpenEsf/PlayerCharacter.inc"

public plugin_precache()
{
    InitializeCharacters();
}

public plugin_init()
{
    register_plugin("OpenESF", "0.1", "hleV");

    register_message(get_user_msgid("VGUIMenu"), "@OnVguiMenu");
}

public client_putinserver(plr)
{
    FixPlayerConnectDeadFlag(plr);
}

public client_disconnected(plr)
{
    ClearPlayerCharacter(plr);
}

public client_infochanged(plr)
{
    if (GetPlayerCharacter(plr) != INVALID_CHARACTER)
        UpdatePlayerModel(plr);
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

@OnClassMenu(plr)
{
    SetPlayerCharacter(plr, 0); // PLACEHOLDER
}