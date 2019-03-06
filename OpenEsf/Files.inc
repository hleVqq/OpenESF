#if defined FILES_INCLUDED
    #endinput
#endif
#define FILES_INCLUDED

Array:ScanFiles(path[], const ext[] = "", Array:files = Invalid_Array)
{
    new file[64], FileType:type;
    new dir = open_dir(path, file, charsmax(file), type);

    if (dir)
    {
        if (files == Invalid_Array)
            files = ArrayCreate(256);
        
        new newPath[256];
        new extLen = strlen(ext);
        
        do
        {
            formatex(newPath, charsmax(newPath), "%s/%s", path, file);

            switch (type)
            {
                case FileType_File:
                {
                    new len = strlen(file);

                    if (!extLen || (len > extLen && equali(ext, file[len - extLen])))
                        ArrayPushString(files, newPath);
                }
                case FileType_Directory: ScanFiles(newPath, ext, files);
            }
        }
        while (next_file(dir, file, charsmax(file), type));

        close_dir(dir);
    }

    return files;
}