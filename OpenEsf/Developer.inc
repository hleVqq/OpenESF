static bool:IsDeveloper[MAX_PLAYERS + 1];

CacheDeveloperStatus(plr)
{
	new value[3];
	get_user_info(plr, "dev", value, charsmax(value));

	IsDeveloper[plr] = bool:equal(value, "1");
}

DevLog(plr, type, const fmt[], ...)
{
	if (!IsDeveloper[plr])
		return;
	
	new msg[192];
	vformat(msg, charsmax(msg), fmt, 4);
	client_print(plr, type, "[OpenESF] %s", msg);
}
