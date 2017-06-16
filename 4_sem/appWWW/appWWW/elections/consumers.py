from django.http import HttpResponse
from channels.handler import AsgiHandler
from channels.channel import Group

def ws_add(message):
    # Accept the connection
    message.reply_channel.send({"accept": True})
    # Add to the chat group
    Group("votes-updates").add(message.reply_channel)

# Connected to websocket.disconnect
def ws_disconnect(message):
    Group("votes-updates").discard(message.reply_channel)

def ws_message(message):
    Group("votes-updates").send({
        "text": message['text'],
    })
