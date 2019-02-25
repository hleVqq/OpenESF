#define INVALID_CHARACTER -1

enum _:CharacterProperty
{
	CHARACTER_NAME[32],
	Array:CHARACTER_FORM_PROPERTIES
};

enum _:FormProperty
{
	FORM_NAME[32],
	FORM_PLAYER_MODEL_NAME[32]
};

static Array:CharacterProperties = Invalid_Array;
static PlayerCharacter[MAX_PLAYERS + 1] = {INVALID_CHARACTER, ...};
static PlayerForm[MAX_PLAYERS + 1];

LoadCharacters()
{
	if (CharacterProperties != Invalid_Array)
	{
		log_error(AMX_ERR_GENERAL, "Characters already loaded");

		return;
	}

	CharacterProperties = ArrayCreate(CharacterProperty);

	new goku = AddCharacter("Goku");
	AddForm(goku, "Base", "OpenGoku");

	new vegeta = AddCharacter("Vegeta");
	AddForm(vegeta, "Base", "OpenVegeta");
}

GetPlayerCharacter(plr)
{
	return PlayerCharacter[plr];
}

bool:PlayerHasCharacter(plr)
{
	return PlayerCharacter[plr] != INVALID_CHARACTER;
}

SetPlayerCharacter(plr, chr)
{
	PlayerCharacter[plr] = chr;
	PlayerForm[plr] = 0;

	engclient_cmd(plr, "goku");

	if (!is_user_alive(plr))
		RefreshPlayerModel(plr);
}

ClearPlayerCharacter(plr)
{
	PlayerCharacter[plr] = INVALID_CHARACTER;
}

GetCharacterCount()
{
	return ArraySize(CharacterProperties);
}

GetCharacterProperties(chr, chrProps[CharacterProperty])
{
	return ArrayGetArray(CharacterProperties, chr, chrProps);
}

GetFormProperties(chr, form, formProps[FormProperty])
{
	new chrProps[CharacterProperty];
	ArrayGetArray(CharacterProperties, chr, chrProps);
	ArrayGetArray(chrProps[CHARACTER_FORM_PROPERTIES], form, formProps);
}

ApplyPlayerCharacterProperties(plr)
{
	new formProps[FormProperty];
	GetFormProperties(PlayerCharacter[plr], PlayerForm[plr], formProps);
	SetPlayerModel(plr, formProps[FORM_PLAYER_MODEL_NAME]);
}

static AddCharacter(const name[])
{
	new props[CharacterProperty];
	copy(props[CHARACTER_NAME], charsmax(props[CHARACTER_NAME]), name);
	props[CHARACTER_FORM_PROPERTIES] = ArrayCreate(FormProperty);
	ArrayPushArray(CharacterProperties, props);

	return ArraySize(CharacterProperties) - 1;
}

static AddForm(chr, const name[], const playerModelName[])
{
	new chrProps[CharacterProperty];
	ArrayGetArray(CharacterProperties, chr, chrProps);

	new formProps[FormProperty];
	copy(formProps[FORM_NAME], charsmax(formProps[FORM_NAME]), name);
	copy(formProps[FORM_PLAYER_MODEL_NAME], charsmax(formProps[FORM_PLAYER_MODEL_NAME]), playerModelName);
	ArrayPushArray(chrProps[CHARACTER_FORM_PROPERTIES], formProps);
	precache_model(GetPlayerModelPath(playerModelName));

	return ArraySize(chrProps[CHARACTER_FORM_PROPERTIES]) - 1;
}
