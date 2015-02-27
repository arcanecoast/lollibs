#include <lollibs/FiledataIndex.h>

#include <fstream>
#include <vector>

using namespace std;
using namespace lollibs;

bool FileDataIndex::LoadFromFile(const char *path)
{
    // Clear junk

    Clear();

    // Open .FDT index file

    ifstream in(path, ios_base::in | ios_base::binary | ios_base::ate);

    if (!in.good()) {
        return false;
    }
    
    unsigned long indexFileSize = static_cast<unsigned long>(in.tellg());
    in.seekg(0);

    vector<pair<unsigned long, FileDataIndexItem> > informationAboutItems; // First item of pair is offset to the filename and the second one is information about file
    unsigned long offsetToFilename;

    // Read index information about files

    do {
        FileDataIndexItem indexItemData;
        in.read(reinterpret_cast<char*>(&offsetToFilename), 4);
        in.read(reinterpret_cast<char*>(&indexItemData.m_size), 4);
        in.read(reinterpret_cast<char*>(&indexItemData.m_unknown), 4);
        in.read(reinterpret_cast<char*>(&indexItemData.m_offsetInPackage), 4);
        in.read(reinterpret_cast<char*>(&indexItemData.m_indexInPackage), 2);
        in.read(reinterpret_cast<char*>(&indexItemData.m_itemType), 2);

        if (offsetToFilename == 0) {
            break;
        }

        informationAboutItems.push_back(make_pair(offsetToFilename, indexItemData));
    } while (true);

    // Read filename of every file

    for (vector<pair<unsigned long, FileDataIndexItem> >::iterator currentItem = informationAboutItems.begin(),
        itemEnd = informationAboutItems.end(); currentItem != itemEnd; ++currentItem) {

        // Calculate length of filename

        unsigned long fileNameLength;
        if ((currentItem + 1) == itemEnd) {
            fileNameLength = indexFileSize - currentItem->first;
        } else {
            fileNameLength = (currentItem + 1)->first - currentItem->first;
        }

        // Read filename and add information about current file to this FileDataIndex object

        char *fileNameBuffer = new char[fileNameLength];
        in.read(fileNameBuffer, fileNameLength);

        string fileName = fileNameBuffer;
        delete[] fileNameBuffer;

        Insert(fileName, currentItem->second);
    }

    return true;
}

void FileDataIndex::SaveToFile(const char *path) const
{
    // Open file stream to write in

    fstream out(path, ios_base::out | ios_base::binary | ios_base::trunc);

    if (!out.good()) {
        return;
    }

    // Write primary information about files

    const unsigned long PRIMARY_INFORMATION_SIZE = 20; // In bytes
    unsigned long offsetToFilename = PRIMARY_INFORMATION_SIZE * (m_items.size() + 1);

    for (FileDataIndex::const_iterator item = m_items.begin(), itemEnd = m_items.end(); item != itemEnd; ++item) {
        out.write(reinterpret_cast<const char *>(&offsetToFilename), 4);
        out.write(reinterpret_cast<const char *>(&item->second.m_size), 4);
        out.write(reinterpret_cast<const char *>(&item->second.m_unknown), 4);
        out.write(reinterpret_cast<const char *>(&item->second.m_offsetInPackage), 4);
        out.write(reinterpret_cast<const char *>(&item->second.m_indexInPackage), 2);
        out.write(reinterpret_cast<const char *>(&item->second.m_itemType), 2);

        offsetToFilename += item->first.length() + 1;
    }

    // Write technical record

    const unsigned long endRecordAttributesValue = 0;
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 4); // Offset to filename is 0000
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 4); // Size is 0000
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 4); // and so on ...
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 4);
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 2);
    out.write(reinterpret_cast<const char *>(&endRecordAttributesValue), 2);

    // Write filenames

    for (FileDataIndex::const_iterator item = m_items.begin(), itemEnd = m_items.end(); item != itemEnd; ++item) {
        out.write(item->first.c_str(), item->first.length() + 1);
    }
}

FileDataIndex::iterator FileDataIndex::Find(const string& name)
{
    return m_items.find(name);
}

FileDataIndex::const_iterator FileDataIndex::Find(const string& name) const
{
    return m_items.find(name);
}

FileDataIndex::iterator FileDataIndex::Begin()
{
    return m_items.begin();
}

FileDataIndex::const_iterator FileDataIndex::Begin() const
{
    return m_items.begin();
}

FileDataIndex::iterator FileDataIndex::End()
{
    return m_items.end();
}

FileDataIndex::const_iterator FileDataIndex::End() const
{
    return m_items.end();
}

bool FileDataIndex::Insert(const string& name, const FileDataIndexItem& data)
{
    pair<FileDataIndex::iterator, bool> operationInfo = m_items.insert(pair<string, FileDataIndexItem>(name, data));

    return operationInfo.second;
}

void FileDataIndex::Update(const string& name, const FileDataIndexItem& data)
{
    m_items[name] = data;
}

void FileDataIndex::Delete(const string& name)
{
    m_items.erase(name);
}

void FileDataIndex::Clear()
{
    m_items.clear();
}

size_t FileDataIndex::Size()
{
    return m_items.size();
}