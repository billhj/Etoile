/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PFMImageLoader.cpp
* @brief 
* @date 1/2/2011
*/

#include "PFMImageLoader.h"
#include "../File.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>

namespace Etoile
{
	PFMImageLoader::PFMImageLoader()
	{
	}

	bool PFMImageLoader::loadImageFromFile(const std::string& strFilename, Image& image)
	{

		File imgFile;
		if ( !imgFile.loadFile( strFilename ) ) {
			//assert( 0 );
			return false;
		}

		size_t sFileSize = imgFile.getSize();
		const char* pByteData = imgFile.getByteContent();

		if ( sFileSize < 11 || !pByteData || pByteData[ 0 ] != 'P' ) {
			// Smallest pfm file must have at least 11 bytes: PF_1_1_(float)
			// Invalid Content
			// Not a pfm file
			assert( 0 );
			return false;
		}
		bool bIsGreyScale = false;

		if ( pByteData[ 1 ] == 'f' ) {
			bIsGreyScale = true;
		}
		std::string strHeader;
		size_t sPos = 2;
		// Go to the first number:
		while( !( pByteData[ sPos ] > 0x2F && pByteData[ sPos ] < 0x3A ) ) {
			if ( pByteData[ sPos ] == '#' ) {
				// go to the line ending
				while( !( pByteData[ sPos ] == 10 || pByteData[ sPos ] == 13 ) ) {
					++sPos;
				}
				break;
			}
			++sPos;
		}
		strHeader.clear();
		while( ( pByteData[ sPos ] > 0x2F && pByteData[ sPos ] < 0x3A ) ) {
			strHeader.push_back( pByteData[ sPos ] );
			++sPos;
		}

		int iWidth = 1;
		std::stringstream ssWidth;
		ssWidth << strHeader;
		ssWidth >> iWidth;

		strHeader.clear();
		while( ( pByteData[ sPos ] > 0x2F && pByteData[ sPos ] < 0x3A ) || pByteData[ sPos ] == ' ' ) {
			strHeader.push_back( pByteData[ sPos ] );
			++sPos;
		}

		int iHeight = 1;
		std::stringstream ssHeight;
		ssHeight << strHeader;
		ssHeight >> iHeight;
		while( pByteData[ sPos ] == 10 || pByteData[ sPos ] == 13 ) {
			++sPos;
		}

		strHeader.clear();
		while( !( pByteData[ sPos ] == 10 || pByteData[ sPos ] == 13 ) ) {
			strHeader.push_back( pByteData[ sPos ] );
			++sPos;
		}

		std::stringstream ssEndian;
		float fEndian = 1.f;
		ssEndian << strHeader;
		ssEndian >> fEndian;

		if ( pByteData[ sPos ] == 10 || pByteData[ sPos ] == 13 )
			++sPos;
		if ( pByteData[ sPos ] == 10 || pByteData[ sPos ] == 13 )
			++sPos;

		bool bIsLittleEndian = ( fEndian > 0.f );
		//const char* pRest = &pByteData[ sPos ];

		//int iNumberOfComponents = ( bIsGreyScale ? 1 : 3 );
		int iNumberOfComponents = 3;
		int iNumberOffloats = iWidth * iHeight * iNumberOfComponents;

		float* pfImgData = new float[ iNumberOffloats ];
		int dataSize = sizeof( float ) * iNumberOffloats;
		memset( pfImgData, 0, dataSize);

		int iValuesRead = 0;

		// Helper for controlling the endianness
		union floatOrChars
		{
			float fValue;
			char chars[ 4 ];
		};

		floatOrChars value;
		value.fValue = 0.f;  // Temp value for file reading
		while ( sPos < sFileSize && iValuesRead < iNumberOffloats ) {

			for ( int i=0; i<4; ++i ) {
				value.chars[ i ] = pByteData[ sPos ];
				++sPos;
			}
			if ( bIsLittleEndian ) {
				floatOrChars vTemp;
				vTemp = value;
				value.chars[ 0 ] = vTemp.chars[ 0 ];
				value.chars[ 1 ] = vTemp.chars[ 1 ];
				value.chars[ 2 ] = vTemp.chars[ 2 ];
				value.chars[ 3 ] = vTemp.chars[ 3 ];
			}

			pfImgData[ iValuesRead ] = value.fValue;
			++iValuesRead;

			if ( bIsGreyScale ) {
				pfImgData[ iValuesRead ] = value.fValue;
				++iValuesRead;
				pfImgData[ iValuesRead ] = value.fValue;
				++iValuesRead;
			}
		}

		image.setWidth(iWidth);
		image.setHeight(iHeight);

		std::vector<Vec4f>& data = image.getData();
		data.resize(iWidth * iHeight);

		for(int index = 0; index < iWidth * iHeight; ++index)
		{
			Vec4f pixel;

			pixel[0] = pfImgData[index * 3 + 0];
			pixel[1] = pfImgData[index * 3 + 1];
			pixel[2] = pfImgData[index * 3 + 2];
			pixel[3] = 1.0f;

			data[index] = pixel;
		}


		return true;
	}
}
