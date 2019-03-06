#if defined FILE_INCLUDED
    #endinput
#endif
#define FILE_INCLUDED

#define MAX_PATH_LENGTH 256
#define MAX_FILE_LENGTH 64

/**
 * Scans all files and directories recursively and gathers files with given extension.
 *
 * @note         @param path should not be constant due to mistake in AMXX includes
 * 
 * @param path   Path where to start the scan
 * @param ext    (Optional) File extension to search for, e.g. ".json"
 * @param files  (Optional) Array to store file paths in
 *
 * @return       Array with stored file paths. Use ArraySize() to find file count
 */
Array:ScanFiles(path[], const ext[] = "", Array:files = Invalid_Array)
{
    new file[MAX_FILE_LENGTH], FileType:type;
    new dir = open_dir(path, file, charsmax(file), type);

    if (dir)
    {
        if (files == Invalid_Array)
            files = ArrayCreate(MAX_PATH_LENGTH);
        
        new newPath[MAX_PATH_LENGTH];
        new extLen = strlen(ext);
        
        do
        {
            if (equal(file, ".") || equal(file, ".."))
                continue;
            
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