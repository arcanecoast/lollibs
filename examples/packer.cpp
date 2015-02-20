/****************************************************************************************//**

 Author of this file is
     __________ _______   __   _____ __ __________ _____ __ __ _____ ______   __ ____   
    / _   _   // ___  /__/ /_ / ___ / // _   _   // ___ / // //____// ____ \ / //___ \  
   / / / / / // /__/_//_  __// /  /  // / / / / // /  /  //   /    / /___/_//   /__/_/  
  / / / / / // /_____  / /_ / /__/  // / / / / // /__/  // /\ \__ / /_____ / /\ \__     
 /_/ /_/ /_/ \______/  \__/ \____/_//_/ /_/ /_/ \____/_//_/  \___/\______//_/  \___/    
                                                                                         
 webpage: http://www.goblinov.net
 email: guru@goblinov.net   

 This file is provided under certain circumstances. For more details see LICENSE file in
 the project's root folder.
  
 \author metamaker
 \brief 
 
 This is example of how to use classes and functions of lollibs/Package.h to work with Lands
 of Lore's .PAK format.
 
 The unit contains program's entry point and functions which correspond to user-level commands
 of application (like dump, pack, unpack, etc.) 
 
********************************************************************************************/

#include <lollibs/package.h>

#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace lollibs;

const int MAXIMUM_LENGTH_OF_FILE_IN_PACKAGE = 32 * 1024 * 1024; // 32 MB

void ShowHelp()
{
    char *helpline =
"Lands of Lore Packer Utility\n"
"----------------------------\n\n"
"Author:     metamaker\n"
"Build date: 16.02.2015\n"
"Version:    0.1\n"
"\n----------------------------\n\n"
"Usage: lolpack <command>\n\n"
"Available commands:\n\n"
"- dump <filename.dat> -- dump information about .DAT content into console\n"
"- unpack <filename.dat> -- unpack .DAT to current folder\n"
"- update file1 ... fileN <filename.dat> -- update files of .DAT with current folder's ones\n"
"- pack file1 ... fileN <filename.dat> -- pack files into single .DAT\n";

    cout << helpline << endl;
}

int DumpPackage(int argc, char **argv)
{
    // Check if there are exactly 3 arguments

    if (argc != 3) {
        ShowHelp();

        return -1;
    }

    // Load the package

    Package package;
    if (!package.LoadFromFile(argv[2])) {
        cout << "Error while loading .DAT package " << argv[2] << endl;

        return 2;
    }

    // Display dump of the package

    cout << "Dump of the package " << argv[2] << endl << endl;
    cout << left << setw(25) << "Filename" << setw(15) << "Size" << endl << endl;

    for (Package::iterator file = package.Begin(), end = package.End(); file != end; ++file) {
        cout << left << setw(25) << file->first << setw(15) << file->second.GetSize() << endl;
    }

    return 0;
}

bool CanFileBeOverwritten(const string& filename)
{
    // Try to open file

    ifstream in(filename);

    // If file does not exist then we can try to overwrite it ...

    if (!in.good()) {
        return true;
    }

    in.close();

    // ... Else ask user if file should be overwritten

    cout << "The " << filename << " file already exists on disk. Do you want to overwrite it? [y/n]";

    char answerChar;
    cin >> answerChar;

    // If answer is yes then file can be overwritten

    if (answerChar == 'y') {
        return true;
    }

    return false;
}

int UnpackPackage(int argc, char **argv)
{
    // Check if there are exactly 3 arguments

    if (argc > 3) {
        ShowHelp();

        return 1;
    }

    // Load the package

    Package package;

    if (!package.LoadFromFile(argv[2])) {
        cout << "Error while loading .DAT package " << argv[2] << endl;

        return 2;
    }

    // Extract every file from the package

    int amountOfUnpackedFiles = 0;

    for (Package::iterator file = package.Begin(), endFile = package.End(); file != endFile; ++file) {
        // Check if file with a same name is already present on disk and if it should be overwritten

        if (CanFileBeOverwritten(file->first) == false) {
            continue;
        }
        
        // Write file contents on disk
        
        ofstream out(file->first, ios_base::out | ios_base::binary);

        if (!out.good()) {
            cout << "Error: unable to write the " << file->first << " file on disk!" << endl;

            continue;
        }

        out.write(file->second.GetContent(), file->second.GetSize());

        ++amountOfUnpackedFiles;
    }

    if (amountOfUnpackedFiles == 0) {
        cout << "Program successfully completed but no files were unpacked!" << endl;
    } else if (amountOfUnpackedFiles == 1) {
        cout << "Program successfully unpacked 1 file!" << endl;
    } else {
        cout << "Program successfully unpacked " << amountOfUnpackedFiles << " files!" << endl;
    }

    return 0;
}

