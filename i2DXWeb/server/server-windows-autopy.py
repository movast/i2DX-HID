import sys
import libi2dx
from os import path
from autopy import key
import ConfigParser
import OSC

client = OSC.OSCClient()
client.connect( ('127.0.0.1', 57120) )

class I2DXWebSocketAutoPy(libi2dx.I2DXWebSocket):
	def toggle_key(self, key_id, state, player):
		try:
			#key.toggle(libi2dx.config.get('keymap_player%s' % player, key_id), state)
			print "Creating message..."
			msg = OSC.OSCMessage()
			msg.setAddress(("/%s"% key_id))
			msg.append(1 if state else 0)
			msg.append(player)
			print msg;
			client.send(msg)
		except ConfigParser.NoOptionError:
			print "key map not found for %s, player %s" % (key_id, player)

if __name__ == "__main__":
	libi2dx.serve(I2DXWebSocketAutoPy)
