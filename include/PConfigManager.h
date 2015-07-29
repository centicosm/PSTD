#pragma once

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <map>
#include <string>
#include <list>
#include <vector>


namespace PSTD {

	class PConfigManager { 
	public:
		PConfigManager(void);
		~PConfigManager(void);

		bool Load_ConfigItems(const std::string &configfile);
		bool Load_ConfigItems(const char *buffer, unsigned int size);
		void Clear(void);

		const std::string *Get_ConfigItem(const std::string &item) const;
		bool Get_StringConfigItem(const std::string &item, std::string &val) const;
		bool Get_IntConfigItem(const std::string &item, int &val) const;
		bool Get_UIntConfigItem(const std::string &item, unsigned int &val) const;
		bool Get_FloatConfigItem(const std::string &item, float &val) const;

		bool Get_Vector3dfConfigItem(const std::string &item, float *val) const;

		bool Get_Vector4dfConfigItem(const std::string &item, float *val) const; 

		inline bool Get_ColorConfigItem(const std::string &item, float *val) const {
			return Get_Vector4dfConfigItem(item, val);
		};

		bool Get_TexureCoord(const std::string &item, float *val) const;
		bool Get_Coord(const std::string &item, int *val) const;
		bool Get_BoolConfigItem(const std::string &key, bool &val) const;

		size_t Get_NumBlob(const std::string &name) const;
		const PConfigManager *Get_Blob(const std::string &name, size_t blobNum) const;

	private:
		bool Load_Blob(const std::string &blobName, std::list<std::string> &buf);

		std::map<std::string, std::string> ConfigParams;
		std::map<std::string, std::vector<PConfigManager>> _Blobs;
	};
};

#endif
