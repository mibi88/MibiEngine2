gcc src/*.c src/backends/gles/*.c \
    -o main -ansi -Isrc -Isrc/backends -lEGL -lm -lX11 -lGL -lpng -Wall \
    -Wextra -Wpedantic -g

