import BuildOptions
from ProjectPath import ProjectPath, EnginePath
from Platform import Platform as pf
from Type import Type as tp
import json
import os.path
thisdir = os.path.dirname(os.path.abspath(__file__))




def getPf():
    return "Linux" if pf == "l" else "Windows"
def getTp():
    return "Debug" if tp == "d" else "Release"




f = open(thisdir + "/../" + ProjectPath + "/.vscode/tasks.json", "r")

while(f.read(1) != '['):
    None
while(f.read(1) != '['):
    None

userBuild = ''
n = 1
while(n > 0):
    c = f.read(1)
    if(c == '['): n += 1
    elif(c == ']'): n -= 1
    userBuild += c

f.close()




f = open(thisdir + "/../" + ProjectPath + "/.vscode/tasks.json", "w")
f.write("""
//This file was generated by the engine
//Only modify the "Build application" task args
//Other changes could be overwritten without any advise
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": \"""" + getPf() + "  |  " + getTp() + """  |  Build Application",
            "command": "python3",
            "args": [""" + userBuild + """,
            "problemMatcher": [ "$gcc" ],
            "options": { "cwd": "${workspaceFolder}" },
            "group": { "kind": "build", "isDefault": true }
        },
        {
            "type": "shell",
            "label": \"""" + getPf() + "  |  " + getTp() + """  |  Build LuxEngine\",
            "command": "/usr/bin/g++",
            "args": [
                //Source files
		            "-xc++", \"""" + EnginePath + """/LuxEngine/LuxEngineBuild.cpp.build",
		            "Test.cpp",\n""" +\
                    (BuildOptions.getDebugOptions() if tp == "d" else BuildOptions.getReleaseOptions()) + """,
                //Engine
                    "-std=c++2a", "-mavx", "-pipe", "-pthread",
                    "-I./", "-ILuxEngine",
                    "-Wall",\n""" +\
                    BuildOptions.getEngineDeps() + """,
                //Output
					"-o", "./Build/""" + getPf() + '/LuxEngine' + getTp() + """\"
            ],
            "problemMatcher": [ "$gcc" ],
            "options": { "cwd": "${workspaceFolder}" },
            "group": { "kind": "build", "isDefault": true }
        },
        {
            "type": "shell",
            "label": " > Switch to """ + ("Windows" if pf == "l" else "Linux") + """\",
            "command": "python3",
            "args": [
                \"""" + EnginePath + """/Build/SetPlatform.py",
                \"""" + ("w" if pf == "l" else "l") + """\",
            ],
            "problemMatcher": [ ],
            "options": { "cwd": "${workspaceFolder}" },
            "group": { "kind": "build", "isDefault": true }
        },
        {
            "type": "shell",
            "label": " > Switch to """ + ("Release" if tp == "d" else "Debug") + """\",
            "command": "python3",
            "args": [
                \"""" + EnginePath + """/Build/SetType.py",
                \"""" + ("r" if tp == "d" else "d") + """\"
            ],
            "problemMatcher": [ ],
            "options": { "cwd": "${workspaceFolder}" },
            "group": { "kind": "build", "isDefault": true }
        }
    ]
}\n""")
f.close()


os.system("rm " + thisdir + "/__pycache__ -r")