int UpdateFileInPackage(int argc, char **argv)
{
    // Load existing package

    Package package;
    
    if (!package.LoadFromFile(argv[argc - 1])) {
        cout << "Error while loading .DAT package " << argv[argc - 1] << endl;

        return 2;
    }

    // Load every specified file and update it in the package

    for (int fileIndex = 2, fileEnd = argc - 1; fileIndex != fileEnd; ++fileIndex) {
        const char *filename = argv[fileIndex];

        ifstream in(filename, ios_base::in | ios_base::binary | ios_base::ate);

        // If file doesn't exist then show warning and continue

        if (!in.good()) {
            cout << "Warning: the " << filename << " file doesn't exist. No action is performed!" << endl;

            continue;
        }

        // If file is too large then do nothing

        unsigned long fileLength = static_cast<unsigned long>(in.tellg());

        if (fileLength > MAXIMUM_LENGTH_OF_FILE_IN_PACKAGE) {
            cout << "Warning: the " << filename << " file is too large to be inserted into a package (maximum file size must be no more than 32 MB). No action is performed!" << endl;

            continue;
        }

        in.seekg(0);

        // Read file's content and update it in a package

        char *fileData = new char[fileLength];
        in.read(fileData, fileLength);

        package.Update(filename, PackageItemData(fileData, fileLength));
    }

    // Save the package

    package.SaveToFile(argv[argc - 1]);

    return 0;
}

int PackPackage(int argc, char **argv)
{
    // Check if package with a same name should be overwritten (if it already exists)

    if (!CanFileBeOverwritten(argv[argc - 1])) {
        return 0;
    }

    // Create new package

    Package package;

    // Load every specified file and insert it into the package

    for (int fileIndex = 2, fileEnd = argc - 1; fileIndex != fileEnd; ++fileIndex) {
        const char *filename = argv[fileIndex];

        ifstream in(filename, ios_base::in | ios_base::binary | ios_base::ate);

        // If file doesn't exist then show warning and continue

        if (!in.good()) {
            cout << "Warning: the " << filename << " file doesn't exist. No action is performed!" << endl;

            continue;
        }

        // If file is too large then do nothing

        unsigned long fileLength = static_cast<unsigned long>(in.tellg());

        if (fileLength > MAXIMUM_LENGTH_OF_FILE_IN_PACKAGE) {
            cout << "Warning: the " << filename << " file is too large to be inserted into a package (maximum file size must be no more than 32 MB). No action is performed!" << endl;

            continue;
        }

        in.seekg(0);

        // Read file's content and insert it into a package

        char *fileData = new char[fileLength];
        in.read(fileData, fileLength);

        package.Insert(filename, PackageItemData(fileData, fileLength));
    }

    // Save the package

    package.SaveToFile(argv[argc - 1]);

    return 0;
}

int main(int argc, char **argv)
{
    // Check if there is specified necessary amount of arguments

    if (argc < 2) {
        ShowHelp();

        return 1;
    }

    // Choose the way of a work flow depending on specified command

    if (strcmp(argv[1], "dump") == 0) {
        return DumpPackage(argc, argv);
    } else if (strcmp(argv[1], "unpack") == 0) {
        return UnpackPackage(argc, argv);
    } else if (strcmp(argv[1], "update") == 0) {
        return UpdateFileInPackage(argc, argv);
    } else if (strcmp(argv[1], "pack") == 0) {
        return PackPackage(argc, argv);
    } else {
        ShowHelp();

        return 2;
    }

    return 0;
}
