# BINIAX-2 1.3 MAKEFILE FOR UNIX/LINUX/MACOS SYSTEMS
# (C) 2005-2009, JORDAN TUZSUZOV
# To build the game you need the development libraries of SDL 1.2, SDL_mixer and SDL_image.
# To execute the game you need the runtimes of SDL, SDL_mixer and SDL_image.

FLAGS+=`dpkg-buildflags --get CFLAGS`
FLAGS+=`dpkg-buildflags --get CPPFLAGS`
CC=gcc
FILES=biniax.c hof.c desktop/cfg.c desktop/gfx.c desktop/snd.c desktop/inp.c desktop/sys.c
INCLUDES=-I . -I desktop
LINKTO=-lSDL -lSDL_mixer -lSDL_image
LINKTO+=`dpkg-buildflags --get LDFLAGS`
DATADIR=/usr/share/games/biniax2
AUTO=`sdl-config --libs --cflags` -DDATADIR=\"$(DATADIR)\"
TARGET=biniax2

biniax:
	$(CC) $(FLAGS) $(AUTO) $(FILES) $(INCLUDES) -o $(TARGET) $(LINKTO)
