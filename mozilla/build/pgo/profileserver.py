#!/usr/bin/python

import SimpleHTTPServer
import SocketServer
import socket
import threading
import os
import sys
import shutil
from datetime import datetime
import automation

PORT = 8888
SCRIPT_DIR = os.path.abspath(os.path.realpath(os.path.dirname(sys.argv[0])))
PROFILE_DIRECTORY = os.path.abspath(os.path.join(SCRIPT_DIR, "./pgoprofile"))
os.chdir(SCRIPT_DIR)

class EasyServer(SocketServer.TCPServer):
  allow_reuse_address = True

if __name__ == '__main__':
  httpd = EasyServer(("", PORT), SimpleHTTPServer.SimpleHTTPRequestHandler)
  t = threading.Thread(target=httpd.serve_forever)
  t.setDaemon(True) # don't hang on exit
  t.start()

  automation.initializeProfile(PROFILE_DIRECTORY)
  browserEnv = dict(os.environ)
 
  # These variables are necessary for correct application startup; change
  # via the commandline at your own risk.
  browserEnv["NO_EM_RESTART"] = "1"
  browserEnv["XPCOM_DEBUG_BREAK"] = "warn"
  if automation.UNIXISH:
    browserEnv["LD_LIBRARY_PATH"] = os.path.join(SCRIPT_DIR, automation.DIST_BIN)
    browserEnv["MOZILLA_FIVE_HOME"] = os.path.join(SCRIPT_DIR, automation.DIST_BIN)

  automation.runApp("http://localhost:%d/index.html" % PORT, browserEnv,
                    os.path.join(SCRIPT_DIR, automation.DEFAULT_APP),
                    PROFILE_DIRECTORY, ())
