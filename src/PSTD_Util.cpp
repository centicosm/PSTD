
#include "pstd_util.h"

//using namespace PSTD;



std::string PSTD::Make_UpperStr(const std::string &str) {
	std::string s = str;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}



std::vector<std::string> PSTD::String_Split(std::string line, char delim, bool removeWhitespace) {
	std::vector<std::string> fields;

	bool lookingForStart = true;
	size_t startI = 0;


	int startIndex = 0;
	for (size_t i = 0; i < line.length(); i++) {
		if ((line[i] == ' ') || (line[i] == '\n')) {
			startIndex++;
		}
		else {
			break;
		}
	}

	for (size_t i = startIndex; i < line.length(); i++) {

		// we are looking for the beginning of a field defined as the first character after a delimiter
		if (lookingForStart) {
			
			if (line[i] == ' ') {
				if (removeWhitespace) {
					startI++;
				}
				else {
					startI = i;
					lookingForStart = false;
				}
			}
			else if (line[i] == delim) {
				startI++;
				std::string empty = "";
				fields.push_back(empty);

			}
			else {
				startI = i;
				lookingForStart = false;
			}
		}

		// we found the starting delimiter now look for the end of the field
		else {
			if ((line[i] == delim) || (line[i] == '\n')) {
				
				std::string field = line.substr(startI, i - startI);
				fields.push_back(field);
				startI = i;
				lookingForStart = true;
			}
		}		
	}

	// if we ended the parsing mid field, the characters after the last delimiter up to the end are a field
	if ((!lookingForStart) && (startI != line.length()) && (startI != startIndex)) {
		std::string field = line.substr(startI);
		fields.push_back(field);
	}

	return fields;
}


std::vector<std::string> PSTD::Read_FileLinesToVector(const std::string &fname, char commentPrefix) {
	std::vector<std::string> outLines;

	FILE *infile = fopen(fname.c_str(), "r");
	if (!infile) {
		return outLines;
	}

	char buf[256];
	while (!feof(infile)) {
		//memset(buf, 0, 256);
		if (!fgets(buf, 255, infile)) {
			fclose(infile);
			return outLines;
		}
		if (buf[0] != commentPrefix) {}
		size_t len = strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = 0;
		len--;

		// make sure the line could contain valid data
		if (len != 0) {
			outLines.push_back(std::string(buf));
		}
	}
	fclose(infile);
	return outLines;
}



// based on glm code
short Pack_FloatAsShort(float toConv) {
	union {
		float f;
		int i;
	} fconv;

	fconv.f = toConv;

	int s = (fconv.i >> 16) & 0x00008000;
	int e = ((fconv.i >> 23) & 0x000000ff) - (127 - 15);
	int m = fconv.i & 0x007fffff;

	if (e <= 0) {
		if (e < -10) {
			//
			// E is less than -10.  The absolute value of f is
			// less than half_MIN (f may be a small normalized
			// float, a denormalized float or a zero).
			//
			// We convert f to a half zero.
			//

			return short(s);
		}

		//
		// E is between -10 and 0.  F is a normalized float,
		// whose magnitude is less than __half_NRM_MIN.
		//
		// We convert f to a denormalized half.
		// 

		m = (m | 0x00800000) >> (1 - e);

		//
		// Round to nearest, round "0.5" up.
		//
		// Rounding may cause the significand to overflow and make
		// our number normalized.  Because of the way a half's bits
		// are laid out, we don't have to treat this case separately;
		// the code below will handle it correctly.
		// 

		if (m & 0x00001000)
			m += 0x00002000;

		//
		// Assemble the half from s, e (zero) and m.
		//

		return short(s | (m >> 13));
	}

	else if (e == 0xff - (127 - 15)) {
		if (m == 0) {

			//
			// F is an infinity; convert f to a half
			// infinity with the same sign as f.
			//
			return short(s | 0x7c00);
		}
		else {
			//
			// F is a NAN; we produce a half NAN that preserves
			// the sign bit and the 10 leftmost bits of the
			// significand of f, with one exception: If the 10
			// leftmost bits are all zero, the NAN would turn 
			// into an infinity, so we have to set at least one
			// bit in the significand.
			//

			m >>= 13;

			return short(s | 0x7c00 | m | (m == 0));
		}
	}

	//
	// E is greater than zero.  F is a normalized float.
	// We try to convert f to a normalized half.
	//
		//
	// Round to nearest, round "0.5" up
	//
	if (m & 0x00001000)	{
		m += 0x00002000;

		if (m & 0x00800000)	{
			m = 0;     // overflow in significand,
			e += 1;     // adjust exponent
		}
	}

	//
	// Handle exponent overflow
	//
	if (e > 30) {
		//overflow();        // Cause a hardware floating point overflow;

		return short(s | 0x7c00);
		// if this returns, the half becomes an
	}   // infinity with the same sign as f.

	//
	// Assemble the half from s, e and m.
	//

	return short(s | (e << 10) | (m >> 13));
}



unsigned int PSTD::Pack_TwoFloatAsUInt(float x, float y) {

	short num1 = Pack_FloatAsShort(x);
	short num2 = Pack_FloatAsShort(y);

	unsigned int res2 = num2;
	res2 = res2 << 16;

	unsigned int res1 = 0;
	res1 |= num1;
	res1 &= 0x0000FFFF;
	
	return (res1 | res2); 
} 


unsigned int PSTD::Pack_FourBytesAsUInt(unsigned int *bytes) {
	unsigned int a = bytes[0];
	unsigned int b = bytes[1];
	unsigned int c = bytes[2];
	unsigned int d = bytes[3];

	unsigned int val = (d << 24) | (c << 16) | (b << 8) | a;
	return val;
}












