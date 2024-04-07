#pragma once
#include "BaseObject.h"
#include <vector>
#include <glad/glad.h> 

class IndexBuffer :  public BaseObject
{
protected:
    unsigned int iboId;
    std::vector<unsigned short> indexData;
    bool isDynamic = false;
    unsigned int maxNumberOfIndices = 0;

public:
    IndexBuffer();
    ~IndexBuffer() = default;

    unsigned int GetBufferId() const { return iboId; }

    inline void Select() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId); }
    inline void Deselect() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

    std::vector<unsigned short>& GetIndexData() { return indexData; }
    inline std::size_t GetNumberOfIndices() const { return indexData.size(); }

    void AddIndexData(unsigned int count, ...);
    void AddIndexData(unsigned short index) { indexData.push_back(index); }

    void Allocate();

    void SetIsDynamic(bool isDynamic) { this->isDynamic = isDynamic; }
    bool IsDynamic() const { return isDynamic; }
    void SetMaxNumberOfIndices(unsigned int maxNumberOfIndices) {
        this->maxNumberOfIndices = maxNumberOfIndices;
    }
    unsigned int GetMaxNumberOfIndices() const {
        return maxNumberOfIndices;
    }
    void SendData();

    void Clear() { indexData.clear(); }
};

