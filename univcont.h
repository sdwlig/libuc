/*
 * UniversalContainer library.
 * Copyright Jason Denton, 2008,2010.
 * Made available under the new BSD license, as described in LICENSE
 *
 * Send comments and bug reports to jason.denton@gmail.com
 * http://www.greatpanic.com/code.html
 */

/*
  Convience include. Most clients will include this one file, instead of the
  component pieces.
 */

#ifndef _UNIVCONT_H_
#define _UNIVCONT_H_
#include "libuc_conf.h"

#include "ucontainer.h"
#include "buffer.h"
#include "string_util.h"
#include "ucio.h"
#include "uccontract.h"

#ifdef _UC_WITH_MYSQL
#include "ucmysql.h"
#endif

#ifdef _UC_WITH_SQLITE    
#include "ucsqlite.h"
#endif

#include "uc_web.h"

#endif
