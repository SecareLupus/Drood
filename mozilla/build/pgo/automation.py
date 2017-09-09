
from datetime import datetime
import itertools
import logging
import shutil
import os
import signal
import sys
import threading

"""
Runs the browser from a script, and provides useful utilities
for setting up the browser environment.
"""

__all__ = [
           "UNIXISH",
           "IS_WIN32",
           "runApp",
           "Process",
           "initializeProfile",
           "DIST_BIN",
           "DEFAULT_APP",
          ]

servers = [
           "localhost:8888", # MUST be first -- see PAC pref-setting code
           "example.org:80",
           "test1.example.org:80",
           "test2.example.org:80",
           "sub1.test1.example.org:80",
           "sub1.test2.example.org:80",
           "sub2.test1.example.org:80",
           "sub2.test2.example.org:80",
           "example.org:8000",
           "test1.example.org:8000",
           "test2.example.org:8000",
           "sub1.test1.example.org:8000",
           "sub1.test2.example.org:8000",
           "sub2.test1.example.org:8000",
           "sub2.test2.example.org:8000",
           "example.com:80",
           "test1.example.com:80",
           "test2.example.com:80",
           "sub1.test1.example.com:80",
           "sub1.test2.example.com:80",
           "sub2.test1.example.com:80",
           "sub2.test2.example.com:80",
           "sectest1.example.org:80",
           "sub.sectest2.example.org:80",
           "sub1.xn--lt-uia.example.org:8000", # U+00E4 U+006C U+0074
           "sub2.xn--lt-uia.example.org:80",   # U+00E4 U+006C U+0074
           "xn--exmple-cua.test:80",
           "sub1.xn--exmple-cua.test:80",
           "xn--hxajbheg2az3al.xn--jxalpdlp:80", # Greek IDN for example.test
           "sub1.xn--hxajbheg2az3al.xn--jxalpdlp:80",
          ]

unprivilegedServers = [
                       "sectest2.example.org:80",
                       "sub.sectest1.example.org:80",
                      ]


DIST_BIN = "./" + "../../dist/bin"
IS_WIN32 = len("") != 0
IS_MAC = 0 != 0
IS_CYGWIN = False
IS_CAMINO = 0 != 0

UNIXISH = not IS_WIN32 and not IS_MAC

DEFAULT_APP = "./" + "../../dist/bin/drood-bin"


log = logging.getLogger()
handler = logging.StreamHandler(sys.stdout)
log.setLevel(logging.INFO)
log.addHandler(handler)



class Process:
  """
  Represents a subprocess of this process.  We don't just directly use the
  subprocess module here because we want compatibility with Python 2.3 on
  non-Windows platforms.  :-(
  """

  def __init__(self, command, args, env):
    """
    Creates a process representing the execution of the given command, which
    must be an absolute path, with the given arguments in the given environment.
    The process is then started.
    """
    command = os.path.abspath(command)
    if IS_WIN32:
      import subprocess
      cmd = [command]
      cmd.extend(args)
      p = subprocess.Popen(cmd, env = env,
                           stdout = subprocess.PIPE,
                           stderr = subprocess.STDOUT)
      self._out = p.stdout
    else:
      import popen2
      cmd = []
      for (k, v) in env.iteritems():
        cmd.append(k + "='" + v + "' ")
      cmd.append("'" + command + "'")
      cmd.extend(map(lambda x: "'" + x + "'", args))
      cmd = " ".join(cmd)
      p = popen2.Popen4(cmd)
      self._out = p.fromchild

    self._process = p
    self.pid = p.pid
    
    self._thread = threading.Thread(target = lambda: self._run())
    self._thread.start()

  def _run(self):
    "Continues execution of this process on a separate thread."
    p = self._process
    out = self._out

    if IS_WIN32:
      running = lambda: p.poll() is None
    else:
      running = lambda: p.poll() == -1

    # read in lines until the process finishes, then read in any last remaining
    # buffered lines
    while running():
      line = out.readline().rstrip()
      if len(line) > 0:
        log.info(line)
    for line in out:
      line = line.rstrip()
      if len(line) > 0:
        log.info(line)
    self._status = p.poll()

  def wait(self):
    "Waits for this process to finish, then returns the process's status."
    self._thread.join()
    return self._status

  def kill(self):
    "Kills this process."
    try:
      if not IS_WIN32: # XXX
        os.kill(self._process.pid, signal.SIGKILL)
    except:
      pass



