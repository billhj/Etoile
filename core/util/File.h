/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file File.h
* @brief 
* @date 1/2/2011
*/

#pragma once
/*
This should be used for loading a file.
It will Load the file into memory where it can be processed.
Will handle the Endiannes automatically


the following should be used during file reading instead of a simple !eof()
while (ss.getline(s, 512).fail() && !ss.eof() && !ss.bad())
*/

#include <string>

namespace Etoile
{

	class File
	{
	public:
		/// This should be left the only constructor
		File( );
		virtual ~File( );


		virtual bool loadFile( const std::string& refFile );

		const char* getByteContent( size_t& refSize ) const { refSize = _fileSize; return _pByteData; }

		const char* getByteContent( ) const { return _pByteData; }
		size_t  getSize() const { return _fileSize; }


		// static methods
		static bool fileExists( const std::string& refFile );
		static size_t getFileSize( const std::string& refFile );
		static bool	loadIntoMemoryChunk( const std::string& refFile, void* pDestination );
		static bool	isAbsolutePath( const std::string& refPathName );



		static std::string	getFileName( const std::string& refFile );
		/**
		* @brief To retrieuve the extension of a filename
		* @param refFile The filename from which the extension should be copied to refDestExtension )
		* @param bToLower Set to true if the extensions should be converted to lower case
		*/
		static std::string	getFileExtension( const std::string& refFile, bool bToLower = true );

		/**
		* @brief Cuts the extension off (also the ".");
		* @param refFile The filename from which the filename should be cut from the extension
		*/
		static std::string	getFileNameWithoutExtension( const std::string& refFile );

		static std::string	getFilePath( const std::string& refFile );


	private:
		char*					_pByteData;
		size_t					_fileSize;

	};


}

