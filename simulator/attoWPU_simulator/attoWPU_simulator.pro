#-------------------------------------------------
#
# Project created by QtCreator 2011-02-05T20:11:16
#
#-------------------------------------------------

QT       += core gui \
            multimedia

TARGET = attoWPU_simulator
TEMPLATE = app


SOURCES += main.cpp\
        simulatorwindow.cpp \
    attoWPUsrc/Exception.cpp \
    attoWPUsrc/dummy.cpp \
    attoWPUsrc/SourceRead.cpp \
    attoWPUsrc/SourceString.cpp \
    attoWPUsrc/SourceChar.cpp \
    attoWPUsrc/SourceFile.cpp \
    attoWPUsrc/AttocodeWriteMem.cpp \
    attoWPUsrc/AttoASMHeader.cpp \
    attoWPUsrc/AttocodeWrite.cpp \
    attoWPUsrc/02_TEMP.cpp \
    attoWPUsrc/00_aPC.cpp \
    attoWPUsrc/S0_aPCwrite.cpp \
    attoWPUsrc/Unit.cpp \
    attoWPUsrc/05_OUT.cpp \
    attoWPUsrc/0A_LED.cpp \
    attoWPUsrc/09_SmallQueue.cpp \
    attoWPUsrc/03_RegisterMem.cpp \
    attoWPUsrc/0B_Text.cpp \
    attoWPUsrc/XX_MemoryController.cpp \
    attoWPUsrc/0C_LCD.cpp \
    attoWPUsrc/UniMemoryRAM.cpp \
    attoWPUsrc/UniMemory.cpp \
    attoWPUsrc/06_FPU.cpp \
    attoWPUsrc/04_ALU.cpp \
    attoWPUsrc/AttoWPU.cpp \
    attoWPUsrc/01_AttocodeMemory.cpp \
    attoWPUsrc/Dumpable.cpp \
    attoWPUsrc/Source.cpp \
    attoWPUsrc/Values.cpp \
    attoWPUsrc/attoasm.cpp \
    attoWPUsrc/0D_InputController.cpp \
    HEXeditor.cpp \
    lcdlargeview.cpp \
    attoassembling.cpp \
    attoWPUsrc/MessageStrings.cpp \
    attoWPUsrc/Message.cpp \
    attoWPUsrc/AudioBuffer.cpp \
    attoWPUsrc/0F_Speaker.cpp \
    NameStrings.cpp \
    SourceQFile.cpp \
    aboutdialog.cpp \
    QLedIndicator-0.1.1/qledindicator.cpp \
    Switch.cpp \
    AttoWPUthread.cpp

HEADERS  += simulatorwindow.h \
    attoWPUsrc/Exception.h \
    attoWPUsrc/AttoASMHeader.h \
    attoWPUsrc/SourceString.h \
    attoWPUsrc/SourceChar.h \
    attoWPUsrc/SourceFile.h \
    attoWPUsrc/AttocodeWriteMem.h \
    attoWPUsrc/AttocodeWrite.h \
    attoWPUsrc/S0_aPCwrite.h \
    attoWPUsrc/06_FPU.h \
    attoWPUsrc/04_ALU.h \
    attoWPUsrc/Bits.h \
    attoWPUsrc/DataWrite.h \
    attoWPUsrc/UniMemoryRAM.h \
    attoWPUsrc/DataRead.h \
    attoWPUsrc/05_OUT.h \
    attoWPUsrc/Units.h \
    attoWPUsrc/03_RegisterMem.h \
    attoWPUsrc/02_TEMP.h \
    attoWPUsrc/01_AttocodeMemory.h \
    attoWPUsrc/0A_LED.h \
    attoWPUsrc/00_aPC.h \
    attoWPUsrc/09_SmallQueue.h \
    attoWPUsrc/XX_MemoryController.h \
    attoWPUsrc/0C_LCD.h \
    attoWPUsrc/0B_Text.h \
    attoWPUsrc/UniMemoryDummy.h \
    attoWPUsrc/Unit.h \
    attoWPUsrc/AttoWPU.h \
    attoWPUsrc/UniMemory.h \
    attoWPUsrc/Dumpable.h \
    attoWPUsrc/Source.h \
    attoWPUsrc/Values.h \
    attoWPUsrc/attoasm.h \
    attoWPUsrc/SourceRead.h \
    attoWPUsrc/csettings.h \
    attoWPUsrc/0D_InputController.h \
    HEXeditor.h \
    lcdlargeview.h \
    attoassembling.h \
    attoWPUsrc/MessageCout.h \
    attoWPUsrc/AssemblyException.h \
    attoWPUsrc/InternalException.h \
    attoWPUsrc/Message.h \
    attoWPUsrc/AudioBuffer.h \
    attoWPUsrc/MessageStrings.h \
    attoWPUsrc/0F_Speaker.h \
    attoWPUsrc/MessageDummy.h \
    NameStrings.h \
    SourceQFile.h \
    aboutdialog.h \
    QLedIndicator-0.1.1/qledindicator.h \
    Switch.h \
    AttoWPUthread.h

FORMS    += simulatorwindow.ui \
    lcdlargeview.ui \
    attoassembling.ui \
    aboutdialog.ui
