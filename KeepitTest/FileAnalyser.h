#pragma once
#include <filesystem>
#include "TextParser.h"

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
	//Default chunk size is 512mb. Adjust for better perfomance due to disk read speed and heap size.
	const size_t m_defaultChunkSize{ 0x2000000 };
	size_t m_chunkSize;

	std::unique_ptr<TextParser> m_parser;

};
