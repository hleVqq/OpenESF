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
    new chrsCount = GetchrsCount();

    for (new i; i < chrsCount; i++)
    {
        ArrayGetArray(CharactersProperties, i, chrProps);
        
        new Array:forms = chrProps[CP_FORMS];
        new formsCount = ArraySize(forms);

        for (new j; j < formsCount; j++)
        {
            ArrayGetArray(forms, j, formProps);
            server_print("[OpenESF] Precaching player model %s", formProps[FP_MODEL]);
            precache_model(GetPlayerModelPath(formProps[FP_MODEL]));
        }
    }
}

GetchrsCount()
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

    new JSON:cObj = json_parse(path, true);

    new chrProps[CharacterProperty];
    json_object_get_string(cObj, "name", chrProps[CP_NAME], charsmax(chrProps[CP_NAME]));
    chrProps[CP_FORMS] = ArrayCreate(FormProperty);

    new JSON:formsArray = json_object_get_value(cObj, "forms");
    new formsCount = json_array_get_count(formsArray);
    new formProps[FormProperty];

    for (new i; i < formsCount; i++)
    {
        new JSON:fObj = json_array_get_value(formsArray, i);
        json_object_get_string(fObj, "name", formProps[FP_NAME], charsmax(formProps[FP_NAME]));
        json_object_get_string(fObj, "model", formProps[FP_MODEL], charsmax(formProps[FP_MODEL]));
        formProps[FP_MODEL_PARTS] = ArrayCreate();

        new JSON:modelPartsArray = json_object_get_value(fObj, "modelParts");
        new modelPartsCount = json_array_get_count(modelPartsArray);

        for (new j; j < modelPartsCount; j++)
            ArrayPushCell(formProps[FP_MODEL_PARTS], json_array_get_number(modelPartsArray, j));

        ArrayPushArray(chrProps[CP_FORMS], formProps);
    }

    ArrayPushArray(CharactersProperties, chrProps);
}

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