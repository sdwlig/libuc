
#ifdef __UCTA_HEADER__
#define __UCTA_HEADER__

#include <map>
#include <string>

namespace JAD {
	struct UniversalContainerTypeAdaptor {
		static std::string to_string(void*) = 0;	
		static UniversalContainer clone(void*) = 0;
		static on_delete(void*) = 0;
	};
	
}// end namespace

 #endif