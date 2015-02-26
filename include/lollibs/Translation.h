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
 
 The Translation.h unit provides functions and the class to work with Lands of Lore's .ENG, .GER,
 .FRE, .DIP formats.
 
********************************************************************************************/

#ifndef LANDS_OF_LORE_TRANSLATION_H
#define LANDS_OF_LORE_TRANSLATION_H

#include <string>
#include <vector>
#include <istream>

namespace lollibs {
    using std::string;
    using std::vector;
    using std::istream;
    using std::ostream;

    class Translation
    {
    public:
        typedef vector<string>::size_type size_type;

        typedef vector<string>::iterator iterator;
        typedef vector<string>::const_iterator const_iterator;

        typedef vector<string>::reference reference;
        typedef vector<string>::const_reference const_reference;

    public:
        //! Load translation contents from the memory.
        /*!
        \param[in] data pointer to translation's content.
        \param[in] size length of translation's content.
        \return If translation was loaded successfully then true, else - false.
        */
        bool LoadFromMemory(const char *data, const unsigned short& size);
        
        //! Load translation contents from the file on disk.
        /*!
        \param[in] path path to a file on disk.
        \return If translation was loaded successfully then true, else - false.
        */
        bool LoadFromFile(const char *path);
        
        //! Load translation contents from the stream.
        /*!
        \param[in] stream stream with translation's content.
        \return If translation was loaded successfully then true, else - false.
        */
        bool LoadFromStream(istream& stream);
        
        //! Save translation contents to the stream.
        /*!
        \param[out] stream stream to what we need to write translation's content.
        */
        void SaveToStream(ostream& stream) const;

        //! Save translation to a file on disk.
        /*!
        \param[in] path path to a file on disk.
        */
        void SaveToFile(const char *path) const;

        Translation::iterator Begin();
        Translation::const_iterator Begin() const;

        Translation::iterator End();
        Translation::const_iterator End() const;

        //! Get translation string by index. It checks if pos is in proper boundaries.
        /*!
        \param[in] pos index of the translation string.
        \return Reference to the translation string.
        */
        reference Get(size_type pos);
        const_reference Get(size_type pos) const;

        //! Insert new translation string at the beginning of strings list.
        /*!
        \param[in] data new translation string's content.
        */
        void Prepend(const string& data);

        //! Insert new translation string at the ending of strings list.
        /*!
        \param[in] data new translation string's content.
        */
        void Append(const string& data);

        //! Insert new translation string before position specified in the pos argument.
        /*!
        \param[in] pos position before which new translation string must be inserted.
        \param[in] data new translation string's content.
        */
        void Insert(size_type pos, const string& data);

        //! Delete translation string by index
        /*!
        \param[in] pos index of translation string.
        */
        void Delete(size_type pos);

        //! Delete all translation strings.
        void Clear();

        //! Count entries in the translation.
        /*!
        \return How many entries are in the package.
        */
        size_t Size();

        //! Get translation string by index. It doesn't check if pos is in proper boundaries.
        /*!
        \param[in] pos index of the translation string.
        \return Reference to the translation string.
        */
        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;

    private:
        vector<string> m_items;
    };

    //! Convert every translation's entry encoding from Floppy version's Russian encoding to CP1251
    /*!
    \param[in,out] translation Translation to decode.
    */
    void DecodeRussianTranslationFloppy(Translation& translation);

    //! Convert every translation's entry encoding from CD version's Russian encoding to CP1251. It works
    //! properly for English translation as well but proper conversion for other translations is not guaranteed.
    /*!
    \param[in,out] translation Translation to decode.
    */
    void DecodeRussianTranslationCD(Translation& translation);

    //! Convert every translation's entry encoding from CP1251 to CD version's Russian encoding. It works
    //! properly for English translation as well but proper conversion for other translations is not guaranteed.
    /*!
    \param[in,out] translation Translation to encode.
    */
    void EncodeRussianTranslationCD(Translation& translation);
}

#endif // LANDS_OF_LORE_TRANSLATION_H