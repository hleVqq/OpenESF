#if defined PLAYERMODEL_INCLUDED
    #endinput
#endif
#define PLAYERMODEL_INCLUDED

GetPlayerModelPath(const model[])
{
    new path[MAX_PATH_LENGTH];
    formatex(path, charsmax(path), "models/player/%s/%s.mdl", model, model);

    return path;
}