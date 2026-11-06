Biniax-2 1.30 for AmigaOS 3.x
=============================

Biniax-2 (Jordan Tuzsuzov, https://www.biniax.com), the puzzle game of
logic and reaction: move your block through the field and take pairs of
elements, before the rising lines push you off the board. Arcade (against
time), Tactic (turn by turn) and a two-player game on one keyboard. For
AmigaOS 3.x on a 68040 or 68060 with an FPU and an RTG screen, on
OpenGPU's SDL 1.2 (sdl12-compat).

Base: biniax2_1.30.orig.tar.gz from Debian (sha256
2c09f6f0a5fe453a409d990d72a964636ee91ca91daac99f0db861b8f42531bf), with
Debian's patches (1.30-8) as the second commit. Branch
team2/biniax2-amiga.

Status: plays in the lab (Instance-45 on GFOpenUp 0.8.19, 6 November
2026): the menu (keyboard), an Arcade game with its music (the block
moving, a pair taken, the score, game over), the Hall of Fame, and Exit
back to the Shell; config.bnx2 and hof.bnx2 written in the drawer. The
modules and sounds load without an error; the lab is muted, so they were
not heard there.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- GFOpenUp 0.8.18 or later (LIBS:OpenGPU/SDL2.module). Everything else is
  in the program.


How to play
-----------

Open the Biniax2 icon, or run Biniax2 from a Shell. It opens an 800x600
window. The cursor keys move in the menu and Return chooses; in a game
the cursor keys move your block, Space clears a column (once in a while)
and Escape is the menu; the second player has W, A, S and D. Help on the
menu has the rules. The settings (config.bnx2), the game left off
(autosave.bnx2) and the Hall of Fame (hof.bnx2) are kept in the drawer;
MUSIC=0 in config.bnx2 turns the music off.


Building
--------

    make -f amiga/Makefile.amigaos dist

with GFOpenAmigaGCC 16.2.0-open4, the OpenNDK headers, the OpenGPU kit and
the Team's static libpng and zlib (see the Makefile's head). The drawer is
dist/Biniax2 (and dist/Biniax2.info).


What was changed
----------------

- biniax.c, hof.c, desktop/cfg.c: on AmigaOS the settings, the saved game
  and the Hall of Fame are PROGDIR:config.bnx2, PROGDIR:autosave.bnx2 and
  PROGDIR:hof.bnx2 (Debian's patches put them in $HOME, through
  getpwuid); the Hall of Fame starts with upstream's names again. The data
  is PROGDIR:data (DATADIR).
- amiga/Makefile.amigaos builds the makefile's sources (not the network
  play, which it leaves out too), with -fcommon: the headers define the
  game's globals, as GCC before 10 allowed.
- No SDL_image or SDL_mixer 1.2 for OpenGPU's SDL 1.2: the Team's
  amiga/sdl_image_lite.c (PNGs on libpng, interlaced ones too, their tRNS
  colour the colour key as SDL_image makes it) and amiga/sdl_mixer_lite.c
  (WAV, with libmikmod 3.3.16 for the Impulse Tracker music:
  amiga/libmikmod, its player, IT loader and no-sound driver), both from
  the TecnoballZ port. The music has at most 16 voices: with mikmod's 64
  the 68k in the lab spent all its time mixing and the game stopped
  answering.
- amiga/stack.c: 256 KB of stack.


Licences
--------

Biniax-2, its pictures and music: zlib licence (COPYING.txt). libmikmod:
LGPL 2 or later (COPYING-libmikmod.txt). The Amiga files (amiga/): the
AmiCradle Team, zlib licence, except the mixer, which comes from the
Team's Wizznic port and is GPL 3 or later, so the program as built here is
GPL 3 or later.
