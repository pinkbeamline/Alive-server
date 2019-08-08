TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = alvsvr
# alvsvr.dbd will be created and installed
DBD += alvsvr.dbd

# alvsvr.dbd will be made up from these files:
alvsvr_DBD += base.dbd

# Include dbd files from all support applications:
alvsvr_DBD += aliveserver.dbd

# Add all the support libraries needed by this IOC
#alvsvr_LIBS += xxx

# alvsvr_registerRecordDeviceDriver.cpp derives from alvsvr.dbd
alvsvr_SRCS += alvsvr_registerRecordDeviceDriver.cpp
alvsvr_SRCS += aliveserver.c

# Build the main IOC entry point on workstation OSs.
alvsvr_SRCS_DEFAULT += alvsvrMain.cpp
alvsvr_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#alvsvr_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
alvsvr_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
