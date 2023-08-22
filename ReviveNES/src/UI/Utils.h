#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace NESEmu {

	struct DataBuffer {
		uint8_t* Data;
		size_t Size;

		DataBuffer()
			:	Data(NULL),
				Size(0)
		{}

		DataBuffer(uint8_t* data, size_t size)
			:	Data(data),
				Size(size)
		{}
	};

	errno_t ReadFile(const char* fileName, DataBuffer& outBuffer);
	errno_t WriteFile(const char* fileName, DataBuffer buffer);
	void DeleteBuffer(DataBuffer& buffer);

}