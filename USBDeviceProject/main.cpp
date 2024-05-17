
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>

using namespace std;

wstring findDataFile(const wstring& fileName) {
    wchar_t szLogicalDrives[MAX_PATH];
    DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, szLogicalDrives);
    if (dwResult > 0 && dwResult <= MAX_PATH) {
        wchar_t* szSingleDrive = szLogicalDrives;
        while (*szSingleDrive) {
            try {
                UINT driveType = GetDriveType(szSingleDrive);
                if (driveType == DRIVE_FIXED) { // Check if the drive is removable (USB)
                    wstring driveRoot = szSingleDrive;
                    vector<wstring> directoriesToSearch;
                    directoriesToSearch.push_back(driveRoot);

                    while (!directoriesToSearch.empty()) {
                        wstring currentDir = directoriesToSearch.back();
                        directoriesToSearch.pop_back();

                        WIN32_FIND_DATA findFileData;
                        wstring searchPath = currentDir + L"*.*";
                        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

                        if (hFind != INVALID_HANDLE_VALUE) {
                            do {
                                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                                    if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
                                        wstring subDir = currentDir + findFileData.cFileName + L"\\";
                                        directoriesToSearch.push_back(subDir);
                                    }
                                }
                                else {
                                    if (wcscmp(findFileData.cFileName, fileName.c_str()) == 0) {
                                        return currentDir + fileName;
                                    }
                                }
                            } while (FindNextFile(hFind, &findFileData) != 0);
                            FindClose(hFind);
                        }
                    }
                }
            }
            catch (const exception& ex) {
                cerr << ex.what() << endl;
            }
            szSingleDrive += wcslen(szSingleDrive) + 1;
        }
    }
    else {
        throw runtime_error("Error: GetLogicalDriveStrings failed");
    }

    throw runtime_error("Error: Data file not found");
}

void simulateDataCollectionFromUSB() {
    wstring dataFileName = L"data.txt"; // Assuming data file is named "data.txt"
    wstring filePath = findDataFile(dataFileName);

    // Access and process the file at filePath
    while (true) {
        ifstream inputFile(filePath);
        if (inputFile.is_open()) {
            // Process the file data
            int data;
            while (inputFile >> data) {
                cout << "Received data from USB: " << data << endl;
            }
            inputFile.close();
        }
        else {
            string errormsg = "Error: Unable to open file";
            errormsg += string(filePath.begin(), filePath.end());
            throw runtime_error(errormsg.c_str());
        }
        Sleep(1000); // Wait for 1 second before reading again
    }
}

int main() {
    try {
        simulateDataCollectionFromUSB();
    }
    catch (const exception& ex) {
        cerr << ex.what() << endl;
    }
    return 0;
}
