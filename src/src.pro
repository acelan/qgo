#qmake file

#message($${CONFIG})
RESOURCES = application.qrc  \
	    board/board.qrc
QT += core gui widgets network multimedia
DESTDIR = ../build
TARGET = qgo
OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
#Because I can't figureout how to turn console and exceptions off:
win32 {
    RC_FILE = qgo.rc
}
TEMPLATE = app
TRANSLATIONS = translations/qgo_cz.ts \
	    	translations/qgo_de.ts \
		translations/qgo_dk.ts \
		translations/qgo_fr.ts \
		translations/qgo_it.ts \
		translations/qgo_la.ts \
		translations/qgo_nl.ts \
		translations/qgo_pt.ts \
		translations/qgo_ru.ts \
		translations/qgo_tr.ts \
		translations/qgo_zh_cn.ts \
		translations/qgo_zh.ts
FORMS += mainwindow.ui \
	 board/boardwindow.ui \
	 board/gameinfo.ui \
	 network/createroomdialog.ui \
	 network/friendslistdialog.ui \
	 network/gamedialog.ui \
	 network/login.ui \
	 network/talk_gui.ui \
    connectionwidget.ui \
    sgfpreview.ui

HEADERS += defines.h \
displayboard.h \
gamedata.h \
listviews.h \
mainwindow.h \
newline_pipe.h \
audio/audio.h \
game_tree/group.h \
game_tree/matrix.h \
game_tree/move.h \
game_tree/tree.h \
board/board.h \
board/boardhandler.h \
board/boardwindow.h \
board/clockdisplay.h \
board/gameinfo.h \
board/gatter.h \
board/imagehandler.h \
board/interfacehandler.h \
board/mark.h \
board/stone.h \
game_interfaces/countdialog.h \
game_interfaces/qgoboard.h \
game_interfaces/resultdialog.h \
game_interfaces/undoprompt.h \
gtp/qgtp.h \
network/boarddispatch.h \
network/codecwarndialog.h \
network/consoledispatch.h \
network/createroomdialog.h \
network/cyberoroconnection.h \
network/cyberoroprotocol.h \
network/eweiqiconnection.h \
network/friendslistdialog.h \
network/gamedialog.h \
network/gamedialogflags.h \
network/igsconnection.h \
network/lgs.h \
network/login.h \
network/matchinvitedialog.h \
network/matchnegotiationstate.h \
network/messages.h \
network/networkconnection.h \
network/orosetphrasechat.h \
network/playergamelistings.h \
network/protocol.h \
network/quickconnection.h \
network/room.h \
network/serverlistdialog.h \
network/serverliststorage.h \
network/setphrasepalette.h \
network/talk.h \
network/tomconnection.h \
network/tygemconnection.h \
network/tygemprotocol.h \
network/wing.h \
sgf/sgfparser.h \
    connectionwidget.h \
    host.h \
    sgfpreview.h

SOURCES += displayboard.cpp \
           listviews.cpp \
 	   main.cpp \
           mainwindow.cpp \
           mainwindow_settings.cpp \
           audio/audio.cpp \
           board/board.cpp \
           board/boardhandler.cpp \
           board/boardwindow.cpp \
           board/clockdisplay.cpp \
           board/gameinfo.cpp \
           board/gatter.cpp \
           board/imagehandler.cpp \
           board/interfacehandler.cpp \
           board/mark.cpp \
           board/stone.cpp \
	   game_interfaces/countdialog.cpp \
	   game_interfaces/qgoboard.cpp \
           game_interfaces/qgoboard_computer.cpp \
           game_interfaces/qgoboard_match.cpp \
	   game_interfaces/qgoboard_network.cpp \
           game_interfaces/qgoboard_observe.cpp \
           game_interfaces/qgoboard_review.cpp \
           game_interfaces/resultdialog.cpp \
           game_interfaces/undoprompt.cpp \
           game_tree/matrix.cpp \
           game_tree/move.cpp \
           game_tree/tree.cpp \
           game_tree/group.cpp \
           gtp/qgtp.cpp \
       	   network/boarddispatch.cpp \
	   network/codecwarndialog.cpp \
	   network/consoledispatch.cpp \
	   network/createroomdialog.cpp \
	   network/cyberoroconnection.cpp \
	   network/eweiqiconnection.cpp \
	   network/friendslistdialog.cpp \
           network/gamedialog.cpp \
	   network/igsconnection.cpp \
	   network/lgs.cpp \
 	   network/login.cpp \
	   network/matchinvitedialog.cpp \
	   network/matchnegotiationstate.cpp \
	   network/networkconnection.cpp \
	   network/orosetphrasechat.cpp \
 	   network/quickconnection.cpp \
 	   network/room.cpp \
	   network/serverlistdialog.cpp \
	   network/serverliststorage.cpp \
	   network/setphrasepalette.cpp \
 	   network/talk.cpp \
	   network/tomconnection.cpp \
	   network/tygemconnection.cpp \
	   network/wing.cpp \
	   sgf/sgfparser.cpp \
    connectionwidget.cpp \
    host.cpp \
    sgfpreview.cpp

QT -= qt3support
DEPENDPATH += """"""board sgf game_tree game_interfaces gtp network""""""

linux-* {
    QGO_INSTALL_PATH = /usr/share/qgo
    QGO_INSTALL_BIN_PATH = /usr/bin

    icon.path = /usr/share/pixmaps
    icon.files = resources/pics/qgo.png
    icon.files += resources/pics/qgo_16x16.xpm
    icon.files += resources/pics/qgo_32x32.xpm
    icon.files += resources/pics/qgo_48x48.png
    icon.files += resources/pics/qgo_48x48.xpm
    INSTALLS += icon
    desktopfile.path = /usr/share/applications
    desktopfile.files = qgo.desktop
    INSTALLS += desktopfile
    filetype.path = /usr/share/mimelnk/text
    filetype.files = sgf.desktop
    mimetype.path = /usr/share/mime/text
    mimetype.files = x-sgf.xml
    INSTALLS += filetype mimetype
}
INCLUDEPATH += . network \
audio \
gtp \
game_interfaces \
game_tree \
board \
sgf

languages.path = $${QGO_INSTALL_PATH}/languages
languages.extra = $(QTDIR)/bin/lrelease src.pro
languages.files = translations/*.qm
INSTALLS += languages

boardpics.path = $${QGO_INSTALL_PATH}/boardtextures
boardpics.files = board/resources/pics/barcelona_cherry.png
boardpics.files += board/resources/pics/eurobeech.png
boardpics.files += board/resources/pics/goldenbeech.png
boardpics.files += board/resources/pics/lemontree.png
boardpics.files += board/resources/pics/manitoba.png
boardpics.files += board/resources/pics/maple.png
boardpics.files += board/resources/pics/paper.png
boardpics.files += board/resources/pics/wood.png
boardpics.files += board/resources/pics/wood3.png
boardpics.files += board/resources/pics/wood4.png
boardpics.files += board/resources/pics/wood5.png
INSTALLS += boardpics

sounds.path = $${QGO_INSTALL_PATH}/sounds
sounds.files += resources/sounds/blip.wav
sounds.files += resources/sounds/buzzer.wav
sounds.files += resources/sounds/doorbell.wav
sounds.files += resources/sounds/ns.wav
sounds.files += resources/sounds/pop.wav
sounds.files += resources/sounds/static.wav
sounds.files += resources/sounds/stone.wav
sounds.files += resources/sounds/timer.wav
INSTALLS += sounds

target.path = $${QGO_INSTALL_BIN_PATH}
INSTALLS += target
