#include <amxmodx>
#include <engine>

#include "OpenEsf/GameFixes.inc"
#include "OpenEsf/File.inc"
#include "OpenEsf/Model.inc"
#include "OpenEsf/Character.inc"
#include "OpenEsf/PlayerModel.inc"
#include "OpenEsf/PlayerCharacter.inc"
#include "OpenEsf/Overridables.inc"

#tryinclude "OpenEsf/Overrides.inc"

public plugin_precache()
{
    InitializeCharacters();
}

public plugin_init()
{
    register_plugin("OpenESF", "0.2", "hleV");

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
    UpdatePlayerModel(plr);
}

public client_PostThink(plr)
{
    UpdatePlayerModelBody(plr);
}

@OnVguiMenu(msg, dest, ent)
{
    new menu = get_msg_arg_int(1);

    if (menu == 3)
    {
        if (OnClassMenu(ent) == PLUGIN_HANDLED)
            return PLUGIN_HANDLED;
    }

    return PLUGIN_CONTINUE;
}

OnClassMenu(plr) <>
{
    RequestFrame("@OnClassMenu", plr);

    return PLUGIN_HANDLED;
}

@OnClassMenu(plr)
{
    SetPlayerCharacter(plr, 0, "goku"); // PLACEHOLDER, automatically chooses first character for player
}