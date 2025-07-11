#include "folder_management.h"


void BrowseForFolder(struct Master_Directory* ptr, int create_or_move)
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Select a folder");
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != NULL)
    {
        if (create_or_move == 0) {
        
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                _tprintf(_T("Folder selected: %s\n"), path);
                _tcscpy_s(ptr->master_folder, MAX_PATH, path);
            }
            CoTaskMemFree(pidl);
        }
        else 
        {
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                _tprintf(_T("Folder selected: %s\n"), path);
                _tcscpy_s(ptr->path_to_media_import, MAX_PATH, path);
            }
            CoTaskMemFree(pidl);
        }
    }
}
