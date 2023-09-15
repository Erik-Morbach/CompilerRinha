# syntax=docker/dockerfile:1
FROM ubuntu:latest

RUN apt update && \
    apt install -y g++ git make curl tar zip cmake build-essential libjsoncpp-dev

RUN ln -s /usr/include/jsoncpp/json/ /usr/include/json

WORKDIR /CompilerRinha
COPY src/ ./src/
COPY include/ ./include/
COPY CMakeLists.txt .
WORKDIR /CompilerRinha/build
RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .
CMD [ "./CompilerRinha", "/var/rinha/source.rinha.json"]
