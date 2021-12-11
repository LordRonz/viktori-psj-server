#!/usr/bin/env bash

gcc -o server.out main.c server.c format.c io.c -pthread
