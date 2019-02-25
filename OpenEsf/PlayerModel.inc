static PlayerModelName[MAX_PLAYERS + 1][32];

RefreshPlayerModel(plr)
{
	if (PlayerModelName[plr][0] != EOS)
		_SetPlayerModel(plr, PlayerModelName[plr]);
}

SetPlayerModel(plr, const modelName[])
{
	copy(PlayerModelName[plr], charsmax(PlayerModelName[]), modelName);
	_SetPlayerModel(plr, modelName);
}

GetPlayerModelPath(const modelName[])
{
	new path[128];
	formatex(path, charsmax(path), "models/player/%s/%s.mdl", modelName, modelName);

	return path;
}

static _SetPlayerModel(plr, const modelName[])
{
	DevLog(plr, print_chat, "Setting player model...");
	set_pdata_string(plr, 1420, modelName, -1, 20);
	entity_set_model(plr, GetPlayerModelPath(modelName));
}
