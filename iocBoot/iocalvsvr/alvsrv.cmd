#!../../bin/linux-x86_64/alvsvr

## You may have to change alvsvr to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/alvsvr.dbd"
alvsvr_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/alvsrv.db","BL=PINK,DEV=ALVSRV")

cd "${TOP}/iocBoot/${IOC}"

dbLoadTemplate("alive.substitutions")

iocInit

## Start any sequence programs
#seq sncxxx,"user=epics"
