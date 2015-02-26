#include <lollibs/Translation.h>

#include <fstream>
#include <sstream>

using namespace std;
using namespace lollibs;

static const char PRIMARY_CHAR_ENCODING_TABLE_CD[16] = { ' ', 'e', 't', 'a', 'i', 'n', 'o', 's', 'r', 'l', 'h', 'c', 'd', 'u', 'p', 'm' };
static const char SECONDARY_CHAR_ENCODING_TABLE_CD[16][8 + 1 /* for 0 symbol */] = {
    "tasio wb", " rnsdalm", "h ieoras", "nrtlc sy",
    "nstcloer", " dtgesio", "nr ufmsw", " tep.ica",
    "e oiadur", " laeiyod", "eia otru", "etoakhlr",
    " eiu,.oa", "nsrctlai", "leoiratp", "eaoip bm"
};

bool Translation::LoadFromStream(istream& in)
{
    // Clear junk

    Clear();
    
    in.seekg(0, ios_base::end);
    unsigned long offsetToTechnicalEntryRawData = static_cast<unsigned long>(in.tellg()) - 1;
    in.seekg(0);

    // Read header until technical record is reached

    vector<unsigned short> offsetsData;
    unsigned short offsetCurrentItemData = 0;

    while (true) {
        in.read(reinterpret_cast<char*>(&offsetCurrentItemData), 2);

        if (offsetCurrentItemData == offsetToTechnicalEntryRawData) {
            break;
        }

        if (!in.good()) {
            return false;
        }

        offsetsData.push_back(offsetCurrentItemData);
    }

    // Read content

    for (vector<unsigned short>::iterator offset = offsetsData.begin(), offsetEnd = offsetsData.end(); offset != offsetEnd; ++offset) {
        unsigned int length;
        if ((offset + 1) == offsetEnd) {
            length = offsetToTechnicalEntryRawData - *(offset);
        } else {
            length = *(offset + 1) - *(offset);
        }

        char* content = new char[length];
        in.read(content, length);

        if (!in.good()) {
            delete[] content;

            return false;
        }

        m_items.push_back(content);
        delete[] content;
    }
    
    return true;
}

bool Translation::LoadFromMemory(const char *data, const unsigned short& size)
{
    // Open in memory file

    stringstream in(ios_base::in | ios_base::out | ios_base::binary | ios_base::ate);
    in.write(data, size);

    if (!in.good()) {
        return false;
    }
    
    // Try to load translation from opened stream
    
    return LoadFromStream(in);
}

bool Translation::LoadFromFile(const char *path)
{
    // Open on disk file

    ifstream in(path, ios_base::in | ios_base::binary | ios_base::ate);

    if (!in.good()) {
        return false;
    }
    
    // Try to load translation from opened stream
    
    return LoadFromStream(in);
}

void Translation::SaveToStream(ostream& out) const
{
    // Write header and calculate expected file length

    const unsigned long SIZE_OF_OFFSET = 2;
    unsigned short offsetToDataStart = SIZE_OF_OFFSET * (static_cast<unsigned short>(m_items.size()) + 1);
    unsigned short offsetToCurrentEntryRawData = offsetToDataStart;

    for (vector<string>::const_iterator entry = Begin(), entryEnd = End(); entry != entryEnd; ++entry) {
        out.write(reinterpret_cast<char*>(&offsetToCurrentEntryRawData), 2);

        offsetToCurrentEntryRawData += static_cast<unsigned short>(entry->length()) + 1 /* for 0 symbol */;
    }

    // Write technical record in header section (it contains <length of file - 1>)

    out.write(reinterpret_cast<char*>(&offsetToCurrentEntryRawData), 2);

    // Write raw data

    for (vector<string>::const_iterator entry = Begin(), entryEnd = End(); entry != entryEnd; ++entry) {
        out.write(entry->c_str(), entry->length() + 1);
    }

    // Write technical record in raw data section (just 0)

    out.write("\0", 1);
}

void Translation::SaveToFile(const char *path) const
{
    // Open file stream to write in

    fstream out(path, ios_base::out | ios_base::binary | ios_base::trunc);

    if (!out.good()) {
        return;
    }

    SaveToStream(out);
}

Translation::iterator Translation::Begin()
{
    return m_items.begin();
}

Translation::const_iterator Translation::Begin() const
{
    return m_items.begin();
}

Translation::iterator Translation::End()
{
    return m_items.end();
}

Translation::const_iterator Translation::End() const
{
    return m_items.end();
}

Translation::reference Translation::Get(size_type pos)
{
    return m_items.at(pos);
}

Translation::const_reference Translation::Get(size_type pos) const
{
    return m_items.at(pos);
}

void Translation::Prepend(const string& data)
{
    m_items.insert(m_items.begin(), data);
}

void Translation::Append(const string& data)
{
    m_items.push_back(data);
}

void Translation::Insert(size_type pos, const string& data)
{
    m_items.insert(m_items.begin() + pos, data);
}

void Translation::Delete(size_type pos)
{
    m_items.erase(m_items.begin() + pos);
}

void Translation::Clear()
{
    m_items.clear();
}

Translation::reference Translation::operator[](size_type pos)
{
    return m_items[pos];
}

Translation::const_reference Translation::operator[](size_type pos) const
{
    return m_items[pos];
}

size_t Translation::Size()
{
    return m_items.size();
}

