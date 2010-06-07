QT += network \
    xml
TARGET = SmartFmListEditor
TEMPLATE = app
SOURCES += startpagewidget.cpp \
    SmartFmClient/smartfmclient.cpp \
    SmartFmClient/smartfmitemsmodel.cpp \
    SmartFmClient/smartfmlistsmodel.cpp \
    main.cpp \
    listpagewidget.cpp
HEADERS += startpagewidget.h \
    SmartFmClient/smartfmclient.h \
    SmartFmClient/smartfmlistsmodel.h \
    SmartFmClient/smartfmitemsmodel.h \
    listpagewidget.h
