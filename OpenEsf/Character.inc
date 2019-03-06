#if defined CHARACTERS_INCLUDED
    #endinput
#endif
#define CHARACTER_INCLUDED

#include <json>

#include "OpenEsf/Files.inc"

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
    CharactersProperties = ArrayCreate(CharacterProperty);
    
    new Array:files = ScanCharacterFiles();
    new count = ArraySize(files);
    new path[256];

    for (new i; i < count; i++)
    {
        ArrayGetString(files, i, path, charsmax(path));
        ParseCharacterFile(path);
    }

    ArrayDestroy(files);
}

Array:ScanCharacterFiles()
{
    new path[128];
    get_localinfo("amxx_datadir", path, charsmax(path));
    add(path, charsmax(path), "/OpenEsf/Characters");

    return ScanFiles(path, "json");
}

ParseCharacterFile(const path[])
{
    new JSON:cObj = json_parse(path, true);

    new cProps[CharacterProperty];
    json_object_get_string(cObj, "name", cProps[CP_NAME], charsmax(cProps[CP_NAME]));
    cProps[CP_FORMS] = ArrayCreate(FormProperty);

    new JSON:formsArray = json_object_get_value(cObj, "forms");
    new formsCount = json_array_get_count(formsArray);
    new fProps[FormProperty];

    for (new i; i < formsCount; i++)
    {
        new JSON:fObj = json_array_get_value(formsArray, i);
        json_object_get_string(fObj, "name", fProps[FP_NAME], charsmax(fProps[FP_NAME]));
        json_object_get_string(fObj, "model", fProps[FP_MODEL], charsmax(fProps[FP_MODEL]));
        fProps[FP_MODEL_PARTS] = ArrayCreate();

        new JSON:modelPartsArray = json_object_get_value(fObj, "modelParts");
        new modelPartsCount = json_array_get_count(modelPartsArray);

        for (new j; j < modelPartsCount; j++)
            ArrayPushCell(fProps[FP_MODEL_PARTS], json_array_get_number(modelPartsArray, j));

        ArrayPushArray(cProps[CP_FORMS], fProps);
    }

    ArrayPushArray(CharactersProperties, cProps);
}