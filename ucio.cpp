/*
 * UniversalContainer library.
 * Copyright Jason Denton, 2008,2010.
 * Made available under the new BSD license, as described in LICENSE
 *
 * Send comments and bug reports to jason.denton@gmail.com
 * http://www.greatpanic.com/code.html
 */

#include <stdio.h>
#include <iostream>

#include "ucontainer.h"
#include "ucio.h"
#include "buffer.h"
#include "string_util.h"

/*
  Routines for doing io with universal containers. Mostly, there are
  wrappers on the serialization and deserialization routines. For. web based
  routines, see uc_web.cpp and uc_curl.cpp
 */

namespace JAD {

  void print(UniversalContainer& uc)
  {
    Buffer* buf = uc_encode_ini(uc);
    buf->put('\n');    
    write_from_buffer(buf,stdout);
    delete buf;
  }

  void print_json(UniversalContainer& uc, int indent)
  {
    Buffer* buf = new Buffer;
    uc_encode_pretty_json(uc,buf,0,indent);
    write_from_buffer(buf,stdout);
    delete buf;
  }

  UniversalContainer load_ini_file(const char* filename)
  {
    UniversalContainer uc;
    Buffer* buf = read_to_buffer(filename);
    if (!buf) return uc;
    uc = uc_decode_ini(buf);
    delete buf;
    return uc;
  }

  bool write_ini_file(const char* filename, UniversalContainer& uc)
  {
    Buffer* buf = uc_encode_ini(uc);
    if (!buf) return false;
    bool result = write_from_buffer(buf,filename);
    delete buf;
    return result;
  }

  UniversalContainer uc_from_json_file(const char* fname)
  {
    UniversalContainer uc;
    Buffer* buf = read_to_buffer(fname);
    if (!buf) return uc;
    uc = uc_decode_json(buf);
    delete buf;
    return uc;
  }
}
