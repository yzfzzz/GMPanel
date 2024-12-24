find_package(Threads REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/database/lib)

set(DATABASE_SRC
    src/ConnectionPool.cpp
    src/MysqlConn.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/database/include ${PROJECT_SOURCE_DIR}/tool) 

add_library(database ${DATABASE_SRC})

target_link_libraries(database
  PUBLIC
  Threads::Threads
  mysqlclient
)