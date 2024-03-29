#!/usr/bin/sh

set -xe

C=clang
F="-Wall -Wextra"

$C $F -o png process_png.c
