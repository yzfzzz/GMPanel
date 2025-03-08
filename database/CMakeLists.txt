find_package(Threads REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/database/lib)

set(DATABASE_SRC
    src/connection_pool.cpp
    src/mysql_conn.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/database/include ${PROJECT_SOURCE_DIR}/utils) 

add_library(database ${DATABASE_SRC})

target_link_libraries(database
  PUBLIC
  Threads::Threads
  mysqlclient
)