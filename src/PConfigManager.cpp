#include <stdlib.h>
#include <string.h>
#include "pconfigmanager.h"
#include "PSTD_Util.h"
#include "GlobalLogger.h"

using namespace std;
using namespace PSTD;

PConfigManager::PConfigManager(void) {
	// nothing here
}


PConfigManager::~PConfigManager(void) {
}

// some size checking would be good
bool PConfigManager::Load_ConfigItems(const string &configfile) {
	FILE *infile;
	char buf1[100], buf2[100], buf3[200];
	int linenum = 1, postnum = 0, prenum;
	bool preflag;
	char curchar;
	char *res;

	infile = fopen(configfile.c_str(), "r");
	if (!infile) {
		return false;
	}

	// now go through each item and read it into a buffer and parse the buffer by hand
	do {
		bool comment = false;
		bool blob = false;
		preflag = false;
		prenum = 0;

		res = fgets(buf3, 200, infile);

		// if we have read a line
		if (res) {
			if ((res[0] == 10) || (res[0] == '\n') || (res[0] == 0))  {
				comment = true;
			}
			size_t slen = strlen(buf3);
			for (size_t cnt = 0; cnt < slen; cnt++) {
				curchar = buf3[cnt];

				// we will skip spaces
				if ((curchar != ' ') && (curchar != 0) && (curchar != '\n') && (curchar != 10) && (curchar != '\t')) {

					// if we already read the = sign then fill up the value buffer
					if (preflag) {

						// # indicates a comment
						if (curchar == '#') {
							cnt = slen;
							//postnum++;
						}

						// We got a opening bracket specifying a blob
						else if (curchar == '{') {
							list<string> blobBuf;
							bool doneBlob = false;
							while (!doneBlob) {
								res = fgets(buf3, 200, infile);
								if (!res) {
									GLNFERR("Reached end of file while reading blob: %s - Line: %d : %s", buf1, linenum, configfile.c_str());
									return false;
								}
								if (strchr(buf3, '}')) {
									doneBlob = true;
								}
								else {
									blobBuf.push_back(buf3);
								}
							}
							if (!Load_Blob(buf1, blobBuf)) {
								GLNFERR("Could not read blob from buffer: %s - Line %d : %s", buf1, linenum, configfile.c_str());
							}
							// we added the data to a blob so we want to skip the normal addition to the map by saying the line was a comment
							//   this should be refactored to be less hacky
							blob = true;
							cnt = slen;
						}

						else {
							buf2[postnum] = curchar;
							postnum++;
						}
					}

					// otherwise fill up the attribute buffer until we hit the equal sign
					else {
						if (curchar == '#') {
							cnt = slen;
							comment = true;
						}

						// we got an equal mark so any text in front of that is our attribute
						else if (curchar == '=') {
							buf1[prenum] = 0;
							preflag = true;
							postnum = 0;
						}
						else {
							buf1[prenum] = curchar;
							prenum++;
						}
					} // end attribute parsing
				} // end valid character routine
			} // end input buffer parsing
		}  // end line parsing

		// do some error checking to make sure both attribute and value are something

		// if we have invalid characters in front of a comment, no value for an attribute, or a garbage line
		if ((((!preflag) && (comment) && (prenum != 0)) || ((preflag) && (postnum == 0)) || ((!preflag) && (!comment) && (prenum != 0))) && (!blob)) {
			GLNFERR("Load_ConfigItems: (Syntax Error): Line %d : %s", linenum, configfile.c_str());
		}

		// and it is not a comment then throw it in the tree
		else if ((!comment) && (!blob) && (preflag) && (postnum != 0)) {
			buf2[postnum] = 0;
			ConfigParams[buf1] = buf2;
		}

		linenum++;
	} while (!feof(infile));

	fclose(infile);

	return true;
}


