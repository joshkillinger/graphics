HEADERS = sci-shield-viewer.h sci-shield-opengl.h Type.h Object.h WaveOBJ.h transform.h cube.h triangle.h \
    material.h \
    shield.h \
    gametime.h \
    hitbox.h \
    spherehitbox.h \
    behavior.h \
    squadron.h
SOURCES = sci-shield.cpp sci-shield-viewer.cpp sci-shield-opengl.cpp Object.cpp WaveOBJ.cpp transform.cpp cube.cpp triangle.cpp \
    material.cpp \
    shield.cpp \
    gametime.cpp \
    hitbox.cpp \
    spherehitbox.cpp \
    behavior.cpp \
    squadron.cpp
QT += opengl
RESOURCES = sci-shield.qrc
