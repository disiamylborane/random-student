
default:
	echo 'Run `make en` to build an english version'

pl: panel_student.cpp panel_whiteboard.cpp random_student.cpp
	g++ panel_student.cpp panel_whiteboard.cpp random_student.cpp `wx-config --cxxflags --libs` -no-pie -O2 -DLANG_PL -o random_student

en: panel_student.cpp panel_whiteboard.cpp random_student.cpp
	g++ panel_student.cpp panel_whiteboard.cpp random_student.cpp `wx-config --cxxflags --libs` -no-pie -O2 -DLANG_EN -o random_student

ru: panel_student.cpp panel_whiteboard.cpp random_student.cpp
	g++ panel_student.cpp panel_whiteboard.cpp random_student.cpp `wx-config --cxxflags --libs` -no-pie -O2 -DLANG_RU -o random_student

clean:

