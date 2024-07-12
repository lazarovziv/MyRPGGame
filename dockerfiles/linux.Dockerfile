# docker image for the build environment
FROM ubuntu:24.04

RUN apt update
# last line is for running the game from inside the container
RUN apt install -y --no-install-recommends cmake build-essential wget gcc g++ unzip git clang \
    libfreetype-dev libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libogg-dev libvorbis-dev \
    libvorbisenc2 libvorbisfile3 libopenal-dev libblis-pthread-dev libopengl-dev libglew-dev libxcursor-dev \
    ninja-build ca-certificates \
    libcanberra-gtk-module libcanberra-gtk3-module

RUN apt clean

ARG UNAME=ubuntu
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME

RUN mkdir /workspace && chown $UNAME:$UNAME /workspace

# setting env variables for compilation
ENV CXX=/usr/bin/clang++
ENV CC=/usr/bin/clang

USER $UNAME
WORKDIR /workspace

#CMD cmake -B build/ -G "Ninja" . && cmake --build build/