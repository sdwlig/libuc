/*
 * UniversalContainer library.
 * Copyright Jason Denton, 2008,2010.
 * Made available under the new BSD license, as described in LICENSE
 *
 * Send comments and bug reports to jason.denton@gmail.com
 * http://www.greatpanic.com/code.html
 */

#ifndef _UCONTAINER_HEADER_
#define _UCONTAINER_HEADER_

#include <string>
#include <map>
#include <vector>
#include <stdint.h>
#include "win32fix.h"

/*
 * Used internally by UniversalContainer. Almost always you will
 * want to wrap these in a UniversalContainer rather than using these types.
 */

namespace JAD {

  typedef char UniversalContainerType;

  const UniversalContainerType uc_Null       = 0;
  const UniversalContainerType uc_Integer    = 1;
  const UniversalContainerType uc_Boolean    = 2;
  const UniversalContainerType uc_Character  = 3;
  const UniversalContainerType uc_String     = 4;
  const UniversalContainerType uc_WString    = 5;
  const UniversalContainerType uc_Real       = 6;
  const UniversalContainerType uc_Map        =10;
  const UniversalContainerType uc_Array      =11;
  const UniversalContainerType uc_Reference  =12;
  const UniversalContainerType uc_LastType   =12;
  const UniversalContainerType uc_Unknown    =-1;

  class UniversalContainer;
  typedef std::vector<UniversalContainer> UniversalArray;
  typedef std::map<std::string, UniversalContainer> UniversalMap;

  struct UniversalContainerTypeAdapter {
    virtual std::string to_string(void*) = 0;  
    virtual void* clone(void*) = 0;
    virtual void on_delete(void*) = 0;
    virtual std::string to_json(void*) {return std::string("NULL");}

    virtual ~UniversalContainerTypeAdapter(void) {}
  };

  class UniversalContainer
  {
  public: // Because we have to due to C++11 limitations:
    UniversalContainerType type;
    union {
      double real;
      bool tf;
      char chr;
      long num;
      std::string* str;
      std::wstring* wstr;
      UniversalArray* ray;
      UniversalMap* map;
      void* reference;
    } data;

  protected :
    
    //member variables
    bool dirty;
    unsigned* refcount;
    
    
    //internal setter methods
    //used by constructors and assignment operators
    inline void set_value_integer(long);
    inline void set_value_uinteger(unsigned long);
    inline void set_value_char(char);
    inline void set_value_double(double);
    inline void set_value_bool(bool);
    inline void set_value_string(const std::string&);
    inline void set_value_wstring(const std::wstring&);
    inline void set_value_cstr(const char*);
    
    //internal conversion methods
    //used by casting and equality testing operators
    inline int convert_int(void) const;
    inline unsigned int convert_uint(void) const;
    inline double convert_double(void) const;
    inline char convert_char(void) const;
    inline bool convert_bool(void) const;
    inline long convert_long(void) const;
    inline std::string convert_string(void) const;
    inline std::wstring convert_wstring(void) const;
    
    //internal designates. These routines are generally
    //exposed through multiple interfaces, which are
    //more user friendly
    void duplicate(const UniversalContainer&);
    UniversalContainer& map_brackets(const std::string&);
    
    //utility functions
    static std::string convert_wstring_to_string(const std::wstring* w);
    static std::wstring convert_string_to_wstring(const std::string* s);
    static double convert_string_to_double(const std::string* s);
    static double convert_wstring_to_double(const std::wstring* s);
    static long convert_string_to_long(const std::string* s);
    static unsigned long convert_string_to_ulong(const std::string* s);
    static long convert_wstring_to_long(const std::wstring* s);
    static unsigned long convert_wstring_to_ulong(const std::wstring* s);
   
    bool remove_refcount(void);
    
  private:
      void cleanup(void);
      void is_integer(std::string const&, long&, bool&) const;

  public: 
    //constructors
    UniversalContainer(void);
    UniversalContainer(int);
    UniversalContainer(int16_t);
    UniversalContainer(uint16_t);
    UniversalContainer(long);
    UniversalContainer(double);
    UniversalContainer(bool);
    UniversalContainer(char);
    UniversalContainer(const std::string);
    UniversalContainer(const std::wstring);
    UniversalContainer(char*);
    UniversalContainer(const UniversalContainer&);
    UniversalContainer(UniversalContainerType, void*);
    void string_interpret(const std::string s);
    
    //destructor
    ~UniversalContainer(void);
    
    //cast operators
    operator int(void) const;
    operator int16_t(void) const;
    operator uint16_t(void) const;
    operator uint32_t(void) const;
    operator double(void) const;
    operator float(void) const;
    operator char(void) const;
    operator bool(void) const;
    operator long(void) const;
    operator std::string(void) const;
    operator std::string*(void) const;
    operator std::wstring(void) const;
    operator std::wstring*(void) const;
    const char* c_str(void) const;
    
