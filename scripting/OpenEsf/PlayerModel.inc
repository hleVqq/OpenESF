#include <engine>
#include <fakemeta>

GetPlayerModelPath(const model[])
{
    new path[MAX_PATH_LENGTH];
    formatex(path, charsmax(path), "models/player/%s/%s.mdl", model, model);

    return path;
}

SetPlayerModel(plr, const model[], body = 0)
{
    set_pdata_string(plr, 1420, model, -1, 20);
    entity_set_model(plr, GetPlayerModelPath(model));
    entity_set_int(plr, EV_INT_body, body);
}