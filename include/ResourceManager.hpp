#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <vector>
#include <map>

namespace PSTD {

	template <typename T>
	class ResourceManager {

		public:
		ResourceManager(void) {};
		virtual ~ResourceManager(void) {};

		int Get_ResourceId(const std::string &name) {
			auto idIt = _ResourceNameMap.find(name);
			if (idIt == _ResourceNameMap.end()) return -1;

			return idIt->second;
		}

		bool Get_Resource(const std::string &name, T &val) {
			auto idIt = _ResourceNameMap.find(name);
			if (idIt == _ResourceNameMap.end()) return false;

			val = _Resource[idIt->second];
			return true;
		}

		bool Get_Resource(size_t id, T &val) {
			if (id > _Resource.size()) return false;
			val = _Resource[id];
			return true;
		}


		int Add_Resource(const std::string &name, T val) {
			auto idIt = _ResourceNameMap.find(name);
			if (idIt != _ResourceNameMap.end()) return -1;
			_Resource.push_back(val);
			int index = (int)(_Resource.size() - 1);
			_ResourceNameMap[name] = index;
			return index;
		}

		protected:

		std::vector<T> _Resource;
		std::map<const std::string, int> _ResourceNameMap;
	}

};

#endif