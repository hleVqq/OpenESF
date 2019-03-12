#define OVERRIDE(%1) state %1:Override
#define VIRTUAL:%1(%2) %1(%2) <%1:Virtual>

/*
 * Called when class menu is about to open for player.
 *
 * @return  PLUGIN_HANDLED to block original class menu
 */
VIRTUAL:OnClassMenu(player)
{
    #pragma unused player
    return PLUGIN_CONTINUE;
}

#undef VIRTUAL
#define VIRTUAL:%1(%2) %1(%2) <%1:Override>