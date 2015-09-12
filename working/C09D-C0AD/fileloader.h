#pragma once
#include <fstream>

class FileLoader
{
public:
	FileLoader(const std::string& filepath, const char* access);
	~FileLoader();
	FILE* operator()();
private:
	FILE* _fp;
};