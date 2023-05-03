QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QCXXHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QCodeEditor.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QFramedTextAttribute.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QGLSLCompleter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QGLSLHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QJSONHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QLanguage.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QLineNumberArea.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QLuaCompleter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QLuaHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QPythonCompleter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QPythonHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QStyleSyntaxHighlighter.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QSyntaxStyle.cpp \
    ../../dependency/Megaxela/QCodeEditor/src/internal/QXMLHighlighter.cpp \
    ../../dependency/bterrier/qt-json-viewer/jsonmodel.cpp \
    main.cpp \
    mainwindow.cpp


INCLUDEPATH += ../../dependency/Megaxela/QCodeEditor/include \
    ../../dependency/bterrier/qt-json-viewer/

HEADERS += \
    ../../dependency/Megaxela/QCodeEditor/include/QCXXHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QCodeEditor \
    ../../dependency/Megaxela/QCodeEditor/include/QFramedTextAttribute \
    ../../dependency/Megaxela/QCodeEditor/include/QGLSLCompleter \
    ../../dependency/Megaxela/QCodeEditor/include/QGLSLHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QHighlightBlockRule \
    ../../dependency/Megaxela/QCodeEditor/include/QHighlightRule \
    ../../dependency/Megaxela/QCodeEditor/include/QJSONHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QLanguage \
    ../../dependency/Megaxela/QCodeEditor/include/QLineNumberArea \
    ../../dependency/Megaxela/QCodeEditor/include/QLuaCompleter \
    ../../dependency/Megaxela/QCodeEditor/include/QLuaHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QPythonCompleter \
    ../../dependency/Megaxela/QCodeEditor/include/QPythonHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QStyleSyntaxHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/QSyntaxStyle \
    ../../dependency/Megaxela/QCodeEditor/include/QXMLHighlighter \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QCXXHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QCodeEditor.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QFramedTextAttribute.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QGLSLCompleter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QGLSLHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QHighlightBlockRule.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QHighlightRule.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QJSONHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QLanguage.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QLineNumberArea.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QLuaCompleter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QLuaHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QPythonCompleter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QPythonHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QStyleSyntaxHighlighter.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QSyntaxStyle.hpp \
    ../../dependency/Megaxela/QCodeEditor/include/internal/QXMLHighlighter.hpp \
    ../../dependency/bterrier/qt-json-viewer/jsonmodel.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

include(../module/module.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../dependency/Megaxela/QCodeEditor/resources/qcodeeditor_resources.qrc \
    resources/resources.qrc

DISTFILES += \
    ../../dependency/Megaxela/QCodeEditor/resources/default_style.xml \
    ../../dependency/Megaxela/QCodeEditor/resources/languages/cpp.xml \
    ../../dependency/Megaxela/QCodeEditor/resources/languages/glsl.xml \
    ../../dependency/Megaxela/QCodeEditor/resources/languages/lua.xml \
    ../../dependency/Megaxela/QCodeEditor/resources/languages/python.xml
