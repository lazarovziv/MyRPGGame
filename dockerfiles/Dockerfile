# docker image for the build environment
FROM ubuntu:latest

RUN apt update
# last 2 lines are necessary packages for building SFML
RUN apt install -y cmake build-essential wget gcc g++ unzip git clang \
           libfreetype-dev libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libogg-dev libvorbis-dev \
           libvorbisenc2 libvorbisfile3 libopenal-dev libblis-pthread-dev libopengl-dev libglew-dev libxcursor-dev

RUN mkdir /workspace
# getting sfml 2.6.0 and compiling it
RUN wget https://www.sfml-dev.org/files/SFML-2.6.0-sources.zip -O /workspace/sfml.zip && cd /workspace && unzip sfml.zip
# getting opencv
RUN wget https://github.com/opencv/opencv/archive/4.8.0.zip -O /workspace/opencv.zip && cd /workspace && unzip opencv.zip
RUN rm /workspace/sfml.zip /workspace/opencv.zip

RUN git clone https://github.com/lazarovziv/MyRPGGame.git /workspace/MyRPGGame
# setting env variables for compilation
ENV CXX=/usr/bin/clang++
ENV CC=/usr/bin/clang

# building SFML
RUN cd /workspace/SFML-2.6.0 && cmake . && make
# building OpenCV
RUN cd /workspace/opencv-4.8.0 && cmake -B build . && cd build && make

RUN cp -r /workspace/SFML-2.6.0/lib/* /usr/local/lib

WORKDIR /workspace/MyRPGGame
