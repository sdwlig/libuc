# This assumes that 'flex json_parser.lex' is run manually when necessary.
# Including both the resulting ucoder_json.cpp and the BSD-licensed FlexLexer.h, copied from MACOSX 10.8.3, to allow easy compiling.

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

ADD_LIBRARY( UnivCont STATIC
buffer.cpp
buffer_adapter.cpp
# buffer_curl.cpp
buffer_util.cpp
#example.cpp
#example_adapter.cpp
string_util.cpp
# uc_curl.cpp
uc_web.cpp
ucadaptor.cpp
# uccontract.cpp
ucdb.cpp
ucio.cpp
# ucmysql.cpp
ucoder_bin.cpp
ucoder_ini.cpp
ucoder_json.cpp
ucontainer.cpp
# ucsqlite.cpp
win32fix.cpp
)

IF( WIN32 )
  TARGET_LINK_LIBRARIES( UnivCont  )
ELSE( WIN32 )
  TARGET_LINK_LIBRARIES( UnivCont  ssl crypto ssl crypto z  )
ENDIF( WIN32 )
#TARGET_LINK_LIBRARIES( UnivCont curl ssl crypto ssl crypto z  )

