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
 
 Class and functions to operate with .FDT format of Lands of Lore game.
 
********************************************************************************************/

#ifndef LANDS_OF_LORE_FILEDATAINDEX_H
#define LANDS_OF_LORE_FILEDATAINDEX_H

#include <map>
#include <string>

#include "FileDataIndexItem.h"

namespace lollibs {
    using std::map;
    using std::string;

    class FileDataIndex
    {
    public:
        typedef map<string, FileDataIndexItem>::iterator iterator;
        typedef map<string, FileDataIndexItem>::const_iterator const_iterator;

    public:
        //! Load .FDT index from the file.
        /*!
        \param[in] path path to the file.
        \return If file was loaded successfully then true, else - false
        */
        bool LoadFromFile(const char *path);

        //! Save .FDT index to the file.
        /*!
        \param[in] path path to the file.
        */
        void SaveToFile(const char *path) const;

        FileDataIndex::iterator Find(const string& name);
        FileDataIndex::const_iterator Find(const string& name) const;

        FileDataIndex::iterator Begin();
        FileDataIndex::const_iterator Begin() const;

        FileDataIndex::iterator End();
        FileDataIndex::const_iterator End() const;

        //! Insert new file into index.
        /*!
        \param[in] name new file's name.
        \param[in] data new file's data.
        \return If file was inserted successfully then true, else - false
        */
        bool Insert(const string& name, const FileDataIndexItem& data);

        //! Update file by name. If file doesn't exists in the index then insert new file entry with specified data.
        /*!
        \param[in] name name of a file in the index.
        \param[in] data file's new data.
        */
        void Update(const string& name, const FileDataIndexItem& data);

        //! Delete file by name.
        /*!
        \param[in] name name of a file in the index.
        */
        void Delete(const string& name);

        //! Delete all files from the index.
        void Clear();

        //! Count files in the index.
        /*!
        \return How many files are in the index.
        */
        size_t Size();

    private:
        map<string, FileDataIndexItem> m_items;
    };
}

#endif // LANDS_OF_LORE_FILEDATAINDEX_H
