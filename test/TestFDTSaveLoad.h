#ifndef TEST_FDT_SAVE_LOAD_H
#define TEST_FDT_SAVE_LOAD_H

#include <cxxtest/TestSuite.h>

#include <stdio.h>
#include <lollibs/FileDataIndex.h>

using namespace lollibs;

class TestFDTSaveLoad : public CxxTest::TestSuite
{
public:
    void testSaveLoad(void)
    {
        FileDataIndex firstIndex, secondIndex;

        TS_ASSERT(firstIndex.LoadFromFile("../test/fixture/FILEDATA.FDT"));
        firstIndex.SaveToFile("FILEDATA2.FDT");

        bool secondFileLoadedSuccessfully = secondIndex.LoadFromFile("FILEDATA2.FDT");
        if (!secondFileLoadedSuccessfully) {
            TS_FAIL("Unable to load file FILEDATA2.FDT");
        }

        remove("FILEDATA2.FDT");

        for (FileDataIndex::iterator firstIndexIter = firstIndex.Begin(); firstIndexIter != firstIndex.End(); ++firstIndexIter) {
            FileDataIndex::iterator secondIndexIter = secondIndex.Find(firstIndexIter->first);

            if (secondIndexIter == secondIndex.End()) {
                TS_FAIL("Unable to find entry with name " + firstIndexIter->first + " in second index");
            }

            TS_ASSERT_EQUALS(secondIndexIter->second.m_indexInPackage, firstIndexIter->second.m_indexInPackage);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_itemType, firstIndexIter->second.m_itemType);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_offsetInPackage, firstIndexIter->second.m_offsetInPackage);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_size, firstIndexIter->second.m_size);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_unknown, firstIndexIter->second.m_unknown);
        }

        for (FileDataIndex::iterator secondIndexIter = secondIndex.Begin(); secondIndexIter != secondIndex.End(); ++secondIndexIter) {
            FileDataIndex::iterator firstIndexIter = firstIndex.Find(secondIndexIter->first);

            if (firstIndexIter == firstIndex.End()) {
                TS_FAIL("Unable to find entry with name " + secondIndexIter->first + " in first index");
            }

            TS_ASSERT_EQUALS(secondIndexIter->second.m_indexInPackage, firstIndexIter->second.m_indexInPackage);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_itemType, firstIndexIter->second.m_itemType);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_offsetInPackage, firstIndexIter->second.m_offsetInPackage);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_size, firstIndexIter->second.m_size);
            TS_ASSERT_EQUALS(secondIndexIter->second.m_unknown, firstIndexIter->second.m_unknown);
        }
    }
};

#endif //TEST_FDT_SAVE_LOAD_H