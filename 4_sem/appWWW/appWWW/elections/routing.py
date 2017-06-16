# In routing.py
from channels.routing import route
from elections.consumers import ws_add, ws_disconnect, ws_message

channel_routing = [
    route("websocket.connect", ws_add),
    route("websocket.disconnect", ws_disconnect),
    route("websocket.receive", ws_message),
]
