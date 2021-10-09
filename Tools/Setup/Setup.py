import sys, os, re, subprocess
#python3.9 -m py_compile Setup.py && { python3 -m PyInstaller -F --clean ./Setup.py; cp ./dist/Setup ./; rm -r ./dist; rm ./build -r; rm ./Setup.spec; }
#! This script must be compiled in an executable file


#TODO make this script an executable
#TODO generate .engine/Path_config.hpp file
#TODO read paths from config file




# Get relative to cwd and absolute application dir
cwdToP = '.' if len(sys.argv) == 1 else os.path.relpath(sys.argv[1], '.')
cwdToE = os.path.relpath(os.path.dirname(os.readlink('/proc/self/exe')) + '/../..')


# Check the application dir and ask for confirmation
if not os.path.exists(cwdToP):
    print("The specified application path does not exist")
    exit(-2)
else:
    print( #TODO update
        f'The following files will be created or overwritten:'
        f'\n    .'
        f'\n    ├─ .engine'
        f'\n    │   ├─ Build.Application.sh'
        f'\n    │   ├─ Build.Engine.sh'
        f'\n    │   ├─ Config.hpp'
        f'\n    │   ├─ .Aabs'
        f'\n    │   ├─ .Eabs'
        f'\n    │   ├─ .AtoE'
        f'\n    │   └─ .EtoA'
        f'\n    └─ .vscode'
        f'\n        ├─ tasks.json'
        f'\n        └─ c_cpp_properties.json'
        f'\n\nContinue? (y/n)'
    )
os.path.dirname(os.getcwd())




