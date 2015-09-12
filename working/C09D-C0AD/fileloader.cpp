#include "FileLoader.h"

FileLoader::FileLoader(const std::string& filepath, const char* access) : _fp{ fopen(filepath.c_str(), access) }
{
	if (_fp == nullptr)
		throw new std::runtime_error("Could not open file " + filepath + " for access " + access);
}

FileLoader::~FileLoader()
{
	fclose(_fp);
}

FILE* FileLoader::operator()()
{
	return _fp;
}