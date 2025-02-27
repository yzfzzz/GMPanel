set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5
    REQUIRED COMPONENTS
    Core
    Widgets
    Charts
    )

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_add_resources(display_resources pic.qrc qss.qrc)

set(SOURCES
    main.cpp
    login.cpp
    start.cpp
    widget.cpp
    net_chart.cpp
    gpu_chart.cpp
    water_progress.cpp
    ../tool/query_data.cpp
    work_thread.cpp
    widget.ui
    login.ui
    start.ui
    ${display_resources}
)

link_directories(${PROJECT_SOURCE_DIR}/build/rpc_manager/client 
                 ${PROJECT_SOURCE_DIR}/build/proto
                 ${PROJECT_SOURCE_DIR}/database/lib
                 ${PROJECT_SOURCE_DIR}/mprpc/lib)

add_executable(display ${SOURCES})

set_target_properties(display PROPERTIES AUTOMOC TRUE)

target_include_directories(display PUBLIC
    ${PROJECT_SOURCE_DIR}/tool
    ${PROJECT_SOURCE_DIR}/database/include
    ${PROJECT_SOURCE_DIR}/build/proto
    ${PROJECT_SOURCE_DIR}/rpc_manager
    ${PROJECT_SOURCE_DIR}/mprpc/src/include
)

target_link_libraries(display
    PUBLIC
    client
    Qt5::Widgets
    Qt5::Core
    Qt5::Charts
    Threads::Threads
    database
    glog
    monitor_proto
    mprpc
    # qwt-qt5
)