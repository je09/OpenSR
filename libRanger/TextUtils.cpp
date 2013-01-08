/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libRanger.h"
#include <iconv.h>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <locale.h>
#include <langinfo.h>
#endif


//TODO: Fix resulting length

/*!
 * \param to input encoding
 * \param from output encoding
 * \param what input string
 * \param srcLength input string size
 * \param destLength output string size
 * \return converted string
 */
char *Rangers::convertText(const char *to, const char *from, const char *what, int srcLength, int &destLength)
{
    if (!strcmp(to, from))
    {
        char *result = (char*)malloc(srcLength);
        memcpy(result, what, srcLength);
        destLength = srcLength;
        return result;
    }
    iconv_t codec = iconv_open(to, from);
    if (codec == (iconv_t) - 1)
    {
        std::cerr << "Cannot open text codec: " << strerror(errno) << std::endl;
        return 0;
    }
    if (srcLength < 0)
        srcLength = strlen(what) + 1;
    char *result = (char*)malloc(4 * srcLength);
    memset(result, 0, 4 * srcLength);

    size_t inbuflength = srcLength;
    size_t outbuflength = 4 * srcLength;
    char *pointer = result;
    char **inp = (char **)&what;
#ifdef ICONV_SECOND_ARGUMENT_IS_CONST
    iconv(codec, (const char **)inp, &inbuflength, &pointer, &outbuflength);
#else
    iconv(codec, inp, &inbuflength, &pointer, &outbuflength);
#endif
    size_t l = 4 * srcLength - outbuflength;

    result = (char *)realloc(result, l);
    iconv_close(codec);
    destLength = l;
    return result;
}

/*!
 * \param codec input encoding
 * \param text input string
 * \param length input string size
 * \return converted string
 */
std::wstring Rangers::fromCodec(const char *codec, const char *text, int length)
{
    if (length == 0)
        return std::wstring();

    if (length < 0)
        length = strlen(text) + 1;

    int outl;

    //FIXME: Workaround about not working WCHAR_T on Windows XP
#ifdef _WIN32
    char *data = convertText("UCS-2LE", codec, text, length, outl);
#else
    char *data = convertText("WCHAR_T", codec, text, length, outl);
#endif

    if (!data)
        return std::wstring();

    std::wstring result = std::wstring((wchar_t *)data);
    free(data);
    return result;
}

std::wstring Rangers::fromUTF8(const char *text, int length)
{
    return fromCodec("UTF-8", text, length);
}

std::wstring Rangers::fromASCII(const char *text, int length)
{
    return fromCodec("ASCII", text, length);
}

/*!
 * \param codec output encoding
 * \param text input string
 * \return converted string
 */
