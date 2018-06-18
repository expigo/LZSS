#pragma once
#include<iostream>
#include<string>
#include<vector>


class LZSS {

	static const short dictLength = 8;
	static const short bufLength = 8;

private:
	short dictionaryLength;
	short bufferLength;
	std::string input;
	std::string dictionary;
	struct output
	{
		bool flag = true;
		short length = 0;
		std::size_t offset = 0;
		std::string charNotFound = "fasd";

	}coder;

	std::vector<output> encoded;

	void showEncoded() const;
	void decompress();
	std::string readTextFile();

public:
	explicit LZSS(std::string s, short dl = dictLength, short bl = bufLength);
	explicit LZSS(short dl = dictLength, short bl = bufLength);
	void changeParameters(short, short);
	void compress();


};