bool PConfigManager::Load_ConfigItems(const char *buffer, unsigned int size) {
	char buf1[100], buf2[100], buf3[200];
	int linenum = 1, postnum = 0, prenum;
	unsigned int index = 0;
	bool preflag;
	char curchar;
	int numRead;

	// now go through each item and read it into a buffer and parse the buffer by hand
	do {
		bool comment = false;
		bool blob = false;
		preflag = false;
		prenum = 0;

		if (buffer[index] == 0) return true;
		numRead = sscanf(&buffer[index], "%200[^\n]", buf3);

		// TODO: FIX THIS
		if (numRead == -1) return true;

		// if we have read a line
		if (numRead > 0) {

			size_t slen = strlen(buf3);

			// XXXX we need to fix this  there should be a better way of removing whitespace
			if (buf3[slen - 1] == 13) buf3[slen - 1] = 0;

			index += (unsigned int)(slen + 1); // need to include the newline
			if ((buf3[0] == 10) || (buf3[0] == '\n') || (buf3[0] == 0))  {
				comment = true;
			}
			//size_t slen = strlen(buf3);
			for (size_t cnt = 0; cnt < slen; cnt++) {
				curchar = buf3[cnt];

				// we will skip spaces
				if ((curchar != ' ') && (curchar != 0) && (curchar != '\n') && (curchar != 10) && (curchar != '\t')) {

					// if we already read the = sign then fill up the value buffer
					if (preflag) {

						// # indicates a comment
						if (curchar == '#') {
							cnt = slen;
							//postnum++;
						}

						// We got a opening bracket specifying a blob
						else if (curchar == '{') {
							list<string> blobBuf;
							bool doneBlob = false;
							while (!doneBlob) {
								numRead = sscanf(&buffer[index], "%200[^\n]", buf3);
								if (numRead < 1) {
									GLNFERR("Reached end of file while reading blob: %s - Line: %d", buf1, linenum);
									return false;
								}
								index += numRead;
								if (strchr(buf3, '}')) {
									doneBlob = true;
								}
								else {
									blobBuf.push_back(buf3);
								}
							}
							if (!Load_Blob(buf1, blobBuf)) {
								GLNFERR("Could not read blob from buffer's buffer: %s - Line %d", buf1, linenum);
							}
							// we added the data to a blob so we want to skip the normal addition to the map by saying the line was a comment
							//   this should be refactored to be less hacky
							blob = true;
							cnt = slen;
						}

						else {
							buf2[postnum] = curchar;
							postnum++;
						}
					}

					// otherwise fill up the attribute buffer until we hit the equal sign
					else {
						if (curchar == '#') {
							cnt = slen;
							comment = true;
						}

						// we got an equal mark so any text in front of that is our attribute
						else if (curchar == '=') {
							buf1[prenum] = 0;
							preflag = true;
							postnum = 0;
						}
						else {
							buf1[prenum] = curchar;
							prenum++;
						}
					} // end attribute parsing
				} // end valid character routine
			} // end input buffer parsing
		}  // end line parsing

		// do some error checking to make sure both attribute and value are something

		// if we have invalid characters in front of a comment, no value for an attribute, or a garbage line
		if ((((!preflag) && (comment) && (prenum != 0)) || ((preflag) && (postnum == 0)) || ((!preflag) && (!comment) && (prenum != 0))) && (!blob)) {
			GLNFERR("Load_ConfigItems (buffer): (Syntax Error): Line %d", linenum);
		}

		// and it is not a comment then throw it in the tree
		else if ((!comment) && (!blob) && (preflag) && (postnum != 0)) {
			buf2[postnum] = 0;
			ConfigParams[buf1] = buf2;
		}

		linenum++;
	} while (index < size);


	return true;
}



bool PConfigManager::Load_Blob(const string &blobName, list<string> &buf) {
	char buf1[100], buf2[100];
	int linenum = 1, postnum = 0, prenum;
	bool preflag;
	char curchar;

	PConfigManager pfm;

	auto lineIt = buf.begin();

	// now go through each item and read it into a buffer and parse the buffer by hand
	while (lineIt != buf.end()) {
		bool comment = false;
		preflag = false;
		prenum = 0;

		const char *res = lineIt->c_str();
		// if we have read a line
		if (res) {
			if ((res[0] == 10) || (res[0] == '\n') || (res[0] == 0))  {
				comment = true;
			}
			size_t slen = lineIt->size();
			for (size_t cnt = 0; cnt < slen; cnt++) {
				curchar = res[cnt];

				// we will skip spaces
				if ((curchar != ' ') && (curchar != 0) && (curchar != '\n') && (curchar != 10) && (curchar != '\t')) {

					//	 if we already read the = sign then fill up the value buffer
					if (preflag) {
						// # indicates a comment in the right hand side so skip the rest
						if (curchar == '#') {
							cnt = slen;
						}

						// otherwise fill the value buffer
						else {
							buf2[postnum] = curchar;
							postnum++;
						}
					} // end handling of the value side of the string

					// otherwise fill up the attribute buffer until we hit the equal sign
					else {

						// if there is a comment while parsing the variable side, skip everything
						if (curchar == '#') {
							cnt = slen;
							comment = true;
						}

						// we got an equal mark so any text in front of that is our attribute
						else if (curchar == '=') {
							buf1[prenum] = 0;
							preflag = true;
							postnum = 0;
						}

						// otherwise just fill the variable name buffer
						else {
							buf1[prenum] = curchar;
							prenum++;
						}
					}

				} // end valid character check

			} // end string parsing


			// if we have invalid characters in front of a comment, no value for an attribute, or a garbage line
			if (((!preflag) && (comment) && (prenum != 0)) || ((preflag) && (postnum == 0)) || ((!preflag) && (!comment) && (prenum != 0))) {
				GLNFERR("Load_Blob: (Syntax Error): Line %d : %s", linenum, blobName.c_str());
				return false;
			}

			// otherwise we have a comment or a valid attribute value pair
			else if (!comment) {
				buf2[postnum] = 0;
				pfm.ConfigParams[buf1] = buf2;
			}

		} // end valid string check

		++lineIt;
	} // end buffer iteration

	// add the blob to our lists of blobs of that type
	_Blobs[blobName].push_back(pfm);
	return true;
}





