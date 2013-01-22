/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_LIBRANGER_H
#define RANGERS_LIBRANGER_H

#include <iostream>
#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <boost/shared_ptr.hpp>

#ifdef LIBRANGER_LIBRARY
#ifdef WIN32
#define LIBRANGER_API __declspec(dllexport)
#else
#define LIBRANGER_API
#endif
#else
#ifdef WIN32
#define LIBRANGER_API
#else
#define LIBRANGER_API extern
#endif
#endif

namespace Rangers
{

//! Header of layer in *.gi files
struct GILayerHeader
{
    uint32_t seek;  //!< Layer offset in file
    uint32_t size;  //!< Layer size
    uint32_t startX;  //!< Layer left corner
    uint32_t startY;  //!< Layer top corner
    uint32_t finishX;  //!< Layer rigth corner
    uint32_t finishY;  //!< Layer bottom corner
    uint32_t unknown1;
    uint32_t unknown2;

    unsigned char *data; //!< layer data
};

//! Header of frame in *.gi file

struct GIFrameHeader
{
    uint32_t signature;  //!< Signature
    uint32_t version;  //!< Version of GI file
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t rBitmask;  //!< Mask of r color component
    uint32_t gBitmask;  //!< Mask of g color component
    uint32_t bBitmask;  //!< Mask of b color component
    uint32_t aBitmask;  //!< Mask of a color component
    uint32_t type;  //!< Frame type
    /*!<
     * Variants:
     *  -# 0 - One layer, 16 or 32 bit, depends on mask.
     *  -# 1 - One layer, 16 bit RGB optimized.
     *  -# 2 - Three layers:
     *   -# 16 bit RGB optimized - body
     *   -# 16 bit RGB optimized - outline
     *   -# 6 bit Alpha optimized
     *  -# 3 - Two layers:
     *   -# Indexed RGB colors
     *   -# Indexed Alpha
     *  -# 4 - One layer, indexed RGBA colors
     *  -# 5 - Unknown type, used in large animations.
     */
    uint32_t layerCount; //!< Number of layers in frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;

    GILayerHeader *layers; //!< Layers
};

//! GI format frame.

struct GIFrame
{
    int width;   //!< Total frame width
    int height;   //!< Total frame height
    unsigned char *data; //!< Frame data (in RGBA format)
};

//! Header of *.gai animation file

struct GAIHeader
{
    uint32_t signature;  //!< File signature
    uint32_t version;  //!< Format version
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t frameCount; //!< Number of frames in animation
    uint32_t haveBackground; //!< Animation a background in separate file
    uint32_t waitSeek;  //!< Wait seek?
    uint32_t waitSize;  //!< Wait size?
    uint32_t unknown1;
    uint32_t unknown2;

