#if defined GAMEFIXES_INCLUDED
    #endinput
#endif
#define GAMEFIXES_INCLUDED

FixPlayerConnectDeadFlag(plr)
{
    entity_set_int(plr, EV_INT_deadflag, DEAD_DEAD);
}