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


set(SOURCES
    main.cpp
    widget.cpp
    net_chart.cpp
    smooth_chart.cpp
    water_progress.cpp
    query_data.cpp
    work_thread.cpp
    widget.ui
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