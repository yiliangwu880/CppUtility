AUTOMAKE_OPTIONS=foreign

bin_PROGRAMS=client

INCLUDES = -I$(top_srcdir)/../../\
			-I$(top_srcdir)/../../linux\
			-I$(top_srcdir)/../../linux/net\
			-I$(top_srcdir)/../../utility

client_SOURCES=main.cpp \
	     $(top_srcdir)/../../linux/net/test_client.cpp\
			$(top_srcdir)/../../utility/logFile.cpp
