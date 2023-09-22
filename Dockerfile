# docker image for the build environment
FROM ubuntu:latest

RUN apt update
RUN apt install -y cmake build-essential wget gcc g++ unzip
RUN mkdir /workspace
# getting sfml 2.6.0 and compiling it
RUN wget https://www.sfml-dev.org/files/SFML-2.6.0-linux-gcc-64-bit.tar.gz -O /workspace/sfml.zip && cd /workspace && unzip sfml.zip
# getting opencv
RUN wget https://github.com/opencv/opencv/archive/4.8.0.zip -O /workspace/opencv.zip && unzip opencv.zip
#RUN git clone https://github.com/lazarovziv/MyRPGGame.git
