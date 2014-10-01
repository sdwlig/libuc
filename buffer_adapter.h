/*
	An example of how to use the buffer_adapter class. See example_adapter.cpp
	for an explanation.
*/

#ifndef __buffer_adapter_h__
#define __buffer_adapter_h__

#include "ucontainer.h"
#include "buffer.h"

namespace JAD {
	struct BufferAdapter : public UniversalContainerTypeAdapter
	{
		 virtual std::string to_string(void*);  
	    virtual void* clone(void*);
	    virtual void on_delete(void*);
	    virtual std::string to_json(void*);

	    static UniversalContainerType tid;
	    static UniversalContainer assign(Buffer *);
	    static Buffer* cast(UniversalContainer const& uc);
	};
}
#endif