if sys.stdin.read(1).lower() == 'y':
    # Create missing directories
    if not os.path.exists(cwdToP + "/.engine"): os.mkdir(cwdToP + "/.engine")
    if not os.path.exists(cwdToP + "/.vscode"): os.mkdir(cwdToP + "/.vscode")


    # Write paths
    Aabs = os.path.abspath(cwdToP)
    Eabs = os.path.abspath(cwdToE)
    AtoE = os.path.relpath(Eabs, cwdToP)
    EtoA = os.path.relpath(cwdToP, Eabs)

    os.chdir(cwdToP)
    with open('.engine/.Aabs', 'w') as f: f.write(Aabs)   # Absolute path of Application dir
    with open('.engine/.Eabs', 'w') as f: f.write(Eabs)   # Absolute path of Engine SDK
    with open('.engine/.AtoE', 'w') as f: f.write(AtoE)   # Application to Engine relative path
    with open('.engine/.EtoA', 'w') as f: f.write(EtoA)   # Engine to Application relative path




    # Write vscode tasks
    with open('.engine/Build.Engine.sh', 'w') as f:
        f.write(
            f'\n##################################################################'
            f'\n# This file contains the command used to build the Lynx Engine'
            f'\n# Bash syntax. Lines are concatenated and comments are ignored'
            f'\n##################################################################'
            f'\n'
            f'\n'
            f'\n#--mode=ld #! Build mode and target platform are copied from the application build'
            f'\n-a: # Source files'
            f'\n    src/Lynx/System/SystemInfo.cpp'
            f'\n    src/Lynx/Tests/StructureInit.cpp'
            f'\n'
            f'\n    src/Lynx/Core/Memory/Ram/Ram.cpp'
            f'\n    src/Lynx/Core/Memory/Ram/Cell_t.cpp'
            f'\n'
            f'\n    src/Lynx/System/System.cpp'
            f'\n'
            f'\n    src/Lynx/Core/Devices.cpp'
            f'\n    src/Lynx/Core/Memory/VRam/VRam.cpp'
            f'\n    src/Lynx/Core/Memory/VRam/VCell_t.cpp'
            f'\n'
            f'\n    src/Lynx/Core/Render/GCommands.cpp'
            f'\n    src/Lynx/Core/Render/Shaders/Shader.cpp'
            f'\n    src/Lynx/Core/Render/Window/Window.cpp #FIXME'
            f'\n    src/Lynx/Core/Render/Window/Swapchain.cpp'
            f'\n    src/Lynx/Core/Core.cpp'
            f'\n'
            f'\n    src/Lynx/Threads/ThreadPool.cpp'
            f'\n'
            f'\n    src/Lynx/Types/Object/Obj_b.cpp'
            f'\n    src/Lynx/Types/Object/2D/Line2.cpp'
            f'\n    src/Lynx/Types/Object/2D/Border2.cpp'
            f'\n    src/Lynx/Types/Object/2D/RenderSpace2.cpp'
            f'\n    src/Lynx/Types/Object/2D/RenderSpace3.cpp'
            f'\n'
            f'\n    src/Lynx/Types/Object/3D/Volume.cpp'
            f'\n'
            f'\n    src/Lynx/Core/Render/Buffers.cpp'
            f'\n    src/Lynx/Core/Render/Render.cpp'
            f'\n'
            f'\n    src/Lynx/Core/Input/Input.cpp'
            f'\n    src/Lynx/Core/Input/InputState.cpp'
            f'\n'
            f'\n-a: # App support source files'
            f'\n    src/Lynx/getEnginePath.cpp'
            f'\n    src/Lynx/Core/Env.cpp'
            f'\n'
            f'\n-a: # Shader source files'
            f'\n    src/Lynx/shaders/*.comp'
            f'\n'
            f'\n-a: # Use AVX'
            f'\n    -mavx'
            f'\n'
            f'\n-d: # Debug options'
            f'\n    -p -g3 -ggdb3 -fverbose-asm                                             # Debug informations'
            f'\n    -O0 -fno-elide-constructors -fno-inline-small-functions -fno-inline     # Optimizations'
            f'\n'
            f'\n-r: # Release options'
            f'\n    -Ofast -frename-registers -funroll-loops                                # Optimizations'
            f'\n    -g0                                                                     # Debug informations'
            f'\n'
            f'\n-d: # Sanitizers'
            f'\n    -fsanitize=undefined'
            f'\n    -fsanitize=alignment'
            f'\n    -fsanitize=bounds'
            f'\n    -fsanitize=null'
            f'\n    -fsanitize=vptr'
            f'\n    -fsanitize=enum'
            f'\n    -fsanitize=leak      #! Not compatible with thread'
            f'\n    -fsanitize=address   #! Not compatible with thread'
            f'\n    #-fsanitize=thread   #! Not compatible leak or address'
            f'\n'
            f'\n-a: # Warnings'
            f'\n    -Wall'
            f'\n    -Wclobbered'
            f'\n    -Wcast-function-type'
            f'\n    -Wdeprecated-copy'
            f'\n    -Wempty-body'
            f'\n    -Wignored-qualifiers'
            f'\n    -Wimplicit-fallthrough=3'
            f'\n    -Wstring-compare'
            f'\n    -Wredundant-move'
            f'\n    -Wtype-limits'
            f'\n    -Wuninitialized'
            f'\n    -Wshift-negative-value'
            f'\n    -Wunused-but-set-parameter'
            f'\n    -Wcast-align'
            f'\n    -Wcast-qual'
            f'\n    -Wctor-dtor-privacy'
            f'\n    -Wdisabled-optimization'
            f'\n    -Wformat=2'
            f'\n    -Winit-self'
            f'\n    -Wlogical-op'
            f'\n    -Wmissing-include-dirs'
            f'\n    -Wnoexcept'
            f'\n    -Woverloaded-virtual'
            f'\n    -Wredundant-decls'
            f'\n    -Wshadow'
            f'\n    -Wsign-conversion'
            f'\n    -Wsign-promo'
            f'\n    -Wstrict-null-sentinel'
            f'\n    -Wstrict-overflow=5'
            f'\n    -Wswitch-default'
            f'\n    -Wundef'
            f'\n'
            f'\n    #! Hard coded -c'
            f'\n    #! Hard coded output paths'
        )


    with open('.engine/Build.Application.sh', 'w') as f:
        f.write(
            f'\n##################################################################'
            f'\n# This file contains the command used to build your application'
            f'\n# Bash syntax. Lines are concatenated and comments are ignored'
            f'\n##################################################################'
            f'\n'
            f'\n'
            f'--mode=ld #! Changed by vscode'
            f'\n-d:'
            f'\n    -ggdb3 -g3'
            f'\n    -O0'
            f'\n'
            f'\n-d: # Sanitizers'
            f'\n    -fsanitize=undefined'
            f'\n    -fsanitize=alignment'
            f'\n    -fsanitize=bounds'
            f'\n    -fsanitize=null'
            f'\n    -fsanitize=vptr'
            f'\n    -fsanitize=enum'
            f'\n    -fsanitize=leak      #! Not compatible with thread'
            f'\n    -fsanitize=address   #! Not compatible with thread'
            f'\n    #-fsanitize=thread   #! Not compatible leak or address'
            f'\n'
            f'\n-r:'
            f'\n    -g0'
            f'\n    -O3'
            f'\n'
            f'\n-a:'
            f'\n    main.cpp #TODO add example file'
            f'\n    #-o LynxEngineTest #TODO'
        )



    subprocess.run(['python3', f'{ AtoE }/Tools/Setup/UpdateBuild.py', 'ld'])
    print('Setup completed successfully')