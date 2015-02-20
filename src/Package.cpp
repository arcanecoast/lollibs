#include <lollibs/Package.h>

#include <fstream>
#include <vector>

using namespace std;
using namespace lollibs;

bool Package::LoadFromFile(const char *path)
{
    // Clear junk

    Clear();

    // Open package file

    ifstream in(path, ios_base::in | ios_base::binary | ios_base::ate);

    if (!in.good()) {
        return false;
    }

    unsigned long fileLength = static_cast<unsigned long>(in.tellg());
    in.seekg(0);

    vector<PackageItemOnDiskHeader> files;

    // Read package's header

    do {
        PackageItemOnDiskHeader item;

        // Read offset to raw data for current file in package

        in.read(reinterpret_cast<char*>(&item.offsetToData), 4);

        if (item.offsetToData == fileLength) {
            break;
        }

        if (!in.good()) {
            return false;
        }

        // Read name of current file

        char c;

        while (true) {
            in.read(&c, 1);

            if (!in.good()) {
                return false;
            }

            if (c == '\0') {
                break;
            }

            item.resourceName += c;
        }

        // Add file's header to a list of read headers

        files.push_back(item);
    } while (true);

    // Read raw content for files

    for (vector<PackageItemOnDiskHeader>::iterator file = files.begin(), endFile = files.end(); file != endFile; ++file) {
        in.seekg(file->offsetToData);

        // Calculate size of file's data

        unsigned long dataSize;
        if ((file+1) == endFile) {
            dataSize = fileLength - file->offsetToData;
        } else {
            dataSize = (file+1)->offsetToData - file->offsetToData;
        }

        // Read content

        char *content = new char[dataSize];
        in.read(content, dataSize);

        // Create PackageItem object and insert into Package

        Insert(file->resourceName, PackageItemData(content, dataSize));

        if (!in.good()) {
            return false;
        }
    }

    return true;
}

void Package::SaveToFile(const char *path) const
{
    // Open file stream to write in

    fstream out(path, ios_base::out | ios_base::binary | ios_base::trunc);

    if (!out.good()) {
        return;
    }

    // Calculate offset to raw data

    const unsigned long SIZE_OF_OFFSET = 4;
    unsigned long offsetToDataStart = (SIZE_OF_OFFSET + 1) * 2;
    unsigned long fileExpectedLength = 0;

    for (Package::const_iterator file = Begin(), fileEnd = End(); file != fileEnd; ++file) {
        offsetToDataStart += SIZE_OF_OFFSET + static_cast<unsigned long>(file->first.length()) + 1 /*for \0 symbol*/;
        fileExpectedLength += file->second.GetSize();
    }

    fileExpectedLength += offsetToDataStart;

    // Write header

    unsigned long offsetToDataCurrentFile = offsetToDataStart;

    for (Package::const_iterator file = Begin(), fileEnd = End(); file != fileEnd; ++file) {
        out.write(reinterpret_cast<char*>(&offsetToDataCurrentFile), 4);
        out.write(file->first.c_str(), file->first.length() + 1);

        offsetToDataCurrentFile += file->second.GetSize();
    }

    // Write two last records (first has offset to the end of file and second is null-only record)

    out.write(reinterpret_cast<char*>(&fileExpectedLength), 4);
    out.write("\0", 1);

    const char nullOffset[] = {'\0', '\0', '\0', '\0'};
    out.write(nullOffset, 4);
    out.write("\0", 1);

    // Write raw data

    for (Package::const_iterator file = Begin(), fileEnd = End(); file != fileEnd; ++file) {
        out.write(file->second.GetContent(), file->second.GetSize());
    }
}

Package::iterator Package::Find(const string& name)
{
    return m_items.find(name);
}

Package::const_iterator Package::Find(const string& name) const
{
    return m_items.find(name);
}

Package::iterator Package::Begin()
{
    return m_items.begin();
}

Package::const_iterator Package::Begin() const
{
    return m_items.begin();
}

Package::iterator Package::End()
{
    return m_items.end();
}

Package::const_iterator Package::End() const
{
    return m_items.end();
}

bool Package::Insert(const string& name, const PackageItemData& data)
{
    pair<Package::iterator, bool> operationInfo = m_items.insert(pair<string, PackageItemData>(name, data));

    return operationInfo.second;
}

void Package::Update(const string& name, const PackageItemData& data)
{
    m_items[name] = data;
}

void Package::Delete(const string& name)
{
    m_items.erase(name);
}

void Package::Clear()
{
    m_items.clear();
}

size_t Package::Size()
{
    return m_items.size();
}