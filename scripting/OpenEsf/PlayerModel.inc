#include <engine>
#include <fakemeta>

#define MAX_MODEL_NAME_LENGTH 64

new PlayerModel[MAX_PLAYERS + 1][MAX_MODEL_NAME_LENGTH];
new PlayerModelBody[MAX_PLAYERS + 1];

SetPlayerModel(player, const model[], bool:force = true)
{
    copy(PlayerModel[player], charsmax(PlayerModel[]), model);

    new form, chr = GetPlayerCharacter(player, form);
    new props[FormProperty]; props = GetFormProperty(chr, form);
    new body = PlayerModelBody[player] = CalculateModelBody(props[FP_MODEL_PARTS]);

    if (force)
        _SetPlayerModel(player, model, body);
}

UpdatePlayerModel(plr)
{
    if (PlayerModel[plr][0] != EOS)
        _SetPlayerModel(plr, PlayerModel[plr], PlayerModelBody[plr]);
}

ClearPlayerModel(plr)
{
    PlayerModel[plr][0] = EOS;
    PlayerModelBody[plr] = 0;
}

UpdatePlayerModelBody(player)
{
    new body = PlayerModelBody[player];

    if (body)
        entity_set_int(player, EV_INT_body, PlayerModelBody[player]);
}

stock GetPlayerModelPath(const model[])
{
    new path[MAX_PATH_LENGTH];
    formatex(path, charsmax(path), "models/player/%s/%s.mdl", model, model);

    return path;
}

stock _SetPlayerModel(plr, const model[], body = 0)
{
    set_pdata_string(plr, 1420, model, -1, 20);
    entity_set_model(plr, GetPlayerModelPath(model));
    entity_set_int(plr, EV_INT_body, body);
}