std::string Rangers::toCodec(const char *codec, const std::wstring& text)
{
    int resultLength;
//FIXME: Workaround about not working WCHAR_T on Windows XP
#ifdef _WIN32
    char *data = convertText(codec, "UCS-2LE", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#else
    char *data = convertText(codec, "WCHAR_T", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#endif
    if (!data)
        return std::string();

    std::string str(data);
    free(data);
    return str;
}

/*!
 * \param codec output encoding
 * \param text input string
 * \return converted string
 */
/*char* Rangers::toCodec(const char *codec, const std::wstring& text, int& resultLength)
{
//FIXME: Workaround about not working WCHAR_T on Windows XP
#ifdef _WIN32
    char *data = convertText(codec, "UCS-2LE", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#else
    char *data = convertText(codec, "WCHAR_T", (char *)text.c_str(), (text.length() + 1) * sizeof(wchar_t), resultLength);
#endif
    return data;
}*/

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toASCII(const std::wstring& text)
{
    return toCodec("ASCII", text);
}

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toUTF8(const std::wstring& text)
{
    return toCodec("UTF-8", text);
}

/*!
 * \param text input string
 * \return converted string
 */
std::string Rangers::toLocal(const std::wstring& text)
{
    std::string codec;
#ifdef _WIN32
    int cp = GetACP();
    std::ostringstream ss;
    ss << "CP" << cp;
    codec = ss.str();
#else
    setlocale(LC_ALL, "");
    codec = nl_langinfo(CODESET);
#endif
    return toCodec(codec.c_str(), text);
}

std::wstring Rangers::fromLocal(const char *text, int length)
{
    std::string codec;
#ifdef _WIN32
    int cp = GetACP();
    std::ostringstream ss;
    ss << "CP" << cp;
    codec = ss.str();
#else
    setlocale(LC_ALL, "");
    codec = std::string(nl_langinfo(CODESET));
#endif
    return fromCodec(codec.c_str(), text, length);
}

/*!
 * \param s string
 * \param c separator
 * \return array of strings
 */
std::vector<std::wstring> Rangers::split(const std::wstring& s, wchar_t c)
{
    std::vector<std::wstring> array;
    std::wstring::const_iterator begin = s.begin();
    std::wstring::const_iterator it = s.begin();
    for (std::wstring::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (*it == c)
        {
            array.push_back(s.substr(begin - s.begin(), it - begin));
            begin = it + 1;
        }
    }
    array.push_back(s.substr(begin - s.begin(), s.end() - begin));
    return array;
}

/*!
 * \param s file path
 * \return file extension
 */
std::wstring Rangers::suffix(const std::wstring& s)
{
    int pos;
    int lastPos, dotPos = std::wstring::npos;

    if ((pos = s.rfind(L'.')) == std::wstring::npos)
        return std::wstring();

    if ((lastPos = s.rfind(L'/')) == std::wstring::npos)
        if ((lastPos = s.rfind(L'\\')) == std::wstring::npos)
            lastPos = -1;

    while ((pos != std::wstring::npos) && (pos > lastPos))
    {
        dotPos = pos;
        if (pos == 0)
            break;
        pos = s.rfind(L'.', pos - 1);
    }

    if (dotPos != std::wstring::npos)
        return s.substr(dotPos + 1);
    else
        return std::wstring();
}

/*!
 * \param s file path
 * \return file basename
 */
std::wstring Rangers::basename(const std::wstring& s)
{
    std::wstring suf = suffix(s);

    int endpos;

    if (suf != L"")
        endpos = s.rfind(suffix(s)) - 1;
    else
        endpos = s.rfind(L'.');

    int startpos = s.rfind(L'/');

    if (endpos == std::wstring::npos)
    {
        if (startpos == std::wstring::npos)
            return s;
        else
            return s.substr(startpos + 1);
    }
    else
    {
        if (startpos == std::wstring::npos)
            return s.substr(0, endpos);
        else
            return s.substr(startpos + 1, endpos - startpos - 1);
    }
}

/*!
 * \param s file path
 * \return file directory
 */
std::wstring Rangers::directory(const std::wstring& s)
{
    int pos;
    if ((pos = s.rfind(L'/')) == std::wstring::npos)
#ifdef WIN32
        if ((pos = s.rfind(L'\\')) == std::wstring::npos)
#endif
            return std::wstring();
    return s.substr(0, pos + 1);
}

/*!
 * \param s string
 * \param c separator
 * \return array of strings
 */
std::vector<std::string> Rangers::split(const std::string& s, char c)
{
    std::vector<std::string> array;
    std::string::const_iterator begin = s.begin();
    std::string::const_iterator it = s.begin();
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (*it == c)
        {
            array.push_back(s.substr(begin - s.begin(), it - begin));
            begin = it + 1;
        }
    }
    array.push_back(s.substr(begin - s.begin(), s.end() - begin));
    return array;
}

/*!
 * \param s file path
 * \return file extension
 */
std::string Rangers::suffix(const std::string& s)
{
    int pos;
    int lastPos, dotPos = std::string::npos;

    if ((pos = s.rfind('.')) == std::string::npos)
        return std::string();

    if ((lastPos = s.rfind('/')) == std::string::npos)
        if ((lastPos = s.rfind('\\')) == std::string::npos)
            lastPos = -1;

    while ((pos != std::string::npos) && (pos > lastPos))
    {
        dotPos = pos;
        if (pos == 0)
            break;
        pos = s.rfind('.', pos - 1);
    }

    if (dotPos != std::string::npos)
        return s.substr(dotPos + 1);
    else
        return std::string();
}

/*!
 * \param s file path
 * \return file basename
 */
std::string Rangers::basename(const std::string& s)
{
    std::string suf = suffix(s);

    int endpos;

    if (suf != "")
        endpos = s.rfind(suffix(s)) - 1;
    else
        endpos = s.rfind('.');

    int startpos = s.rfind('/');

    if (endpos == std::string::npos)
    {
        if (startpos == std::string::npos)
            return s;
        else
            return s.substr(startpos + 1);
    }
    else
    {
        if (startpos == std::string::npos)
            return s.substr(0, endpos);
        else
            return s.substr(startpos + 1, endpos - startpos - 1);
    }
}

/*!
 * \param s file path
 * \return file directory
 */
std::string Rangers::directory(const std::string& s)
{
    int pos;
    if ((pos = s.rfind('/')) == std::string::npos)
#ifdef WIN32
        if ((pos = s.rfind('\\')) == std::string::npos)
#endif
            return std::string();
    return s.substr(0, pos + 1);
}

/*!
 * \param s input string
 * \return trimmed string
 */
std::wstring Rangers::trim(const std::wstring& s)
{
    std::wstring result = s;
    std::wstring::iterator end = result.end();
    std::wstring::iterator spaceEnd;
    for (spaceEnd = result.begin(); (spaceEnd != end) && ((*spaceEnd == L' ') || (*spaceEnd == L'\t') || (*spaceEnd == L'\n') || (*spaceEnd == L'\r')); ++spaceEnd);
    if (spaceEnd != result.begin())
        result.erase(result.begin(), spaceEnd);
    end = result.begin();
    for (spaceEnd = result.end(); (spaceEnd != end) && ((*(spaceEnd - 1) == L' ') || (*(spaceEnd - 1) == L'\t') || (*(spaceEnd - 1) == L'\n') || (*(spaceEnd - 1) == L'\r')); --spaceEnd);
    if (spaceEnd != result.end())
        result.erase(spaceEnd, result.end());
    return result;
}

/*!
 * \param s input string
 * \return trimmed string
 */
std::string Rangers::trim(const std::string& s)
{
    std::string result = s;
    std::string::iterator end = result.end();
    std::string::iterator spaceEnd;
    for (spaceEnd = result.begin(); (spaceEnd != end) && ((*spaceEnd == ' ') || (*spaceEnd == '\t') || (*spaceEnd == '\n') || (*spaceEnd == '\r')); ++spaceEnd);
    if (spaceEnd != result.begin())
        result.erase(result.begin(), spaceEnd);
    end = result.begin();
    for (spaceEnd = result.end(); (spaceEnd != end) && ((*(spaceEnd - 1) == ' ') || (*(spaceEnd - 1) == '\t') || (*(spaceEnd - 1) == '\n') || (*(spaceEnd - 1) == '\r')); --spaceEnd);
    if (spaceEnd != result.end())
        result.erase(spaceEnd, result.end());
    return result;
}
