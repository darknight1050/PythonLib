import json
import os
import sys

def GetScriptDirectory():
    return getSourceFile().getParentFile().toString()
  
sm = currentProgram.getSymbolTable()
symb = sm.getDefinedSymbols()
f = open(os.path.join(GetScriptDirectory(), ".\\exports.txt"), "w")
for s in symb:
  if str(s).startswith("Py"):
    f.write(str(s) + "\n")

f.close()