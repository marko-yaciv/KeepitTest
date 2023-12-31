#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <mutex>

class TextParser
{
public:
	TextParser() = default;

	void Parse(const std::vector<char> chunk);
	
	uint64_t GetWordsCount();
private:
	void UpdateWordCount(std::unordered_set<std::string>&& localWords);

	std::mutex m_writeMutex;
	std::unordered_set<std::string> m_wordsToCount;
};

