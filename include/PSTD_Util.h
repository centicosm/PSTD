
#pragma once

#ifndef PSTD_UTIL_H
#define PSTD_UTIL_H

#include <string>
#include <vector>
#include <algorithm>
#include "PVector3d.hpp"


namespace PSTD {


	
	std::string Make_UpperStr(const std::string &str);


	// structure for packed normals
	union Packed1010102 {
		struct {
			int X : 10;
			int Y : 10;
			int Z : 10;
			int W : 2;
		} Ele;
		unsigned int iVal;
	};


	std::vector<std::string> String_Split(std::string line, char delim, bool removeWhitespace = true);
	std::vector<std::string> Read_FileLinesToVector(const std::string &fname, char commentPrefix = '#');
	unsigned int Pack_TwoFloatAsUInt(float x, float y);
	unsigned int Pack_FourBytesAsUInt(unsigned int *bytes);

	inline unsigned short PackTo_UnsignedShort(float x) { return (unsigned short)(x * 65535); }

	inline unsigned int PackTo_2_10_10_10Unsigned(float x, float y, float z) {
		unsigned int packed = 0;
		unsigned int xc = (unsigned int)(x * 255.0f);
		unsigned int yc = (unsigned int)(y * 255.0f);
		unsigned int zc = (unsigned int)(z * 255.0f);

		packed = xc | (yc >> 10) | (zc >> 20);
		return packed;
	}


	inline PVector3df Unpack_2_10_10_10Signed(unsigned int i) {
		Packed1010102 packed;
		packed.iVal = i;

		PVector3df v;
		v.X = (float)(2 * packed.Ele.X + 1) / 1023.0f;
		v.Y = (float)(2 * packed.Ele.Y + 1) / 1023.0f;
		v.Z = (float)(2 * packed.Ele.Z + 1) / 1023.0f;
		return v;
	}


	inline unsigned int PackTo_2_10_10_10Signed(float x, float y, float z) {
		Packed1010102 packed;

		//(2c + 1) / (2 ^ 10 - 1)
		packed.Ele.X = int(round(x * 511.f));
		packed.Ele.Y = int(round(y * 511.f));
		packed.Ele.Z = int(round(z * 511.f));
		packed.Ele.W = 0;
		return packed.iVal;
	}


	inline unsigned int PackTo_2_10_10_10Signed(PVector3df &v) {
		return PackTo_2_10_10_10Signed(v.Z, v.Y, v.X);
	}


	inline unsigned int PackTo_Signed(float x, float y, float z) {
		unsigned char r = (unsigned char)((x + 1.0f) / 2.0f * 255);
		unsigned char g = (unsigned char)((y + 1.0f) / 2.0f * 255);
		unsigned char b = (unsigned char)((z + 1.0f) / 2.0f * 255);
		unsigned char a = 0;
		return ((a << 24) | (r << 16) | (g << 8) | (b));
	}
};


#endif
