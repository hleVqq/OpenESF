#include <amxmodx>
#include <engine>

#include "OpenEsf/GameFixes.inc"
#include "OpenEsf/Character.inc"

public plugin_precache()
{
    InitializeCharacters();
}

public client_putinserver(plr)
{
    FixPlayerConnectDeadFlag(plr);
}