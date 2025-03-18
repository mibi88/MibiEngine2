gcc src/main.c src/mat.c src/window.c src/model.c src/shader.c src/types.c \
    src/modelarray.c src/obj.c src/texture.c src/image.c src/stdmodel.c \
    src/texturedmodel.c src/framebuffer.c \
    -o main -ansi -Isrc -lEGL -lm -lX11 -lGL -lpng -Wall -Wextra -Wpedantic -g