const string *PConfigManager::Get_ConfigItem(const string &item) const {

	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return NULL;
	}

	return &confit->second;
}


bool PConfigManager::Get_StringConfigItem(const std::string &item, std::string &val) const {
	auto confIt = ConfigParams.find(item);

	if (confIt == ConfigParams.end()) {
		return false;
	}

	val = confIt->second;
	return true;
}




bool PConfigManager::Get_IntConfigItem(const std::string &item, int &val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}

	int myint = atoi(confit->second.c_str());
	val = myint;
	return true;
}


bool PConfigManager::Get_UIntConfigItem(const std::string &item, unsigned int &val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}

	unsigned int myint = strtoul(confit->second.c_str(), NULL, 10);
	val = myint;
	return true;
}


bool PConfigManager::Get_FloatConfigItem(const std::string &item, float &val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}

	float myfloat = strtof(confit->second.c_str(), NULL);
	val = myfloat;
	return true;
}

bool PConfigManager::Get_Vector4dfConfigItem(const std::string &item, float *val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}
	int numRes = sscanf(confit->second.c_str(), "(%f,%f,%f,%f)", &val[0], &val[1], &val[2], &val[3]);
	if (numRes != 4) return false;
	return true;
}


bool PConfigManager::Get_Vector3dfConfigItem(const std::string &item, float *val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}
	int numRes = sscanf(confit->second.c_str(), "(%f,%f,%f)", &val[0], &val[1], &val[2]);
	if (numRes != 3) return false;
	return true;
}



bool PConfigManager::Get_BoolConfigItem(const std::string &key, bool &val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(key);

	if (confit == ConfigParams.end()) {
		return false;
	}

	if (Make_UpperStr(confit->second) == "TRUE") {
		val = true;
		return true;
	}
	else if (Make_UpperStr(confit->second) == "FALSE") {
		val = false;
		return true;
	}

	return false;
}


size_t PConfigManager::Get_NumBlob(const std::string &name) const {

	auto blobIt = _Blobs.find(name);
	if (blobIt == _Blobs.end()) return 0;

	return blobIt->second.size();
}


const PConfigManager *PConfigManager::Get_Blob(const std::string &name, size_t blobNum) const {
	auto blobIt = _Blobs.find(name);
	if (blobIt == _Blobs.end()) return NULL;
	if (blobNum >= blobIt->second.size()) return NULL;
	return &(blobIt->second[blobNum]);
}


bool PConfigManager::Get_TexureCoord(const std::string &item, float *val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}
	int numRes = sscanf(confit->second.c_str(), "%f,%f", &val[0], &val[1]);
	if (numRes != 2) return false;
	return true;

}

bool PConfigManager::Get_Coord(const std::string &item, int *val) const {
	map <string, string> ::const_iterator confit = ConfigParams.find(item);

	if (confit == ConfigParams.end()) {
		return false;
	}
	int numRes = sscanf(confit->second.c_str(), "%d,%d", &val[0], &val[1]);
	if (numRes != 2) return false;
	return true;

}


void PConfigManager::Clear(void) {
	_Blobs.clear();
	ConfigParams.clear();
}