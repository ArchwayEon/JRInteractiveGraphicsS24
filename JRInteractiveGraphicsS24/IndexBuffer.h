#pragma once
#include "BaseObject.h"
#include <vector>
#include <glad/glad.h> 

class IndexBuffer :  public BaseObject
{
protected:
    unsigned int iboId;
    std::vector<unsigned short> indexData;

public:
    IndexBuffer();
    ~IndexBuffer() = default;

    unsigned int GetBufferId() const { return iboId; }

    inline void Select() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId); }
    inline void Deselect() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    const std::vector<unsigned short>& GetIndexData() const { return indexData; }
    inline std::size_t GetNumberOfIndexes() const { return indexData.size(); }

    void AddIndexData(unsigned int count, ...);
};

