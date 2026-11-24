#include "SystemUtils.h"

#include <algorithm>
#include <windows.h>
#include <shlobj.h>

namespace utils
{

std::optional<std::string> SelectFolder(const std::string & _PreferredPath)
{
  HRESULT Hresult = CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
  if (FAILED(Hresult))
    return std::nullopt;

  IFileDialog * FileDialog = nullptr;

  Hresult = CoCreateInstance(
      CLSID_FileOpenDialog,
      NULL,
      CLSCTX_ALL,
      IID_IFileOpenDialog,
      reinterpret_cast<void**>(&FileDialog)
    );

  if (FAILED(Hresult))
    return std::nullopt;

  DWORD Options = 0;
  FileDialog->GetOptions(&Options);
  FileDialog->SetOptions(Options | FOS_PICKFOLDERS);


  PIDLIST_ABSOLUTE IdList;

  std::wstring InitialDir(_PreferredPath.begin(), _PreferredPath.end());
  SHParseDisplayName(InitialDir.c_str(), 0, &IdList, SFGAO_FOLDER, 0);

  IShellItem * FolderItem = nullptr;
  SHCreateShellItem(NULL, NULL, IdList, &FolderItem);
  ILFree(IdList);

  FileDialog->SetFolder(FolderItem);

  if (FAILED(FileDialog->Show(nullptr)))
    return std::nullopt;

  IShellItem * ResultItem = nullptr;
  Hresult = FileDialog->GetResult(&ResultItem);
  if (FAILED(Hresult))
    return std::nullopt;

  PWSTR pszFilePath;
  Hresult = ResultItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

  if (FAILED(Hresult))
    return std::nullopt;


  std::wstring WideResult(pszFilePath);
  std::string  FolderPathResult(WideResult.length(), 0);

  if (!WideResult.empty())
    std::transform(WideResult.begin(), WideResult.end(), FolderPathResult.begin(), [](wchar_t c) { return char(c); });

  CoTaskMemFree(pszFilePath);
  ResultItem->Release();
  FileDialog->Release();
  CoUninitialize();

  return FolderPathResult;
}

std::filesystem::path GetExecDirectory()
{
  wchar_t Buffer[MAX_PATH];

  GetModuleFileNameW(nullptr, Buffer, MAX_PATH);

  return std::filesystem::path(Buffer).parent_path();
}

} // namespace utils