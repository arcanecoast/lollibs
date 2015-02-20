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
 
 PackageItemData describes contents of file in a package (for Lands of Lore .PAK packages
 format).
 
 It is important to mention that the class only describes file's content. Information about
 filename is stored in a package.
 
********************************************************************************************/

#ifndef LANDS_OF_LORE_PACKAGE_ITEM_DATA_H
#define LANDS_OF_LORE_PACKAGE_ITEM_DATA_H

namespace lollibs {
    class PackageItemData
    {
    private:
        unsigned long m_size;
        char* m_content;

        mutable unsigned int* m_referencesToContentAmount;

    public:
        PackageItemData();

        //! Content assign constructor. It doesn't copy data from content pointer but rather assigns inner
        //! content pointer to location where parameter "content" points (it doesn't copy new data, so be
        //! aware not to free content pointer after assignment).
        /*!
        \param[in] content pointer to data.
        \param[in] size content's length (in bytes).
        */
        PackageItemData(char* content, unsigned long size);

        PackageItemData(const PackageItemData& data);

        ~PackageItemData();

        //! Create new copy of content and put it to destination.
        /*!
        \param[out] destination pointer where to put data.
        \return Destination pointer if operation is successful or nullptr if not.
        */
        char* CopyContent(char* destination) const;

        //! Get size of content.
        /*!
        \return Content's length (in bytes).
        */
        unsigned long GetSize() const;

        //! Get pointer to data. It doesn't copy content but rather provides direct access to data.
        /*!
        \return Pointer to item's contents or nullptr if no content is present.
        */
        const char* GetContent() const;

        //! Assign new content to current object. It deletes current object's data and 
        //! assigns object's inner content pointer to location where parameter "content" points 
        //! (it doesn't copy new data, so be aware not to free content pointer after assignment).
        /*!
        \param[in] content pointer to data.
        \param[in] size content's length (in bytes).
        */
        void SetContent(char* content, unsigned long size);

        //! Free content to which this object is assigned.
        void FreeContent();

        PackageItemData& operator=(const PackageItemData& data);
    };
}

#endif //LANDS_OF_LORE_PACKAGE_ITEM_DATA_H