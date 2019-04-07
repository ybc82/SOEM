# sources specifically for this project
set(SOURCES_PROJECT
	simple_test_torque.c
	control.c
	Control_grt_rtw/Control.c
	Control_grt_rtw/Control_data.c
	Control_grt_rtw/rt_nonfinite.c
	Control_grt_rtw/rtGetNaN.c
	Control_grt_rtw/rtGetInf.c
)

# shared sources(libraries) of Matlab
set(SOURCES_MATLAB
	matlab_lib/rt_logging.c
)

# matlab exported shared library
# copy all *.h from library.zip//R2016/ to matlab_lib
# copy library.zip//R2016/rtw/c/src to matlab_lib
include_directories(matlab_lib)
include_directories(Control_grt_rtw)

add_executable(simple_test_torque ${SOURCES_PROJECT} ${SOURCES_MATLAB})

target_link_libraries(simple_test_torque soem)
target_link_libraries(simple_test_torque m)
install(TARGETS simple_test_torque DESTINATION bin)

