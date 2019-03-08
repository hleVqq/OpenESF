enum _:ModelPart
{
    MP_INDEX,
    MP_GROUPSIZE
};

stock CalculateModelBody(Array:groups)
{
    new body = 0;
    new count = ArraySize(groups);
    new parts[ModelPart];

    while (count--)
    {
        ArrayGetArray(groups, count, parts);

        if (parts[MP_GROUPSIZE] == 1)
            continue;
        
        new temp = parts[MP_INDEX];

        for (new i = 0; i < count; i++)
        {
            ArrayGetArray(groups, i, parts);
            temp *= parts[MP_GROUPSIZE];
        }

        body += temp;
    }

    return body;
}