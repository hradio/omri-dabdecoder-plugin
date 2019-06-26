#ifndef MPGDECODER_H
#define MPGDECODER_H

#include "mpg123.h"

class MpgDecoder {

public:
	MpgDecoder();
	~MpgDecoder();

	size_t decode(u_int8_t* audioData, int length);
};

#endif // MPGDECODER_H
