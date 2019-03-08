new PlayerCharacter[MAX_PLAYERS + 1] = {INVALID_CHARACTER, ...};
new PlayerForm[MAX_PLAYERS + 1] = {INITIAL_FORM, ...};

SetPlayerCharacter(plr, character, const baseClassCommand[] = "goku")
{
    PlayerCharacter[plr] = character;
    PlayerForm[plr] = INITIAL_FORM;

    new form, chr = GetPlayerCharacter(plr, form);
    new props[FormProperty]; props = GetFormProperty(chr, form);
    SetPlayerModel(plr, props[FP_MODEL], false);

    if (!is_user_alive(plr))
        engclient_cmd(plr, baseClassCommand);
}

ClearPlayerCharacter(plr)
{
    PlayerCharacter[plr] = INVALID_CHARACTER;
    ClearPlayerModel(plr);
}

GetPlayerCharacter(plr, &form = -1)
{
    form = PlayerForm[plr];

    return PlayerCharacter[plr];
}

GetPlayerForm(plr)
{
    return PlayerForm[plr];
}

bool:HasPlayerCharacter(plr)
{
    return PlayerCharacter[plr] != INVALID_CHARACTER;
}