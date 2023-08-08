#include <iostream>
#include "FileAnalyser.h"
#include <chrono>

int main(int argc, char** argv) 
try {
	const int numOfArguments = 2;
	
	if(argc != numOfArguments) 
	{
		throw std::invalid_argument("Please provide file path as an argument to the app");
	}

	const std::string path {argv[1]};
	std::cout << " File path is: " << path << std::endl;

	FileAnalyser analyser(path);
	uint64_t result = analyser.StartProcessing();
	auto end = std::chrono::steady_clock::now();
	std::cout << "Count of unique words: " << result << std::endl;
	return 0;
}
catch (const std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
}