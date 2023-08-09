#include "FileAnalyser.h"
#include "thread_pool.h"
#include <thread>
#include <fstream>

FileAnalyser::FileAnalyser(const std::filesystem::path& path)
	: 
	m_filePath(path), 
	m_parser(new TextParser())
{
	m_chunkSize = m_defaultChunkSize;
}

uint64_t FileAnalyser::StartProcessing()
{
	DefineChunkSize();
	return ProcessFile();
}

void FileAnalyser::DefineChunkSize()
{
	uint64_t sizeInBytes = std::filesystem::file_size(m_filePath);
	unsigned threadCount = std::thread::hardware_concurrency();
	m_chunkSize = std::min(sizeInBytes / threadCount, m_defaultChunkSize);
}

uint64_t FileAnalyser::ProcessFile()
{
	std::ifstream file(m_filePath, std::ios::binary);
	if (!file) 
	{
		throw std::exception("Failed to open file");
	}

	//std::vector<std::thread> threads;
	dp::thread_pool pool(std::thread::hardware_concurrency());
	std::vector<std::future<void>> res;
	while (true) 
	{
		std::vector<char> buffer(m_chunkSize);
		file.read(buffer.data(), m_chunkSize);
		
		if (!file.good() && file.gcount() == 0) break;

		auto lastSpaceIt = std::find_if(buffer.rbegin(), buffer.rend(),
			[](char c) 
			{ 
				return std::isspace(c); 
			});
		std::size_t newChunkSize = lastSpaceIt == buffer.rbegin() ? m_chunkSize : std::distance(lastSpaceIt, buffer.rend());	
		if (newChunkSize != m_chunkSize) 
		{
			buffer.resize(newChunkSize);
			int a = file.tellg();
			file.seekg(newChunkSize - m_chunkSize, std::ios::cur);
		}

		//threads.emplace_back(std::thread(&TextParser::Parse, m_parser.get(), std::move(buffer)));
		auto task = [this](std::vector<char> buffer) 
		{
			m_parser->Parse(std::move(buffer)); 
		};
		res.push_back(pool.enqueue(task, std::move(buffer)));
	}

	file.close();
	for (auto& f : res) 
	{
		f.get();
	}

	return m_parser->GetWordsCount();
}
