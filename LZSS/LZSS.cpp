#include"Header.h"
#include<string>
#include<vector>
#include<fstream>




LZSS::LZSS(std::string s, short dl, short bl) : input(s), dictionaryLength(dl), bufferLength(bl) {
	// initialize dictionary:
	// length: dl (default=4)
	// content: first letter of the input string (s[0])
	// example: input=aabsdhjgmatiwozignoj => dictionary = aaaa 
	dictionary.assign(dl, s[0]);
}

LZSS::LZSS(short dl, short bl) : dictionaryLength(dl), bufferLength(bl) {
	input = readTextFile();
	dictionary.assign(dl, input[0]);
}

void LZSS::changeParameters(short dl, short bl)
{
	this->dictionaryLength = dl;
	this->bufferLength = bl;
	this->dictionary.assign(dl, this->input[0]);
}

std::string LZSS::readTextFile() {
	std::string inFileName = "text.txt";
	std::ifstream inFile;
	inFile.open(inFileName);

	std::string line;

	if (inFile.is_open()) {

		while (!inFile.eof()) {
			getline(inFile, line);
		}
			std::cout << line << std::endl;
			inFile.close();
	}
	else {
		std::cout << "Cannot open the file: " << inFileName << ". Setting the input string to default" << std::endl;
		line = "aabbcabbcabd";
	}

	return line;
}

void LZSS::decompress() {
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << "Decompression begins..." << std::endl;
	std::string outputString{ "" };
	dictionary.assign(this->dictionaryLength, input[0]);
	std::string chain = dictionary;

	for (auto i : encoded) {
		if (i.flag == true) {						// printing a pair (encoded flag, first unencoded symbol)
			outputString += i.charNotFound;			// there was no sequence that starts with i.charNotFound in the dictionary
			chain += i.charNotFound;
		}
		else
		{
			outputString += chain.substr(i.offset, i.length);	// print triple (encoded flag, offset(indeks początku podciągu), length)
			chain += chain.substr(i.offset, i.length);
		}
		chain.erase(0, i.length);  // update the chain
	}

	std::cout << "###############################" << std::endl;

	std::cout << "Decoded string: " << std::endl;
	std::cout << outputString << std::endl;
}

void LZSS::compress() {
	std::cout << "Compression begins..." << std::endl;
	std::string chain = dictionary + input;

	for (int i = 0; i < input.size();) { // i will be incremented by the value of length of the last encoded 
		//dictionary.resize(chain.length());

		// update the dictionary
		dictionary = chain.substr(i, dictionaryLength); 

		//initialize buffer that will be later used to find LCS with dictionary (default length of buffer=4)
		std::string buffer = input.substr(i, bufferLength);

		// find the longest common subsequence, until there is data in the buffer
		// if nothing is found, stding::find() will return string::npos <- termination condition
		while (dictionary.find(buffer) == std::string::npos && buffer.length() > 1)
			buffer.pop_back();

		// if nothing found: match is less than minimum allowable match length
		if (dictionary.find(buffer) == std::string::npos) { // storing the uncoded flag and the first uncoded symbol
			coder.flag = true;								// flag 
			coder.length = buffer.length();					// buffer will be moved by one position
			coder.offset = dictionary.find(buffer);			// 
			coder.charNotFound = buffer;					// in dictionary there is no sequence starting with letter that is left in the buffer
			this->encoded.push_back(coder);
		}
		else   // match found: storing encoded flag, the offset and length
		{
			coder.flag = false;
			coder.offset = dictionary.find(buffer);			// find returns the position of the first character of the first match
			coder.length = buffer.length();					// length of the found LCS
			coder.charNotFound = "";	
			this->encoded.push_back(coder);
		}
		
		i += buffer.length();								// updating the size of actually encoded string
	}
	this->showEncoded();

	this->decompress();
}

void LZSS::showEncoded() const{

	for (auto i : encoded)
	{
		if (!i.flag)
			std::cout << static_cast<int>(i.flag) << i.offset << i.length << std::endl;
		else
			std::cout << static_cast<int>(i.flag) << i.charNotFound << std::endl;

	}

}