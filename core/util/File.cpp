/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file File.cpp
* @brief 
* @date 1/2/2011
*/

#include "File.h"
#include <algorithm>
#include <fstream>
#include <sys/stat.h>			/// For file stats
#include <assert.h>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif


namespace Etoile
{

	bool File::fileExists( const std::string& refFile )
	{
		std::ifstream streamFile( refFile.c_str(), std::ios::in | std::ios::binary );
		if ( streamFile.is_open() ) {
			streamFile.close();
			return true;
		}
		else {
			return false;
		}
	}

	/**
	* @brief Returns the size in bytes of the file
	* @param refFile Filename of the file to check
	* static method
	*/
	size_t File::getFileSize( const std::string& refFile )
	{
		struct stat filestatus;
		stat( refFile.c_str(), &filestatus );
		return (size_t) filestatus.st_size;
	}

	/**
	* @brief Loads a file into given memory address
	* @param refFile The Filename
	* @param pDestination Destination in memory
	* You should call GetFileSize before!
	* static method
	*/
	bool File::loadIntoMemoryChunk( const std::string& refFile, void* pDestination )
	{
		if ( !fileExists( refFile ) || !pDestination ) {
			assert( 0 );
			return false;
		}

#if 0 
		// Some files stats
		struct stat fileInfo;
		if ( stat( refFile.c_str(), &fileInfo ) != 0 ) {
			// Some error

		}
		else {
			size_t iSizeInBytes = fileInfo.st_size;			// Size in Bytes
			_dev_t dDeviceNumber = fileInfo.st_dev;			// Device Number
			char cDeviceNumber = ( (char) dDeviceNumber ) + 'A';
			__time32_t tCreationTime = fileInfo.st_ctime;	// Creation Time
			__time32_t tLastModTime = fileInfo.st_mtime;	// Last modification

			int foo = 0;
		}
#endif // 0


		std::ifstream streamFile( refFile.c_str(), std::ios::in | std::ios::binary );

		if ( streamFile.is_open() ) {
			streamFile.seekg( 0, std::ios::end );										// go to the end
			std::ifstream::pos_type lFileSize = streamFile.tellg();						// get the end position

			streamFile.seekg( 0, std::ios::beg );										// go to beginning of the file

			// read into the memory chunk
			streamFile.read( (char*) pDestination, lFileSize );

			// no saving since this is the static method
			// _fileSize = lFileSize;

			streamFile.close();
			return true;
		}
		else {
			assert( 0 );
			return false;
		}
	}


	std::string	File::getFileName( const std::string& refFile) 
	{
		/*std::string strReturn;*/
		size_t nPos1 = refFile.find_last_of( '/' );
		size_t nPos2 = refFile.find_last_of( '\\' );
		size_t nPos = std::max< int >( nPos1, nPos2 );

		if ( nPos == std::string::npos ) {
			return refFile;
		}
		else {
			return std::string( refFile.substr(nPos + 1, refFile.size()));
		}

	}

	/**
	* @brief For extracting the extension of a filename
	* @param refFile The filename
	* Guaranteed to be lower case return value
	*/
	std::string File::getFileExtension( const std::string& refFile, bool bToLower )
	{
		std::string strReturn;
		size_t nPos = refFile.find_last_of( '.' ) + 1;
		size_t iExtLength = refFile.length() - nPos;
		strReturn.insert( 0, refFile.c_str(), nPos, iExtLength );
		if ( bToLower ) {
			std::transform( strReturn.begin(), strReturn.end(), strReturn.begin(), tolower );
		}
		return strReturn;
	}

	std::string File::getFileNameWithoutExtension( const std::string& refFile )
	{
		std::string fileName = getFileName(refFile);
		size_t nPos = fileName.find_last_of( '.' );
		return fileName.substr(0,nPos);
	}

	std::string File::getFilePath( const std::string& refFile )
	{
		size_t nPos1 = refFile.find_last_of( '/' );
		size_t nPos2 = refFile.find_last_of( '\\' );
		size_t nPos = std::max< int >( nPos1, nPos2 );

		if ( nPos == std::string::npos ) {
			return std::string("./");
		}
		else {
			return std::string( refFile.substr(0, nPos + 1) );
		}
	}

	bool File::isAbsolutePath( const std::string& refPathName )
	{
		return ( refPathName.find_first_of( ':' ) != std::string::npos );
	}

	File::File( ) :
	_pByteData( 0 ),
		_fileSize( 0 )
	{
	}

	File::~File( )
	{
		delete _pByteData;
	}


	bool File::loadFile( const std::string& refFile )
	{
		// Release if something else
		// is already in memory
		if ( _pByteData ) {
			delete( _pByteData );
			_fileSize = 0;
		}

		if ( !fileExists( refFile ) ) {
			//MxAssert( 0 );
			return false;
		}


#if 0 
		// Some files stats
		struct stat fileInfo;
		if ( stat( refFile.c_str(), &fileInfo ) != 0 ) {
			// Some error

		}
		else {
			size_t iSizeInBytes = fileInfo.st_size;			// Size in Bytes
			_dev_t dDeviceNumber = fileInfo.st_dev;			// Device Number
			char cDeviceNumber = ( (char) dDeviceNumber ) + 'A';
			__time32_t tCreationTime = fileInfo.st_ctime;	// Creation Time
			__time32_t tLastModTime = fileInfo.st_mtime;	// Last modification

			int foo = 0;
		}
#endif // 0


		std::ifstream streamFile( refFile.c_str(), std::ios::in | std::ios::binary );

		if ( streamFile.is_open() ) {
			streamFile.seekg( 0, std::ios::end );										// go to the end
			std::ifstream::pos_type lFileSize = streamFile.tellg();						// get the end position

			streamFile.seekg( 0, std::ios::beg );										// go to beginning of the file

			// reserve one extra byte for the 0 termination
			lFileSize = lFileSize + std::ifstream::pos_type( 1 );

			// request enough memory
			_pByteData = new char[ lFileSize ];
			if ( !_pByteData ) {
				assert( 0 );
				return false;
			}

			streamFile.read( _pByteData, lFileSize );

			// make it zero terminated for all file-parsing stuff
			_pByteData[ lFileSize - std::ifstream::pos_type( 1 ) ] = 0;
			_fileSize = lFileSize - std::ifstream::pos_type( 1 );

			streamFile.close();
			return true;
		}
		else {
			return false;
		}
	}

}
