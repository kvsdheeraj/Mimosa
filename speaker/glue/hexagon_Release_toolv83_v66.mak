# pakman tree build file
_@ ?= @

.PHONY: QAIC_DIR MAKE_D_EXT_4_DIR MAKE_D_1_LIBDIR MAKE_D_2_LIBDIR MAKE_D_3_LIBDIR MAKE_D_5_INCDIR tree QAIC_DIR_clean MAKE_D_EXT_4_DIR_clean MAKE_D_1_LIBDIR_clean MAKE_D_2_LIBDIR_clean MAKE_D_3_LIBDIR_clean MAKE_D_5_INCDIR_clean tree_clean

tree: MAKE_D_3_LIBDIR MAKE_D_EXT_4_DIR MAKE_D_3_LIBDIR MAKE_D_2_LIBDIR MAKE_D_1_LIBDIR MAKE_D_5_INCDIR MAKE_D_1_LIBDIR MAKE_D_2_LIBDIR MAKE_D_5_INCDIR
	$(call job,,$(MAKE) V=hexagon_Release_toolv83_v66,making .)

tree_clean: MAKE_D_3_LIBDIR_clean MAKE_D_EXT_4_DIR_clean MAKE_D_3_LIBDIR_clean MAKE_D_2_LIBDIR_clean MAKE_D_1_LIBDIR_clean MAKE_D_5_INCDIR_clean MAKE_D_1_LIBDIR_clean MAKE_D_2_LIBDIR_clean MAKE_D_5_INCDIR_clean
	$(call job,,$(MAKE) V=hexagon_Release_toolv83_v66 clean,cleaning .)

MAKE_D_5_INCDIR: 
	$(call job,$(HEXAGON_SDK_ROOT)/test/common/test_main,$(MAKE) V=hexagon_Release_toolv83_v66,making $(HEXAGON_SDK_ROOT)/test/common/test_main)

MAKE_D_5_INCDIR_clean: 
	$(call job,$(HEXAGON_SDK_ROOT)/test/common/test_main,$(MAKE) V=hexagon_Release_toolv83_v66 clean,cleaning $(HEXAGON_SDK_ROOT)/test/common/test_main)

MAKE_D_3_LIBDIR: MAKE_D_EXT_4_DIR MAKE_D_1_LIBDIR MAKE_D_2_LIBDIR
	$(call job,$(HEXAGON_SDK_ROOT)/libs/common/rpcmem,$(MAKE) V=hexagon_Release_toolv83_v66,making $(HEXAGON_SDK_ROOT)/libs/common/rpcmem)

MAKE_D_3_LIBDIR_clean: MAKE_D_EXT_4_DIR_clean MAKE_D_1_LIBDIR_clean MAKE_D_2_LIBDIR_clean
	$(call job,$(HEXAGON_SDK_ROOT)/libs/common/rpcmem,$(MAKE) V=hexagon_Release_toolv83_v66 clean,cleaning $(HEXAGON_SDK_ROOT)/libs/common/rpcmem)

MAKE_D_2_LIBDIR: 
	$(call job,$(HEXAGON_SDK_ROOT)/libs/common/atomic,$(MAKE) V=hexagon_Release_toolv83_v66,making $(HEXAGON_SDK_ROOT)/libs/common/atomic)

MAKE_D_2_LIBDIR_clean: 
	$(call job,$(HEXAGON_SDK_ROOT)/libs/common/atomic,$(MAKE) V=hexagon_Release_toolv83_v66 clean,cleaning $(HEXAGON_SDK_ROOT)/libs/common/atomic)

MAKE_D_1_LIBDIR: 
	$(call job,$(HEXAGON_SDK_ROOT)/test/common/test_util,$(MAKE) V=hexagon_Release_toolv83_v66,making $(HEXAGON_SDK_ROOT)/test/common/test_util)

MAKE_D_1_LIBDIR_clean: 
	$(call job,$(HEXAGON_SDK_ROOT)/test/common/test_util,$(MAKE) V=hexagon_Release_toolv83_v66 clean,cleaning $(HEXAGON_SDK_ROOT)/test/common/test_util)

MAKE_D_EXT_4_DIR: QAIC_DIR

MAKE_D_EXT_4_DIR_clean: QAIC_DIR_clean

QAIC_DIR: 
	$(call job,$(HEXAGON_SDK_ROOT)/tools/qaic,make,making $(HEXAGON_SDK_ROOT)/tools/qaic)

QAIC_DIR_clean: 
	$(call job,$(HEXAGON_SDK_ROOT)/tools/qaic,make clean,cleaning $(HEXAGON_SDK_ROOT)/tools/qaic)

W := $(findstring ECHO,$(shell echo))# W => Windows environment
@LOG = $(if $W,$(TEMP)\\)$@-build.log

C = $(if $1,$(if $W,cd /D,cd) $1 && )$2
job = $(_@)echo $3 && ( $C )> $(@LOG) && $(if $W,del,rm) $(@LOG) || ( echo ERROR $3 && $(if $W,type,cat) $(@LOG) && $(if $W,del,rm) $(@LOG) && exit 1)
ifdef VERBOSE
  job = $(_@)echo $3 && $C
endif
