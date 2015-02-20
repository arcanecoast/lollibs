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
 
 The Package class provides operations to manipulate Lands of Lore's .PAK package.
 
********************************************************************************************/

#ifndef LANDS_OF_LORE_PACKAGE_H
#define LANDS_OF_LORE_PACKAGE_H

#include <map>
#include <string>
#include <memory>

#include "PackageItemData.h"

namespace lollibs {
    using std::map;
    using std::string;

    class Package
    {
    public:
        typedef map<string, PackageItemData>::iterator iterator;
        typedef map<string, PackageItemData>::const_iterator const_iterator;

    public:
        //! Load package content from a file.
        /*!
        \param[in] path path to a file.
        \return If file was loaded successfully then true, else - false
        */
        bool LoadFromFile(const char *path);

        //! Save package content to a file.
        /*!
        \param[in] path path to a file.
        */
        void SaveToFile(const char *path) const;

        Package::iterator Find(const string& name);
        Package::const_iterator Find(const string& name) const;

        Package::iterator Begin();
        Package::const_iterator Begin() const;

        Package::iterator End();
        Package::const_iterator End() const;

        //! Insert new file.
        /*!
        \param[in] name new file's name.
        \param[in] data new file's data.
        \return If file was inserted successfully then true, else - false
        */
        bool Insert(const string& name, const PackageItemData& data);

        //! Update file by name. If file doesn't exists in package then insert new file with specified data.
        /*!
        \param[in] name name of a file in the package.
        \param[in] data file's new data.
        */
        void Update(const string& name, const PackageItemData& data);

        //! Delete file by name.
        /*!
        \param[in] name name of a file in the package.
        */
        void Delete(const string& name);

        //! Delete all files from the package.
        void Clear();
        
        //! Count files in the package.
        /*!
        \return How many files are in the package.
        */
        size_t Size();

    private:
        struct PackageItemOnDiskHeader
        {
            string resourceName;
            unsigned long offsetToData;
        };

        map<string, PackageItemData> m_items;
    };
}

#endif // LANDS_OF_LORE_PACKAGE_H
