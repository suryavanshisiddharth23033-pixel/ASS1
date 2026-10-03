#!/bin/bash

FILE=$1

sitar translate "$FILE"
sitar compile
./sitar_sim 20000