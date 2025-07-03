cmake_minimum_required(VERSION 3.16)

project(Jukebox_IUT VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

# 1. ماژول Network به اینجا اضافه شد
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools Multimedia Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools Multimedia Network)

set(TS_FILES Jukebox_IUT_en_US.ts)

# تمام فایل‌های پروژه به صورت متمرکز در این لیست قرار گرفتند
set(PROJECT_SOURCES
        main.cpp
        ${TS_FILES}

        # --- Backend Files ---
        src/backend/player.cpp
        src/backend/player.h
        src/backend/libraryManager.cpp
        src/backend/libraryManager.h
        src/backend/song.cpp
        src/backend/song.h
        src/backend/queueManager.cpp
        src/backend/queueManager.h
        src/backend/playlistManager.cpp
        src/backend/playlistManager.h
        src/backend/playlist.h
        src/backend/networkmanager.cpp
        src/backend/networkmanager.h

        # --- Frontend Files ---
        src/frontend/mainwindow.cpp
        src/frontend/mainwindow.h
        src/frontend/mainwindow.ui
        src/frontend/controls.cpp
        src/frontend/controls.h
        src/frontend/controls.ui
        src/frontend/library.cpp
        src/frontend/library.h
        src/frontend/library.ui
        src/frontend/playlists.cpp
        src/frontend/playlists.h
        src/frontend/playlists.ui
        src/frontend/queue.cpp
        src/frontend/queue.h
        src/frontend/queue.ui
        src/frontend/librarysong.cpp
        src/frontend/librarysong.h
        src/frontend/librarysong.ui
        src/frontend/queuesong.cpp
        src/frontend/queuesong.h
        src/frontend/queuesong.ui
        src/frontend/createplaylistdialog.cpp
        src/frontend/createplaylistdialog.h
        src/frontend/createplaylistdialog.ui
        src/frontend/playlistinlist.cpp
        src/frontend/playlistinlist.h
        src/frontend/playlistinlist.ui
        src/frontend/addtoplaylistdialog.cpp
        src/frontend/addtoplaylistdialog.h
        src/frontend/addtoplaylistdialog.ui
        src/frontend/addtoplaylist.cpp
        src/frontend/addtoplaylist.h
        src/frontend/addtoplaylist.ui
        src/frontend/editor.cpp
        src/frontend/editor.h
        src/frontend/editor.ui
        src/frontend/editorsong.cpp
        src/frontend/editorsong.h
        src/frontend/editorsong.ui
        src/frontend/switcher.cpp
        src/frontend/switcher.h
        src/frontend/switcher.ui
        src/frontend/logindialog.h
        src/frontend/logindialog.cpp
        src/frontend/logindialog.ui
        src/frontend/registerdialog.h
        src/frontend/registerdialog.cpp
        src/frontend/registerdialog.ui
        src/frontend/welcomewindow.h
        src/frontend/welcomewindow.cpp
        src/frontend/welcomewindow.ui
        src/frontend/partydialog.h
        src/frontend/partydialog.cpp
        src/frontend/partydialog.ui
        src/frontend/joinpartydialog.h
        src/frontend/joinpartydialog.cpp
        src/frontend/joinpartydialog.ui
        src/frontend/createserverdialog.h
        src/frontend/createserverdialog.cpp
        src/frontend/createserverdialog.ui
        # مسیر و نام UserManager اصلاح شد
        src/frontend/UserManager.h
        src/frontend/UserManager.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Jukebox_IUT
        MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources/resources.qrc
            src/frontend/fakevisualizerwidget.h src/frontend/fakevisualizerwidget.cpp
    )
else()
    # بخش Qt5 برای سازگاری
    if(ANDROID)
        add_library(Jukebox_IUT SHARED ${PROJECT_SOURCES})
    else()
        add_executable(Jukebox_IUT ${PROJECT_SOURCES})
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 2. ماژول Network به اینجا هم اضافه شد
target_link_libraries(Jukebox_IUT
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia
        PRIVATE Qt${QT_VERSION_MAJOR}::Network
)

# ... (بقیه فایل بدون تغییر باقی می‌ماند) ...

if(${QT_VERSION_MAJOR} EQUAL 6)
    qt_finalize_executable(Jukebox_IUT)
endif()
