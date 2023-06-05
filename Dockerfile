FROM ubuntu

RUN apt update && apt install -y --no-install-recommends \
        build-essential \
        cmake \
        lib32ncurses-dev \
        zlib1g-dev

WORKDIR /home/evalvid

COPY . /home/evalvid/

RUN cp -R libgpac/gpac /usr/include \
    && cp libgpac/libgpac_static.a /usr/lib \
    && cd /usr/local/lib \
    && ln -s  /usr/local/lib64/* . \
    && ldconfig

CMD ["/bin/bash"]

