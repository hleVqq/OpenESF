#if defined GAMEFIXES_INCLUDED
    #endinput
#endif
#define GAMEFIXES_INCLUDED

/**
 * Fixes game bug where player's dead flag is DEAD_NO upon connecting. 
 * Without this, is_user_alive() will return 1 after player has been put into server but hasn't spawned yet.
 *
 * @note  Call on client_putinserver()
 */
FixPlayerConnectDeadFlag(plr)
{
    entity_set_int(plr, EV_INT_deadflag, DEAD_DEAD);
}