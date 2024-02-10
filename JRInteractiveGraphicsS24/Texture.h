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
	void SelectToChange();
	void SelectToRender(int textureUnit = 0);
	void Allocate();

private:
	void CleanUp();
};

