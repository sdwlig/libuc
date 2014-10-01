LOCAL_PATH:= $(call my-dir)

#
# libuc
#

include $(CLEAR_VARS)

LOCAL_MODULE:= libuc

include $(PI_ROOT)/app/common/common.mk

LOCAL_SRC_FILES:= \
	buffer_adapter.cpp \
	buffer.cpp \
	buffer_util.cpp \
	string_util.cpp \
	ucadaptor.cpp \
	ucdb.cpp \
	ucio.cpp \
	ucoder_bin.cpp \
	ucoder_ini.cpp \
	ucoder_json.cpp \
	ucontainer.cpp \
	uc_web.cpp \
	win32fix.cpp

	#buffer_curl.cpp
	#uccontract.cpp
	#uc_curl.cpp
	#ucmysql.cpp
	#ucsqlite.cpp

LOCAL_CPP_FEATURES := exceptions

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
