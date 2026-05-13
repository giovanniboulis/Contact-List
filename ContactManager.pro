#
#  ContactManager.pro  —  Qt project build file
#  Run:  qmake ContactManager.pro && make


QT       += core gui widgets
CONFIG   += c++17
TARGET    = ContactManager
TEMPLATE  = app

SOURCES  += main.cpp MainWindow.cpp
HEADERS  += MainWindow.h Contact.h HashTable.h UndoStack.h BinarySearch.h ContactManager.h
