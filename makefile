RAYLIB_LINK=https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
RAYGUI_LINK=https://github.com/raysan5/raygui/archive/refs/tags/4.0.tar.gz

SRCS=*.c
LIBS=build/libs/*.a -lopengl32 -lgdi32 -lwinmm
INCS=-I"." -I"build/"
OUT=main.exe
CFLAG=-static -fno-builtin -Wl,--gc-sections -s

all:
	@gcc $(SRCS) -o $(OUT) $(LIBS) $(CFLAG) $(INCS)
run:	
	main.exe test.tdat

setup: install raylib raygui clean
install:
	@if not exist tmp mkdir tmp
	@if not exist build mkdir build
	@cd build && if not exist objects mkdir objects
	@cd build && if not exist libs mkdir libs
	@curl -L -o tmp/raylib-5.5.tar.gz $(RAYLIB_LINK)
	@curl -L -o tmp/raygui.tar.gz $(RAYGUI_LINK)
	@cd tmp && tar -xzf raylib-5.5.tar.gz && tar -xzf raygui.tar.gz && del *.gz
raylib:
	copy tmp\raylib-5.5\src\*.h build\ 
	$(MAKE) -C tmp/raylib-5.5/src PLATFORM=PLATFORM_DESKTOP
	move tmp\raylib-5.5\src\libraylib.a build\libs
raygui:
	copy tmp\raygui-4.0\src\*.h build\ 
clean:
	rd /s /q tmp
	rd /s /q build\objects
