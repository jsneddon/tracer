add_library(geometry geometry.cpp geometry.h sphere.cpp sphere.h pointlight.cpp pointlight.h)

install (TARGETS geometry DESTINATION bin)
install (FILES sphere.h geometry.h pointlight.h DESTINATION include)