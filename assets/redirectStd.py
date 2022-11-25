import sys
import nativelib

class StdBuffer(object):
    def __init__(self, type):
        self.type = type
    def write(self, data: bytes):
        nativelib.stdWrite(self.type, data)
        return
    def flush(self):
        return

sys.stdout = StdBuffer(0)
sys.stderr = StdBuffer(1)

print("redirectStd Loaded!")
