# docker image for the build environment
FROM ubuntu:24.04

RUN apt update
# last line is for running the game from inside the container
RUN apt install -y --no-install-recommends cmake build-essential wget gcc g++ unzip git clang \
    libfreetype-dev libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libogg-dev libvorbis-dev \
    libvorbisenc2 libvorbisfile3 libopenal-dev libblis-pthread-dev libopengl-dev libglew-dev libxcursor-dev \
    ninja-build ca-certificates \
    libcanberra-gtk-module libcanberra-gtk3-module

RUN apt install wget --no-install-recommends -y
RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc -O /etc/apt/trusted.gpg.d/lunarg-signing-key-pub.asc
RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.290-noble.list https://packages.lunarg.com/vulkan/1.3.290/lunarg-vulkan-1.3.290-noble.list
RUN apt update && apt install vulkan-sdk -y
RUN apt install libwayland-dev xorg-dev -y

RUN git clone https://github.com/KhronosGroup/Vulkan-Tools.git /vulkan-tools
WORKDIR /vulkan-tools
RUN apt install python3 -y 
RUN cmake -S . -B build -D UPDATE_DEPS=ON -D BUILD_WERROR=ON -D BUILD_TESTS=ON -D CMAKE_BUILD_TYPE=Debug
RUN cmake --build build --config Debug

RUN apt clean

ARG UNAME=ubuntu
ARG UID=1000
ARG GID=1000

RUN usermod -aG $GID $UNAME

RUN mkdir /workspace && chown $UNAME:$UNAME /workspace

# setting env variables for compilation
ENV CXX=/usr/bin/clang++
ENV CC=/usr/bin/clang

USER $UNAME
WORKDIR /workspace

#CMD cmake -B build/ -G "Ninja" . && cmake --build build/