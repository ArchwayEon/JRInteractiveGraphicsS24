#pragma once
#include <string>

class TextFile
{
private:
	std::string data;

public:
	TextFile() : data("") {}
	~TextFile() = default;

	bool Read(const std::string& filePath);
	inline const std::string& GetData() const { return data; }
protected:
	void Trim(std::string& str);
};

