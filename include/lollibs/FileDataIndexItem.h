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
 
 FileDataIndexItem describes single entry of .FDT index.
 
********************************************************************************************/

#ifndef LANDS_OF_LORE_FILEDATAINDEX_ITEM_H
#define LANDS_OF_LORE_FILEDATAINDEX_ITEM_H

namespace lollibs {
    struct FileDataIndexItem
    {
        unsigned long m_size; //! Size of file
        unsigned long m_unknown; //! Unknown word of index file entry
        unsigned long m_offsetInPackage; //! Offset to file's content (?) in the package
        unsigned short m_indexInPackage; //! Index of file in the package
        unsigned short m_itemType; //! Type of item: 00 05 - package, 00 00 - TLK file, 00 10 - normal file (?) 
    };
}

#endif //LANDS_OF_LORE_FILEDATAINDEX_ITEM_H