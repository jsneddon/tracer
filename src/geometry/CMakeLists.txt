add_library(geometry geometry.cpp geometry.h sphere.cpp sphere.h pointlight.cpp pointlight.h plane.cpp plane.h)

install (TARGETS geometry DESTINATION bin)
install (FILES sphere.h geometry.h pointlight.h plane.h DESTINATION include)