#include <lollibs/PackageItemData.h>

#include <cstring>

using namespace std;
using namespace lollibs;

PackageItemData::PackageItemData() : 
    m_size(0), m_content(nullptr),
    m_referencesToContentAmount(nullptr)
{
}

PackageItemData::PackageItemData(char* content, unsigned long size) : 
    m_size(size), m_content(content),
    m_referencesToContentAmount(new unsigned int(1))
{
}

PackageItemData::PackageItemData(const PackageItemData& data) :
    m_size(data.m_size), m_content(data.m_content),
    m_referencesToContentAmount(data.m_referencesToContentAmount)
{    
    ++(*m_referencesToContentAmount);
}

PackageItemData::~PackageItemData()
{
    FreeContent();
}

char* PackageItemData::CopyContent(char* destination) const
{
    if (m_size > 0) {
        memcpy(destination, m_content, m_size);

        return destination;
    } else {
        return nullptr;
    }
}

unsigned long PackageItemData::GetSize() const
{
    return m_size;
}

const char* PackageItemData::GetContent() const
{
    return m_content;
}

void PackageItemData::SetContent(char* content, unsigned long size)
{
    FreeContent();

    m_size = size;
    m_content = content;
    m_referencesToContentAmount = new unsigned int(1);
}

PackageItemData& PackageItemData::operator=(const PackageItemData& data)
{
    if (this != &data) {
        FreeContent();

        m_size = data.m_size;
        m_content = data.m_content;
        m_referencesToContentAmount = data.m_referencesToContentAmount;
        
        ++(*m_referencesToContentAmount);
    }

    return *this;
}

void PackageItemData::FreeContent()
{
    if (m_referencesToContentAmount) {
        // If another objects are referencing to content then
        
        if (*m_referencesToContentAmount > 1) {
            // ... just decrease references count
            
            --(*m_referencesToContentAmount);
        } else {
            // Else free content and references counter

            delete m_referencesToContentAmount;
            delete[] m_content;
        }
    }

    m_size = 0;
    m_content = nullptr;
    m_referencesToContentAmount = nullptr;
}
