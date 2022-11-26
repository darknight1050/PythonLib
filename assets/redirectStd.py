import sys
import nativelib

class StdBuffer(object):
    def __init__(self, type):
        self.type = type
    def write(self, data):
        for x in data.splitlines():
            nativelib.std_write(self.type, x)
        return
    def flush(self):
        return

sys.stdout = StdBuffer(0)
sys.stderr = StdBuffer(1)

print("redirectStd Loaded!")