/*
	This file is an example type adapter. To use, search the strings below
	with the appropriate symbols. Then, split the file into header and cpp
	files at the given point. Then implement the required methods.

	In theory you could make the adapter class a singleton. In practice, 
	better not to mess with this complexity. Instead, just don't ever 
	instantiate an instance of the adapter. Let the linker hack described
	below create the single required instance. Just use the static methods.

	Note that the static members of the class are not part of the required
	interface. But you need these methods or something like them to make
	the adapter useful. Making them static methods of the adapter class
	keeps everything packaged together and allows for hiding the type id
	returned by the registration function where the user doesn't need to
	know about it.
*/

// Find and replace these strings
//  myadapter                is the name of the adapter class
//  filename                 is the name of the files, without the .h or .cpp
//  adapter_type				  this type this class wraps for uc

#ifndef __filename_h__
#define __filename_h__

#include <ucontainer.h>

using namespace JAD;

struct myadapter : public UniversalContainerTypeAdapter
{
	 virtual std::string to_string(void*);  
    virtual void* clone(void*);
    virtual void on_delete(void*);
    virtual std::string to_json(void*);

    static UniversalContainerType tid;
    static UniversalContainer assign(adapter_type*);
    static adapter_type* cast(UniversalContainer const& uc);
};

#endif

//------------ Split into header and cpp files here. --------------

#include "filename.h"

using namespace JAD;

//This linker hack relies on the fact that c++ will execute code to initialize global variables before
//main is invoked. You can exploit this make sure that your class is registered anytime it gets linked in.

UniversalContainerType myadapter::tid = UniversalContainer::register_new_type(new myadapter);

void* myadapter::clone(void* ptr) 
{
	adapter_type* data = (adapter_type*) ptr;
	//implement clone
}

void myadapter::on_delete(void* ptr)
{
	adapter_type* data = (adapter_type*) ptr;
	//implement code for delete.
	//this will only be called when the last refering uc is deleted
}

std::string myadapter::to_string(void* ptr)  
{
	adapter_type* data = (adapter_type*) ptr;
	//implement to_string
}

std::string myadapter::to_json(void* ptr)  
{
	adapter_type* data = (adapter_type*) ptr;
	//implment code for json translation, or just return an empty string.
	//A default implementation returning the string "NULL" is provided.
	//return std:string("\"NULL\"");
}

//For most uses there should be no need to change the code below this point.

UniversalContainer myadapter::assign(adapter_type* ptr)
{
	return UniversalContainer(tid, ptr);
}

adapter_type* myadapter::cast(UniversalContainer const& uc)
{
	adapter_type* at = (adapter_type*) uc.cast_or_throw(tid);
	return adapter_type;
}


