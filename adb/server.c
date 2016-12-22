import sys
import os
import socket,select

import sys
import os
import socket,select

BUFSIZE=4096

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(("0.0.0.0", 5556))
sock.listen(0)

sockD5 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockD5.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sockD5.bind(("0.0.0.0", 5555))
sockD5.listen(0)

android = None
pc = None

while True:
  inSd = [sock,sockD5]
  if android:
    inSd.append(android)
  if pc:
    inSd.append(pc)
  rb,wr,ep = select.select(inSd,[],[],1)
  if sock in rb:
    android,addr = sock.accept()
    print "connect from android",addr
  elif sockD5 in rb:
    pc,addr = sockD5.accept()
    print "connect from pc",addr
  elif android in rb:
    pc.send(android.recv(BUFSIZE))
  elif pc in rb:
    android.send(pc.recv(BUFSIZE))

sock.close()
sockD5.close()
