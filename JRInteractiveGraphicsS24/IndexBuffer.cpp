#include "IndexBuffer.h"
#include <cstdarg>

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &iboId);
}

void IndexBuffer::AddIndexData(unsigned int count, ...)
{
	va_list args;
	va_start(args, count);
	while (count > 0) {
		indexData.push_back(static_cast<unsigned short>(va_arg(args, int)));
		count--;
	}
	va_end(args);
}

void IndexBuffer::Allocate()
{
	if (isDynamic == false) {
		unsigned long long bytesToAllocate =
			indexData.size() * sizeof(unsigned short);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, bytesToAllocate, indexData.data(),
			GL_STATIC_DRAW);
	}else{
		unsigned long long bytesToAllocate =
			maxNumberOfIndices * sizeof(unsigned short);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, bytesToAllocate, nullptr,
			GL_DYNAMIC_DRAW);
	}
	
}

void IndexBuffer::SendData()
{
	unsigned long long sizeInBytes = indexData.size() * sizeof(unsigned short);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeInBytes, indexData.data());
}
