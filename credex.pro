QT       += core gui widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/database.cpp \
    src/db_reader.cpp \
    src/db_writer.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/password_dialog.cpp \
    src/password_input_dialog.cpp \
    src/record.cpp \
    src/record_dialog.cpp

HEADERS += \
    include/database.h \
    include/db_reader.h \
    include/db_writer.h \
    include/mainwindow.h \
    include/password_dialog.h \
    include/password_input_dialog.h \
    include/record.h \
    include/record_dialog.h

FORMS += \
    ui/mainwindow.ui \
    ui/password_dialog.ui \
    ui/password_input_dialog.ui \
    ui/record_dialog.ui

LIBS += -lssl -lcrypto

# Include path for header files
INCLUDEPATH = include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
