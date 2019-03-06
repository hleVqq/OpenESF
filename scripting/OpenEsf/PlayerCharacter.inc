new PlayerCharacter[MAX_PLAYERS + 1] = {INVALID_CHARACTER, ...};
new PlayerForm[MAX_PLAYERS + 1] = {INITIAL_FORM, ...};

SetPlayerCharacter(plr, character, const baseClassCommand[] = "goku")
{
    PlayerCharacter[plr] = character;
    PlayerForm[plr] = INITIAL_FORM;

    if (!is_user_alive(plr))
        engclient_cmd(plr, baseClassCommand);
}

ClearPlayerCharacter(plr)
{
    PlayerCharacter[plr] = INVALID_CHARACTER;
}

GetPlayerCharacter(plr)
{
    return PlayerCharacter[plr];
}

UpdatePlayerModel(plr)
{
    new chr = PlayerCharacter[plr];
    new form = PlayerForm[plr];

    SetPlayerModel(plr, GetFormProperty(chr, form, FP_MODEL));
}