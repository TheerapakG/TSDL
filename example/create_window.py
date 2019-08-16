import time
import tsdl

with tsdl.TSDL():
    with tsdl.Window("TSDL Example", tsdl.SDL_WINDOWPOS_UNDEFINED, tsdl.SDL_WINDOWPOS_UNDEFINED, 1280, 720, tsdl.SDL_WindowFlags.SDL_WINDOW_SHOWN):
        time.sleep(10)