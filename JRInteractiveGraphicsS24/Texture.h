#pragma once
#include "BaseObject.h"
class Texture :  public BaseObject
{
private:
	unsigned char* textureData;
	bool isLoadedFromFile;
	unsigned int textureId;
	unsigned int type;
	unsigned int width, height;
	unsigned int internalFormat, sourceFormat;
	unsigned int wrapS, wrapT;
	unsigned int magFilter, minFilter;
	int numberOfChannels;

public:
	Texture();
	~Texture();

	inline const unsigned char* GetTextureData() const {
		return textureData;
	}
	inline bool IsLoadedFromFile() const {
		return isLoadedFromFile;
	}
	void SetTextureData(unsigned int count, unsigned char* data);
	inline unsigned int GetTextureId() const { return textureId; }
	void SetDimension(unsigned int width, unsigned int height);
	void SetWrapS(unsigned int wrapS) { this->wrapS = wrapS; }
	void SetWrapT(unsigned int wrapT) { this->wrapT = wrapT; }
	void SetMagFilter(unsigned int magFilter) { this->magFilter = magFilter; }
	void SetMinFilter(unsigned int minFilter) { this->minFilter = minFilter; }
	void SelectToChange();
	void SelectToRender(int textureUnit = 0);
	void Allocate();
	inline int GetNumberOfChannels() const { return numberOfChannels; }
	void LoadTextureDataFromFile(const std::string& filepath);

private:
	void CleanUp();
};

