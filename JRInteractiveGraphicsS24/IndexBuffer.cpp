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
