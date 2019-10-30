import tsdl

all_events = [(attr, getattr(tsdl.SDL_EventType, attr)) for attr in dir(tsdl.SDL_EventType)]

def get_event_name(event):
    r = [et[0] for et in filter(lambda ev: ev[1] == event.type, all_events)]
    assert len(r) <= 1, "more than 1 event value for {}: {}, SDL_EventType is messed up in cpp".format(event.type, ', '.join(r))
    return r[0] if r else None

with tsdl.TSDL():
    with tsdl.Window("TSDL Example", tsdl.SDL_WINDOWPOS_UNDEFINED, tsdl.SDL_WINDOWPOS_UNDEFINED, 1280, 720, tsdl.SDL_WindowFlags.SDL_WINDOW_SHOWN) as window:
        surface = window.window_surface_object()
        surface.fill(255, 255, 255)
        with tsdl.Eventloop() as evloop:
            def render_handler():
                window.update_window_surface()
            def quit_handler(event):
                print("received {} ({})".format(get_event_name(event), event.type))
                evloop.interrupt()
            evloop.render_function(render_handler)
            evloop.add_event_handler(tsdl.SDL_EventType.SDL_QUIT, quit_handler)
            evloop.run()