    GIFrame *frames;  //!< Animation frames
};

//! GAI Animation

struct GAIAnimation
{
    int width;   //!< Total animation width
    int height;   //!< Total animation height
    int frameCount;  //!< Number of frames in animation
    int waitSeek;  //!< Wait seek?
    int waitSize;  //!< Wait size?
    GIFrame *frames;  //!< Animation frames
};

//! Header of *.hai animation file

struct HAIHeader
{
    uint32_t signature;  //!< Signature
    uint32_t width;  //!< Animation width
    uint32_t height;  //!< Animation height
    uint32_t rowBytes;  //!< Bytes in one line
    uint32_t count;  //!< Number of frames in animation
    uint32_t frameSize;  //!< Size of one frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
    uint32_t palSize;  //!< Size of pallete
};

//! HAI animation

struct HAIAnimation
{
    int width;   //!< Animation width
    int height;   //!< Animation height
    int frameCount;  //!< Number of frames in animation
    unsigned char *frames; //!< Frames data
};

enum RPKGCompression {RPKG_SEEKABLE_ZLIB, RPKG_SEEKABLE_LZMA, RPKG_NONE};

//! File in RPKG archive
struct RPKGItem
{
    uint32_t packType;  //!< Compression type
    uint32_t packSize;  //!< Compressed data size
    uint32_t size;  //!< Uncompressed data size
    uint32_t chunkSize;
    unsigned char *data; //!< Data
};

//! File description in RPKG

struct RPKGEntry
{
    std::wstring name;  //!< File name
    uint32_t offset;  //!< File offset in archive
    uint32_t size; //!< File size in archive
};

//! PKG archive item

struct PKGItem
{
    uint32_t sizeInArc;  //!< Compressed data size
    uint32_t size;  //!< Uncompressed data size
    char fullName[63];  //!< File name in upper case
    char name[63];  //!< File name
    uint32_t dataType;  //!< Data type
    /*!<
     * Variants:
     *  -# 1 - RAW data
     *  -# 2 - ZLIB compressed file
     *  -# 3 - Directory
     */
    uint32_t offset;  //!< Fille offset in archive
    uint32_t childCount; //!< Number of childs
    PKGItem *parent;  //!< Parent item
    PKGItem *childs;  //!< Child items
};

struct DDSPixelFormat
{
    uint32_t size;
    uint32_t flags;
    uint32_t fourCC;
    uint32_t rgbBitCount;
    uint32_t rBitMask;
    uint32_t gBitMask;
    uint32_t bBitMask;
    uint32_t aBitMask;
};

enum DDSPixelFormatFlags {DDPF_ALPHAPIXELS = 0x1, DDPF_ALPHA = 0x2, DDPF_FOURCC = 0x4,
                          DDPF_RGB = 0x40, DDPF_YUV = 0x200, DDPF_LUMINANCE = 0x20000
                         };

struct DDSHeader
{
    uint32_t size;
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    uint32_t pitchOrLinearSize;
    uint32_t depth;
    uint32_t mipMapCount;
    uint32_t reserved1[11];
    DDSPixelFormat ddspf;
    uint32_t caps;
    uint32_t caps2;
    uint32_t caps3;
    uint32_t caps4;
    uint32_t reserved2;
};

enum DDSHeaderFlags {DDSD_CAPS = 0x1, DDSD_HEIGHT = 0x2, DDSD_WIDTH = 0x4, DDSD_PITCH = 0x8,
                     DDSD_PIXELFORMAT = 0x1000, DDSD_MIPMAPCOUNT = 0x20000, DDSD_LINEARSIZE = 0x80000,
                     DDSD_DEPTH = 0x800000
                    };

enum DDSHeaderCaps {DDSCAPS_COMPLEX = 0x8, DDSCAPS_MIPMAP = 0x400000, DDSCAPS_TEXTURE = 0x1000};

enum DDSHeaderCaps2 {DDSCAPS2_CUBEMAP = 0x200, DDSCAPS2_CUBEMAP_POSITIVEX = 0x400, DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800,
                     DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000, DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000,
                     DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000, DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
                     DDSCAPS2_VOLUME = 0x200000
                    };

enum PNGType {PNG_INVALID, PNG_GRAY, PNG_RGB, PNG_RGBA};

struct PNGFrame
{
    int width;
    int height;
    PNGType type;
    unsigned char *data;
};

//! Load GI frame type 0
LIBRANGER_API GIFrame loadFrameType0(const GIFrameHeader& image);
//! Load GI frame type 1
LIBRANGER_API GIFrame loadFrameType1(const GIFrameHeader& image);
//! Load GI frame type 2
LIBRANGER_API GIFrame loadFrameType2(const GIFrameHeader& image);
//! Load GI frame type 3
LIBRANGER_API GIFrame loadFrameType3(const GIFrameHeader& image);
//! Load GI frame type 4
LIBRANGER_API GIFrame loadFrameType4(const GIFrameHeader& image);
//! Load GI frame type 5 - dummy
LIBRANGER_API GIFrame loadFrameType5(const GIFrameHeader& image, GIFrame *background = 0);
//! Load GI frame
LIBRANGER_API GIFrame loadGIImageData(const GIFrameHeader& image, GIFrame *background = 0);

//! Load GI frame from file
LIBRANGER_API GIFrame loadGIFrame(std::istream& stream, size_t &offset, GIFrame *background = 0, int startX = 0, int startY = 0, int finishX = 0, int finishY = 0);
//! Load GI frame from memory
LIBRANGER_API GIFrame loadGIFrame(const char *buffer, size_t &offset, GIFrame *background = 0, int startX = 0, int startY = 0, int finishX = 0, int finishY = 0);

//! Load HAI animation from file
LIBRANGER_API HAIAnimation loadHAI(std::istream& stream);
//! Load HAI animation from memory
LIBRANGER_API HAIAnimation loadHAI(const char *data);

//! Load GI file
LIBRANGER_API GIFrame loadGIFile(std::istream& stream);
//! Load GI file from memory
LIBRANGER_API GIFrame loadGIFile(const char *data);

//! Load GAI animation file
LIBRANGER_API GAIAnimation loadGAIAnimation(std::istream& stream, GIFrame *background = 0);
//! Load GAI animation from memory
LIBRANGER_API GAIAnimation loadGAIAnimation(const char *data, GIFrame *background = 0);
//! Load GAI header from file
LIBRANGER_API GAIHeader loadGAIHeader(std::istream& stream);
//! Load GAI header from memory
LIBRANGER_API GAIHeader loadGAIHeader(const char *data);

//! Get std::istream of file in package
LIBRANGER_API boost::shared_ptr<std::istream> getRPKGFileStream(const RPKGEntry& e, boost::shared_ptr<std::istream> stream);

//! Unpack ZL01 - compressed file
LIBRANGER_API unsigned char *unpackZL01(const unsigned char * src, size_t srclen, size_t& destlen);
//! Unpack ZL02 - compressed file
LIBRANGER_API unsigned char *unpackZL02(const unsigned char * src, size_t srclen, size_t& destlen);
//! Unpack ZL02 - compressed file
LIBRANGER_API void unpackZL02(unsigned char * dst, const unsigned char * src, size_t srclen, size_t& destlen);

//! Load file descriptions from PKG archive
LIBRANGER_API PKGItem *loadPKG(std::istream& stream);
//! Extract file from PKG archive
LIBRANGER_API unsigned char *extractFile(PKGItem item, std::istream& pkgfile);


LIBRANGER_API bool packRSXZ(const char * src, size_t srclen, RPKGItem &item);
LIBRANGER_API char *unpackRSXZ(RPKGItem item);

//! Compress data to RSZL format
LIBRANGER_API bool packRSZL(const char * src, size_t srclen, RPKGItem &item);
//! Uncompress data in ZLIB format
LIBRANGER_API char *unpackRSZL(RPKGItem item);

//! Load file descriptions from RPKG archive
LIBRANGER_API std::list<RPKGEntry> loadRPKG(std::istream& stream);
//! Load file from archive
LIBRANGER_API char *extractFile(const RPKGEntry& e, std::istream& stream, size_t &size);

//! Calculate size of RPKG archive header
LIBRANGER_API uint32_t calculateRPKGHeaderSize(const std::vector<RPKGEntry>& entryList);
//! Add data to RPKG archive
LIBRANGER_API void appendRPKGFile(std::ofstream& out, RPKGEntry& e, const char *data, uint32_t size, RPKGCompression compression);
//! Write RPKG archive header
LIBRANGER_API void writeRPKGHeader(std::ofstream& out, const std::vector<RPKGEntry>& entryList);

//! Convert text using iconv
LIBRANGER_API char* convertText(const char *to, const char *from, const char *what, int srcLength, int &destLength);

//! Convert text from charset to wstring
LIBRANGER_API std::wstring fromCodec(const char *codec, const char *text, int length = -1);
//! Convert text from UTF-8 to wstring
LIBRANGER_API std::wstring fromUTF8(const char *text, int length = -1);
//! Convert text from ASCII to wstring
LIBRANGER_API std::wstring fromASCII(const char *text, int length = -1);
//! Convert text from Local charset to wstring
LIBRANGER_API std::wstring fromLocal(const char *text, int length = -1);

//! Convert wstring to charset
LIBRANGER_API std::string toCodec(const char *codec, const std::wstring& text);

//! Convert wstring to UTF-8
LIBRANGER_API std::string toUTF8(const std::wstring& text);
//! Convert wstring to ASCII
LIBRANGER_API std::string toASCII(const std::wstring& text);
//! Convert wstring to local charset
LIBRANGER_API std::string toLocal(const std::wstring& text);

//! Split string
LIBRANGER_API std::vector<std::wstring> split(const std::wstring& s, wchar_t c);
//! Remove leading and trailing whitespace/tabs/newlines from a string
LIBRANGER_API std::wstring trim(const std::wstring& s);
//! Get file extension
LIBRANGER_API std::wstring suffix(const std::wstring& s);
//! Get file basename
LIBRANGER_API std::wstring basename(const std::wstring& s);
//! Get file dir
LIBRANGER_API std::wstring directory(const std::wstring& s);

//! Split string
LIBRANGER_API std::vector<std::string> split(const std::string& s, char c);
//! Remove leading and trailing whitespace/tabs/newlines from a string
LIBRANGER_API std::string trim(const std::string& s);
//! Get file extension
LIBRANGER_API std::string suffix(const std::string& s);
//! Get file basename
LIBRANGER_API std::string basename(const std::string& s);
//! Get file dir
LIBRANGER_API std::string directory(const std::string& s);

//! Create directory path
LIBRANGER_API bool createDirPath(const std::wstring& path);
//! Check directory writable
LIBRANGER_API bool checkDirWritable(const std::wstring& path);

LIBRANGER_API PNGFrame loadPNG(const char *buffer, size_t bufferSize);
LIBRANGER_API PNGFrame loadPNG(std::istream &stream);
}

#endif
