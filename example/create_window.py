import tsdl
import collections

event_dict = {getattr(tsdl.SDL_EventType, attr) : attr for attr in dir(tsdl.SDL_EventType) if isinstance(getattr(tsdl.SDL_EventType, attr), collections.Hashable)}

with tsdl.TSDL():
    with tsdl.Window("TSDL Example", tsdl.SDL_WINDOWPOS_UNDEFINED, tsdl.SDL_WINDOWPOS_UNDEFINED, 1280, 720, tsdl.SDL_WindowFlags.SDL_WINDOW_SHOWN) as window:
        surface = window.window_surface_object()
        surface.fill(255, 255, 255)
        with tsdl.Eventloop() as evloop:
            def render_handler():
                window.update_window_surface()
            def quit_handler(event):
                print("received {} ({})".format(event_dict[event.type], event.type))
                evloop.interrupt()
            evloop.render_function(render_handler)
            evloop.add_event_handler(tsdl.SDL_EventType.SDL_QUIT, quit_handler)
            evloop.run()
