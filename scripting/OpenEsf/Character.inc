#include <json>

#define INVALID_CHARACTER -1
#define INITIAL_FORM 0

enum _:CharacterProperty
{
    CP_NAME[32],
    Array:CP_FORMS
};

enum _:FormProperty
{
    FP_NAME[32],
    FP_MODEL[32],
    Array:FP_MODEL_PARTS
}

new Array:CharactersProperties = Invalid_Array;

InitializeCharacters()
{
    LoadCharacters();
    PrecacheCharacterFiles();
}

LoadCharacters()
{
    CharactersProperties = ArrayCreate(CharacterProperty);
    
    new Array:files = ScanCharacterFiles();
    new count = ArraySize(files);
    new path[MAX_PATH_LENGTH];

    for (new i; i < count; i++)
    {
        ArrayGetString(files, i, path, charsmax(path));
        ParseCharacterFile(path);
    }

    ArrayDestroy(files);
}

PrecacheCharacterFiles()
{
    new chrProps[CharacterProperty], formProps[FormProperty];
    new chrsCount = GetCharactersCount();

    for (new i; i < chrsCount; i++)
    {
        ArrayGetArray(CharactersProperties, i, chrProps);
        
        new Array:forms = chrProps[CP_FORMS];
        new formsCount = ArraySize(forms);

        for (new j; j < formsCount; j++)
        {
            ArrayGetArray(forms, j, formProps);
            precache_model(GetPlayerModelPath(formProps[FP_MODEL]));
        }
    }
}

GetCharactersCount()
{
    return ArraySize(CharactersProperties);
}

Array:ScanCharacterFiles()
{
    new path[128];
    get_localinfo("amxx_datadir", path, charsmax(path));
    add(path, charsmax(path), "/OpenEsf/Characters");

    return ScanFiles(path, ".json");
}

ParseCharacterFile(const path[])
{
    server_print("[OpenESF] Parsing character %s", path);

    new JSON:chrJson = json_parse(path, true);

    new chrProps[CharacterProperty];
    json_object_get_string(chrJson, "name", chrProps[CP_NAME], charsmax(chrProps[CP_NAME]));
    chrProps[CP_FORMS] = ArrayCreate(FormProperty);

    new JSON:formsJson = json_object_get_value(chrJson, "forms");
    new formProps[FormProperty];
    new modelParts[2];

    for (new i, formsCount = json_array_get_count(formsJson); i < formsCount; i++)
    {
        new JSON:formJson = json_array_get_value(formsJson, i);
        json_object_get_string(formJson, "name", formProps[FP_NAME], charsmax(formProps[FP_NAME]));
        json_object_get_string(formJson, "model", formProps[FP_MODEL], charsmax(formProps[FP_MODEL]));
        formProps[FP_MODEL_PARTS] = ArrayCreate(2);

        new JSON:modelPartsJson = json_object_get_value(formJson, "modelParts");

        for (new j, modelPartsGroupsCount = json_array_get_count(modelPartsJson); j < modelPartsGroupsCount; j++)
        {
            new JSON:modelPartsArray = json_array_get_value(modelPartsJson, j);

            modelParts[MP_INDEX] = json_array_get_number(modelPartsArray, MP_INDEX);
            modelParts[MP_GROUPSIZE] = json_array_get_number(modelPartsArray, MP_GROUPSIZE);
            ArrayPushArray(formProps[FP_MODEL_PARTS], modelParts);
        }

        ArrayPushArray(chrProps[CP_FORMS], formProps);
    }

    ArrayPushArray(CharactersProperties, chrProps);
}

// Needs rework
GetFormProperty(chr, form, prop = -1)
{
    new chrProps[CharacterProperty], formProps[FormProperty];
    ArrayGetArray(CharactersProperties, chr, chrProps);
    ArrayGetArray(chrProps[CP_FORMS], form, formProps);

    switch (prop)
    {
        case FP_NAME: copy(formProps, charsmax(formProps), formProps[FP_NAME]);
        case FP_MODEL: copy(formProps, charsmax(formProps), formProps[FP_MODEL]);
    }

    return formProps;
}