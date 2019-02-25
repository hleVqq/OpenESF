#include <amxmodx>
#include <engine>
#include <fakemeta>
#include <hamsandwich>

#include "OpenEsf/Character.inc"
#include "OpenEsf/CharacterMenu.inc"
#include "OpenEsf/PlayerModel.inc"
#include "OpenEsf/Developer.inc"

public plugin_precache()
{
	LoadCharacters();
}

public plugin_init()
{
	register_plugin("OpenESF", "1.0", "hleV");

	register_message(get_user_msgid("VGUIMenu"), "@OnVguiMenu");

	RegisterHam(Ham_Spawn, "player", "@OnPlayerSpawn", true);

	CreateCharacterMenu();
}

public client_authorized(plr)
{
	CacheDeveloperStatus(plr);
}

public client_putinserver(plr)
{
	// Fix player being treated as alive upon connect
	entity_set_int(plr, EV_INT_deadflag, DEAD_DEAD);
}

public client_disconnected(plr)
{
	ClearPlayerCharacter(plr);
}

public client_infochanged(plr)
{
	if (!is_user_connected(plr))
		return;

	if (PlayerHasCharacter(plr))
		RefreshPlayerModel(plr);
	
	CacheDeveloperStatus(plr);
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

@OnPlayerSpawn(plr)
{
	if (!is_user_alive(plr))
		return;
	
	ApplyPlayerCharacterProperties(plr);
}

@OnClassMenu(plr)
{
	ShowCharacterMenu(plr);
}
