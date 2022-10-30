import sys
import ctypes
libpythonlib = ctypes.cdll.LoadLibrary("")

class StdoutBuffer(object):
    def __init__(self, type):
        self.type = type
    def write(self, data: bytes):
        libpythonlib.pythonWrite(self.type, data.encode("utf-8"))
        return
    def flush(self):
        return

sys.stdout = StdoutBuffer(0)
sys.stderr = StdoutBuffer(1)

def setHandle(handle):
    libpythonlib._handle = handle
    libpythonlib.pythonWrite.argtypes = [ctypes.c_int, ctypes.c_char_p]