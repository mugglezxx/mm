# target
file(GLOB_RECURSE Order_Test_SOURCES "*.cpp")
file(GLOB_RECURSE Order_Test_HEADERS "*.hpp")

add_executable(OrderTest 
	${Order_Test_SOURCES}
)

target_include_directories(OrderTest PUBLIC
	${Common_INCLUDES}
	${Bus_INCLUDES}
	${Messaging_INCLUDES}
	${Threading_INCLUDES}
	${Service_INCLUDES}
	${Order_INCLUDES}
)

target_link_libraries(OrderTest 
	${Boost_LIBRARIES}
	${TBB_LIBS}
	${POCO_LIBS}
	${GTEST_LIBS}
	Common
	Messaging
	Threading
	Service
	Order
)

add_test(OrderTest, OrderTest)

add_custom_command(
     TARGET OrderTest
     COMMENT "Run tests"
     POST_BUILD 
#     COMMAND ${CMAKE_COMMAND} -E copy OrderTest UnitTest
     COMMAND OrderTest
)