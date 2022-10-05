cmake_minimum_required(VERSION 3.16)
project(WOR-World_Kinematica)
set(EXE robotworld)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(Boost_USE_MULTITHREADED TRUE)

find_package(Threads REQUIRED)
find_package(wxWidgets REQUIRED)
find_package(Boost REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(include/application)
include_directories(include/base)
include_directories(include/config)
include_directories(include/messaging)
include_directories(include/model)
include_directories(include/pathAlgorithm)
include_directories(include/utils)
include_directories(include/view)
include_directories(include/widgets)

include_directories(src/)

set(HEADERS_APPLICATION
        include/application/Button.hpp
        include/application/CommandlineArgument.hpp
        include/application/CommunicationService.hpp
        include/application/Logger.hpp
        include/application/LogTextCtrl.hpp
        include/application/MainApplication.hpp
        include/application/MainFrameWindow.hpp
        include/application/WidgetTraceFunction.hpp
        )
set(HEADERS_BASE
        include/base/FileTraceFunction.hpp
        include/base/ITraceFunction.hpp
        include/base/NotificationEvent.hpp
        include/base/NotificationFunctionTypeTraits.hpp
        include/base/NotificationHandler.hpp
        include/base/Notifier.hpp
        include/base/ObjectId.hpp
        include/base/Observer.hpp
        include/base/Queue.hpp
        include/base/StdOutTraceFunction.hpp
        include/base/Trace.hpp
        )
set(HEADERS_CONFIG
        include/config/config.h.in
        include/config/Config.hpp
        )
set(HEADERS_MESSAGING
        include/messaging/Client.hpp
        include/messaging/Message.hpp
        include/messaging/MessageHandler.hpp
        include/messaging/MessageTypes.hpp
        include/messaging/Server.hpp
        include/messaging/Session.hpp
        )
set(HEADERS_MODEL
        include/model/AbstractActuator.hpp
        include/model/AbstractAgent.hpp
        include/model/AbstractSensor.hpp
        include/model/BoundedVector.hpp
        include/model/Goal.hpp
        include/model/LaserDistanceSensor.hpp
        include/model/ModelObject.hpp
        include/model/Robot.hpp
        include/model/RobotWorld.hpp
        include/model/SteeringActuator.hpp
        include/model/Wall.hpp
        include/model/WayPoint.hpp

        include/model/NoisySensor.h
        include/model/CompassSensor.h
        include/model/LidarSensor.h
        include/model/OdometerSensor.h

        )
set(HEADERS_PATH_ALGORITHM
        include/pathAlgorithm/AStar.hpp)
set(HEADERS_UTILS
        include/utils/MathUtils.hpp
        include/utils/Shape2DUtils.hpp
        include/utils/TimeStamp.hpp
        include/utils/TypeInfo.hpp
        )
set(HEADERS_VIEW
        include/view/GoalShape.hpp
        include/view/LineShape.hpp
        include/view/RectangleShape.hpp
        include/view/RobotShape.hpp
        include/view/RobotWorldCanvas.hpp
        include/view/Shape.hpp
        include/view/ViewObject.hpp
        include/view/WallShape.hpp
        include/view/WayPointShape.hpp
        )
set(HEADERS_WIDGETS
        include/widgets/Point.hpp
        include/widgets/Region.hpp
        include/widgets/Size.hpp
        include/widgets/Widgets.hpp
        )

set(HEADERS
        ${HEADERS_APPLICATION}
        ${HEADERS_BASE}
        ${HEADERS_CONFIG}
        ${HEADERS_MESSAGING}
        ${HEADERS_MODEL}
        ${HEADERS_PATH_ALGORITHM}
        ${HEADERS_UTILS}
        ${HEADERS_VIEW}
        ${HEADERS_WIDGETS})

set(SOURCES
        src/AbstractActuator.cpp
        src/AbstractAgent.cpp
        src/AbstractSensor.cpp
        src/AStar.cpp
        src/BoundedVector.cpp
        src/CommunicationService.cpp
        src/FileTraceFunction.cpp
        src/Goal.cpp
        src/GoalShape.cpp
        src/LaserDistanceSensor.cpp
        src/LineShape.cpp
        src/Logger.cpp
        src/LogTextCtrl.cpp
        src/Main.cpp
        src/MainApplication.cpp
        src/MainFrameWindow.cpp
        src/MathUtils.cpp
        src/ModelObject.cpp
        src/NotificationHandler.cpp
        src/Notifier.cpp
        src/ObjectId.cpp
        src/Observer.cpp
        src/RectangleShape.cpp
        src/Robot.cpp
        src/RobotShape.cpp
        src/RobotWorld.cpp
        src/RobotWorldCanvas.cpp
        src/Server.cpp
        src/Shape2DUtils.cpp
        src/StdOutTraceFunction.cpp
        src/SteeringActuator.cpp
        src/Trace.cpp
        src/ViewObject.cpp
        src/Wall.cpp
        src/WallShape.cpp
        src/WayPoint.cpp
        src/WayPointShape.cpp
        src/Widgets.cpp
        src/WidgetTraceFunction.cpp

        src/NoisySensor.cpp
        src/CompassSensor.cpp
        src/LidarSensor.cpp
        src/OdometerSensor.cpp
        )

add_executable(${EXE} ${SOURCES} ${HEADERS})

target_link_libraries(${EXE} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${EXE} ${wxWidgets_LIBRARIES})
target_link_libraries(${EXE} ${Boost_LIBRARIES})

install(TARGETS ${EXE} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR})