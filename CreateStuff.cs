using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace Csharp
{
    class Program
    {

        static async Task Main(string[] args)
        {
            string[] allFiles = Directory.GetFiles(@"C:\Development\BeatSaber\QuestMods\python\termux-packages\output\data\data\com.termux\files\usr\include\python3.10", "*.h").Concat(Directory.GetFiles(@"C:\Development\BeatSaber\QuestMods\python\termux-packages\output\data\data\com.termux\files\usr\include\python3.10\cpython", "*.h")).ToArray();
            string exports = @"C:\Users\Florian\ghidra_scripts\exports.txt";
            string declare = @"C:\Users\Florian\Desktop\declare.txt";
            string load = @"C:\Users\Florian\Desktop\load.txt";
            var types = new List<string>();
            var declares = new List<string>();
            var loads = new List<string>();
            foreach (string search in File.ReadAllLines(exports).Distinct())
            {
                string currentBuffer = null;
                foreach (string file in allFiles)
                {
                    foreach (string line in File.ReadAllLines(file))
                    {
                        if (line.StartsWith("PyAPI_FUNC(") && line.Contains(search + "("))
                        {
                            //DECLARE_DLSYM(PyObject*, PyObject_CallObject, PyObject* callable, PyObject* args);
                            var start = "PyAPI_FUNC(".Length;
                            var end = line.IndexOf(")");
                            currentBuffer = ("DECLARE_DLSYM(" + line.Substring(start, end - start) + ", " + search + ", " + line.Substring(line.IndexOf(search) + search.Length + 1)).Trim();
                            if (currentBuffer.Contains(");"))
                                break;
                        }
                        else
                        if (currentBuffer != null)
                        {
                            currentBuffer = currentBuffer + line.Trim();
                            if (currentBuffer.Contains(");"))
                                break;
                        }
                    }
                    if (currentBuffer != null)
                        break;
                }
                if (currentBuffer != null && currentBuffer.EndsWith(");"))
                {
                    if (currentBuffer.Contains("Py_GCC_ATTRIBUTE"))
                        continue;
                    Regex regex = new Regex(string.Format("Py.+?\\*"), RegexOptions.RightToLeft);
                    var currentTypes = regex.Matches(currentBuffer).Where(x =>
                    {
                        int index = x.Value.IndexOf(",");
                        if (index == -1)
                            return true;
                        return x.Value.Substring(0, index).Contains("*");
                    }
                    ).Select(x => "struct " + x.Value.Substring(0, x.Value.IndexOf("*")).Trim() + " { };").ToList();
                    types.AddRange(currentTypes.Except(types));
                    Console.WriteLine(currentBuffer);
                    declares.Add(currentBuffer);
                    loads.Add("LOAD_DLSYM(libpython, " + search + ");");
                }
            }
            File.WriteAllLines(declare, types.Concat(declares));
            File.WriteAllLines(load, loads);
        }
    }
}