void lollibs::DecodeRussianTranslationFloppy(Translation& translation)
{
    static const char PRIMARY_CHAR_ENCODING_TABLE[16] = { ' ', 'מ', 'ו', 'א', 'ע', 'ם', 'ט', 'נ', 'ס', 'כ', 'ג', 'ה', 'ל', 'ך', 'ף', '.' };
    static const char SECONDARY_CHAR_ENCODING_TABLE[16][8 + 1 /* for 0 symbol */] = {
        "םןגסלך ה", "כ נעהגזך", " םענכלסב", " לסךכנעם",
        "מüטוא נû", "מאטוûÿ ף", "ע םקכלונ", "מאוטףûÿך",
        "עÿוüכמךן", "טאוÿמ‏üף", "אמוטû נף", "אומטףסםכ",
        " מואטם.ף", "מאט נףכס", " הםעבזלס", "\n . אבÝו"
    };

    string decodedString;

    // Iterate through all entries

    for (Translation::iterator entry = translation.Begin(), entryEnd = translation.End(); entry != entryEnd; ++entry) {
        // Iterate through all symbols in entry

        decodedString = "";

        for (string::size_type symbolIndex = 0, symbolEnd = entry->length(); symbolIndex != symbolEnd; ++symbolIndex) {
            char symbol = (*entry)[symbolIndex];

            if (symbol & 0x80) {
                // If greatest bit of symbol is set then symbol encodes two characters

                unsigned short primaryPart = (symbol >> 3) & 0x0F;
                unsigned short secondaryPart = symbol & 0x07;

                decodedString += PRIMARY_CHAR_ENCODING_TABLE[primaryPart];
                decodedString += SECONDARY_CHAR_ENCODING_TABLE[primaryPart][secondaryPart];
            } else if (symbol == 0x7F) {
                // If symbol is 0x7F then just add next symbol to resulting string without any transformations

                ++symbolIndex;
                decodedString += entry->at(symbolIndex);
            } else {
                // Otherwise modify it and add to resulting string

                if (symbol > 47) {
                    symbol -= 112;
                }

                decodedString += symbol;
            }
        }

        *entry = decodedString;
    }
}

void lollibs::DecodeRussianTranslationCD(Translation& translation)
{
    string decodedString;

    // Iterate through all entries

    for (Translation::iterator entry = translation.Begin(), entryEnd = translation.End(); entry != entryEnd; ++entry) {
        // Iterate through all symbols in entry

        decodedString = "";

        for (string::size_type symbolIndex = 0, symbolEnd = entry->length(); symbolIndex != symbolEnd; ++symbolIndex) {
            char symbol = (*entry)[symbolIndex];

            if (symbol & 0x80) {
                // If greatest bit of symbol is set then symbol encodes two characters

                unsigned short primaryPart = (symbol >> 3) & 0x0F;
                unsigned short secondaryPart = symbol & 0x07;

                decodedString += PRIMARY_CHAR_ENCODING_TABLE_CD[primaryPart];
                decodedString += SECONDARY_CHAR_ENCODING_TABLE_CD[primaryPart][secondaryPart];
            } else if (symbol == 27) {
                // If symbol is 27 then convert encoding of next symbol and add latter to resulting string

                ++symbolIndex;
                symbol = entry->at(symbolIndex);

                if (symbol >= 0 && symbol <= 48) {
                    symbol = symbol + 64 + 127;
                } else if (symbol >= 97 && symbol <= 112) {
                    symbol = symbol + 16 + 127;
                }

                decodedString += symbol;
            } else {
                // Otherwise don't do anything and just add symbol to resulting string

                decodedString += symbol;
            }
        }

        *entry = decodedString;
    }
}

void lollibs::EncodeRussianTranslationCD(Translation& translation)
{
    string encodedString;

    // Iterate through all entries

    for (Translation::iterator entry = translation.Begin(), entryEnd = translation.End(); entry != entryEnd; ++entry) {
        // Iterate through all symbols in entry

        encodedString = "";

        for (string::size_type symbolIndex = 0, symbolEnd = entry->length(); symbolIndex != symbolEnd; ++symbolIndex) {
            char symbol = (*entry)[symbolIndex];

            if ((symbolIndex + 1) != symbolEnd) {
                // If there is at least 2 symbols until the end then we can try to compress them into 1 symbol.
                // Lets check if these symbols can be compressed.

                char firstSymbol = symbol;
                char secondSymbol = (*entry)[(symbolIndex + 1)];

                int primaryTableIndex = -1;
                int secondaryTableIndex = -1;

                // Find index in primary symbols table for first symbol

                for (int i = 0; i < 16; ++i) {
                    if (firstSymbol == PRIMARY_CHAR_ENCODING_TABLE_CD[i]) {
                        primaryTableIndex = i;
                        break;
                    }
                }

                if (primaryTableIndex != -1) {
                    // If primary index was found then find index in secondary symbols table for second symbol

                    for (int i = 0; i < 8; ++i) {
                        if (secondSymbol == SECONDARY_CHAR_ENCODING_TABLE_CD[primaryTableIndex][i]) {
                            secondaryTableIndex = i;
                            break;
                        }
                    }

                    if (secondaryTableIndex != -1) {
                        // If primary and secondary indexes are found then compress symbols and add them to resulting string

                        symbol = 0x80 | ((primaryTableIndex & 0x0F) << 3) | (secondaryTableIndex & 0x07);

                        encodedString += symbol;
                        ++symbolIndex;

                        continue;
                    }
                }
            }
            
            if (symbol < 0) {
                // If symbol code is in 128..255 boundaries then we should encode it with 2 symbols
                // and transform its code to corresponding one to Russian font of the game

                symbol = symbol - 128;
                if (symbol >= 63 && symbol <= 111) {
                    symbol -= 63;
                } else if (symbol >= 112 && symbol <= 127) {
                    symbol -= 15;
                }

                encodedString += static_cast<char>(27);
                encodedString += symbol;
            } else {
                // Otherwise don't do anything and just add symbol to resulting string

                encodedString += symbol;
            }
        }

        *entry = encodedString;
    }
}
