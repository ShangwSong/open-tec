# $Id: Makefile.am 3140 2012-06-18 15:03:02Z susancummins $
INCLUDES = -I$(srcdir)/../../src
LDADD = ./libtoolslib.la ../../src/libgpstk.la

lib_LTLIBRARIES = libtoolslib.la
libtoolslib_la_LDFLAGS = -version-number @GPSTK_SO_VERSION@
libtoolslib_la_SOURCES = RinexEditor.cpp

bin_PROGRAMS = NavMerge RinexDump Rinex3Dump ResCor EditRinex RinSum Rin3Sum

NavMerge_SOURCES = NavMerge.cpp
RinexDump_SOURCES = RinexDump.cpp
Rinex3Dump_SOURCES = Rinex3Dump.cpp
RinSum_SOURCES = RinSum.cpp
Rin3Sum_SOURCES = Rin3Sum.cpp
EditRinex_SOURCES = EditRinex.cpp
ResCor_SOURCES = ResCor.cpp
