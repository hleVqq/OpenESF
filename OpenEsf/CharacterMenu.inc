static CharacterMenu;

CreateCharacterMenu()
{
	if (CharacterMenu)
	{
		log_error(AMX_ERR_GENERAL, "Character menu already created");

		return;
	}

	CharacterMenu = menu_create("Character Menu", "@OnCharacterMenu");

	new count = GetCharacterCount();
	new chrProps[CharacterProperty];

	for (new i; i < count; i++)
	{
		GetCharacterProperties(i, chrProps);
		menu_additem(CharacterMenu, chrProps[CHARACTER_NAME]);
	}

	menu_setprop(CharacterMenu, MPROP_PAGE_CALLBACK, "@OnCharacterMenuPage");
}

ShowCharacterMenu(plr)
{
	menu_display(plr, CharacterMenu);
}

static @OnCharacterMenu(plr, menu, item)
{
	if (item < 0)
		return;
	
	SetPlayerCharacter(plr, item);
}

static @OnCharacterMenuPage(plr, status)
{
	menu_setprop(CharacterMenu, MPROP_EXIT, PlayerHasCharacter(plr) ? MEXIT_ALL : MEXIT_NEVER);
}