    //container access
    UniversalContainer& operator[](int);
    // UniversalContainer& operator[](uint32_t);
    UniversalContainer& operator[](const std::string&);
    UniversalContainer& operator[](const std::wstring&);
    UniversalContainer& operator[](char*);
    UniversalContainer& operator[](const char*);

    //assignment operators
    UniversalContainer& operator=(long);
    UniversalContainer& operator=(unsigned long);
    UniversalContainer& operator=(int);
    UniversalContainer& operator=(int16_t);
    UniversalContainer& operator=(uint16_t);
    UniversalContainer& operator=(uint32_t);
    // UniversalContainer& operator=(uint64_t); // With clang, conflicts with UniversalContainer*
    UniversalContainer& operator=(double);
    UniversalContainer& operator=(bool);
    UniversalContainer& operator=(char);
    UniversalContainer& operator=(const std::string&);
    UniversalContainer& operator=(const std::wstring&);
    UniversalContainer& operator=(const char*);
    UniversalContainer& operator=(const UniversalContainer&);
    void* cast_or_throw(UniversalContainerType) const;

    //logical operators
    bool operator==(const UniversalContainer&) const;
    bool operator==(int) const;
    bool operator==(int16_t) const;
    bool operator==(uint16_t) const;
    bool operator==(uint32_t) const;
    // bool operator==(uint64_t) const;
    bool operator==(long) const;
    bool operator==(char) const;
    bool operator==(double) const;
    bool operator==(std::string) const;
    bool operator==(std::wstring) const;
    bool operator==(bool) const;
    
    //support operations
    UniversalContainerType get_type(void) const;
    UniversalContainer clone(void) const;
    UniversalContainer partial_copy(const char**);

    bool remove(const std::string key);
    void clear(void);
    bool exists(std::string const& key) const;
    size_t size(void) const;
    size_t length(void) const;
    bool is_dirty(void) const;
    void clean(void);
    UniversalContainer& added_element(void);
    std::vector<std::string> keys_for_map(void) const;
    std::string to_string(void) const;
    
    //allow the user to container type
    void init_map(void);
    void init_array(void);

    UniversalMap* get_map(void) const;
    UniversalArray* get_vector(void) const;
    
    static UniversalContainerType register_new_type(UniversalContainerTypeAdapter*);

    static UniversalContainer construct_exception(const int, const char* = NULL,
						  const char* = NULL, int = 0, const UniversalContainer* = NULL, const std::string* arg = NULL);
    static UniversalContainer construct_exceptionuc(UniversalContainer ucc,
						  const int, const char* = NULL,
						    const char* = NULL, int = 0, const UniversalContainer* = NULL, const std::string* arg = NULL);
  };
  // Had to move these to be compatible with C++11, then fix everything else up. -- sdw@lig.net
  //Support for maps, vectors, and iterators
  UniversalMap::iterator map_begin(const UniversalContainer& uc);// const; 
  UniversalMap::iterator map_end(const UniversalContainer& uc);// const;
  UniversalArray::iterator vector_begin(const UniversalContainer& uc);// const;
  UniversalArray::iterator vector_end(const UniversalContainer& uc);// const;
  
} //end namespace

#ifndef NDEBUG
#define ucexception(C) UniversalContainer::construct_exception(C,__FUNCTION__,__FILE__,__LINE__,NULL)
#else
#define ucexception(C) UniversalContainer::construct_exception(C,NULL,NULL,0,NULL)
#endif
#ifndef NDEBUG
#define ucexceptionuc(ucc,C) UniversalContainer::construct_exceptionuc(ucc,C,__FUNCTION__,__FILE__,__LINE__,NULL)
#else
#define ucexceptionuc(ucc,C) UniversalContainer::construct_exceptionuc(ucc,C,NULL,NULL,0,NULL)
#endif

#ifndef NDEBUG
#define ucdataexception(C,D) UniversalContainer::construct_exception(C,__FUNCTION__,__FILE__,__LINE__,&D)
#else
#define ucdataexception(C,D) UniversalContainer::construct_exception(C,NULL,NULL,0,&D)
#endif

/* Defines for exception error codes */
#define KNOWN_EXCEPTIONS 16

#define uce_Unknown 0
#define uce_TypeMismatch_Write 1
#define uce_TypeMismatch_Read 2
#define uce_Collection_as_Scalar 3
#define uce_Scalar_as_Collection 4
#define uce_Non_Map_as_Map 5
#define uce_Non_Array_as_Array 6
#define uce_Array_Subscript_Out_of_Bounds 7 
#define uce_Deserialization_Error 8
#define uce_Serialization_Error 9
#define uce_DB_Connection 10
#define uce_Unknown_mime_type 11
#define uce_Communication_Error 12
#define uce_ContractViolation 13
#define uce_IOError 14
#define uce_BadContract 15
#define uce_Unregistered_Type 16
#endif