def initializeProfile(profileDir):
  "Sets up the standard testing profile."

  # Start with a clean slate.
  shutil.rmtree(profileDir, True)
  os.mkdir(profileDir)

  prefs = []

  part = """\
user_pref("browser.dom.window.dump.enabled", true);
user_pref("dom.disable_open_during_load", false);
user_pref("dom.max_script_run_time", 0); // no slow script dialogs
user_pref("signed.applets.codebase_principal_support", true);
user_pref("security.warn_submit_insecure", false);
user_pref("browser.shell.checkDefaultBrowser", false);
user_pref("browser.warnOnQuit", false);
user_pref("accessibility.typeaheadfind.autostart", false);
user_pref("javascript.options.showInConsole", true);
user_pref("layout.debug.enable_data_xbl", true);
user_pref("browser.EULA.override", true);

user_pref("camino.warn_when_closing", false); // Camino-only, harmless to others
"""
  prefs.append(part)

  # Grant God-power to all the servers on which tests can run.
  for (i, server) in itertools.izip(itertools.count(1), servers):
    part = """
user_pref("capability.principal.codebase.p%(i)d.granted",
          "UniversalXPConnect UniversalBrowserRead UniversalBrowserWrite \
           UniversalPreferencesRead UniversalPreferencesWrite \
           UniversalFileRead");
user_pref("capability.principal.codebase.p%(i)d.id", "http://%(server)s");
user_pref("capability.principal.codebase.p%(i)d.subjectName", "");
"""  % {"i": i, "server": server}
    prefs.append(part)

  # Now add the two servers that do NOT have God-power so we can properly test
  # the granting and receiving of God-power.  Strip off the first server because
  # we proxy all the others to it.
  allServers = servers[1:] + unprivilegedServers


  # Now actually create the preference to make the proxying happen.
  quotedServers = ", ".join(map(lambda x: "'" + x + "'", allServers))

  pacURL = """data:text/plain,
function FindProxyForURL(url, host)
{
  var servers = [%(quotedServers)s];
  var regex = new RegExp('http://(?:[^/@]*@)?(.*?(:\\\\\\\\d+)?)/');
  var matches = regex.exec(url);
  if (!matches)
    return 'DIRECT';
  var hostport = matches[1], port = matches[2];
  if (!port)
    hostport += ':80';
  if (servers.indexOf(hostport) >= 0)
    return 'PROXY localhost:8888';
  return 'DIRECT';
}""" % {"quotedServers": quotedServers}
  pacURL = "".join(pacURL.splitlines())

  part = """
user_pref("network.proxy.type", 2);
user_pref("network.proxy.autoconfig_url", "%(pacURL)s");

user_pref("camino.use_system_proxy_settings", false); // Camino-only, harmless to others
""" % {"pacURL": pacURL}
  prefs.append(part)

  # write the preferences
  prefsFile = open(profileDir + "/" + "user.js", "a")
  prefsFile.write("".join(prefs))
  prefsFile.close()



def runApp(testURL, env, app, profileDir, extraArgs):
  "Run the app, returning the time at which it was started."
  # mark the start
  start = datetime.now()

  # now run with the profile we created
  cmd = app
  if IS_MAC and not IS_CAMINO and not cmd.endswith("-bin"):
    cmd += "-bin"
  cmd = os.path.abspath(cmd)

  args = []
  if IS_MAC:
    args.append("-foreground")

  if IS_CYGWIN:
    profileDirectory = commands.getoutput("cygpath -w \"" + profileDir + "/\"")
  else:
    profileDirectory = profileDir + "/"

  args.extend(("-no-remote", "-profile", profileDirectory))
  if IS_CAMINO:
    args.extend(("-url", testURL))
  else:
    args.append((testURL))
  args.extend(extraArgs)
  proc = Process(cmd, args, env = env)
  log.info("Application pid: %d", proc.pid)
  status = proc.wait()
  if status != 0:
    log.info("ERROR FAIL Exited with code %d during test run", status)

  return start
