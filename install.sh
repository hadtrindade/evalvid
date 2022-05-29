#!/bin/bash

# Fail on error
set -e

# Fail on unset var usage
set -o nounset

NCURSES=$(apt-cache search lib32ncurses-dev | grep lib32ncurses-dev | cut -d" " -f1)

if [ -z $NCURSES ] 
then 
    LIBNCURSES="lib32ncursesw5-dev" 
fi

export DEBIAN_FRONTEND=noninteractive
export TZ=America/Recife

apt-get update && apt-get install -y --no-install-recommends \
    gpac \ 
    ffmpeg \
    mediainfo \
    $NCURSES