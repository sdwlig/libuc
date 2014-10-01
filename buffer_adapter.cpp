#include "ucontainer.h"
#include <cstring>
#include "buffer_adapter.h"

namespace JAD {

	UniversalContainerType BufferAdapter::tid = UniversalContainer::register_new_type(new BufferAdapter);

	void* BufferAdapter::clone(void* ptr) 
	{
		Buffer* data = (Buffer*) ptr;
		Buffer* clone;
		if (!data->own_data) {
			clone = new Buffer(data->data, data->length);
		}
		else {
			clone = new Buffer(data->size);
			memcpy(clone->data, data->data, data->length);
			clone->length = data->length;
		}
		clone->rpos = data->rpos;
		clone->wpos = data->wpos;
		return clone;
	}

	void BufferAdapter::on_delete(void* ptr)
	{
		Buffer* buf = (Buffer*) ptr;
		delete buf;
	}

	std::string BufferAdapter::to_string(void* ptr)  
	{
		Buffer* b64 = base64_encode((Buffer*) ptr);
		string str(b64->data);
		delete b64;
		return str;
	}

	std::string BufferAdapter::to_json(void* ptr)  
	{
		Buffer* b64 = base64_encode((Buffer*) ptr);
		string str("\"");
		str.append(b64->data);
		str.push_back('\"');
		delete b64;
		return str;
	}

	//For most uses there should be no need to change the code below this point.

	UniversalContainer BufferAdapter::assign(Buffer* ptr)
	{
		return UniversalContainer(tid, ptr);
	}

	Buffer* BufferAdapter::cast(UniversalContainer const& uc)
	{
		Buffer* at = (Buffer*) uc.cast_or_throw(tid);
		return at;
	}
}


