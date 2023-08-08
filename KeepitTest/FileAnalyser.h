#pragma once
#include <filesystem>

class TextParser;

class FileAnalyser
{
public:
	explicit FileAnalyser(const std::filesystem::path& path);

	uint64_t StartProcessing();

private:
	void DefineChunkSize();
	uint64_t ProcessFile();

private:
	const std::filesystem::path m_filePath;
	unsigned m_threadCount;
	const size_t m_defaultChunkSize{ 0x20000000 }; //Minimum chunk size is 1gb 
	size_t m_chunkSize;

	std::unique_ptr<TextParser> m_parser;

};
