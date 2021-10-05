import os, re, sys, subprocess
from textwrap import indent
from math import ceil
from argparse import Namespace as ns
#! Shaders are validated in lynxg++
#! This script is compiled with lynxg++
#TODO write what external bindings are and how to use them
#TODO add matrix support
#TODO add image support
#TODO add std140 support

#FIXME fix array reference translation
#FIXME stop program if std or other stuff cannot be found

#TODO In WGPU
#TODO To make uniform buffers portable they have to be std140 and not std430. Uniform structs have to be std140. Storage structs have to be std430
#TODO Storage buffers for compute shaders can be std140 or std430



# Element parsing ################################################################################################################








def roundUp(x : int, b : int) -> int :
    return b * ceil(x / b)






def parseElms(glsl:str) :
    """!
        Parses the elements of a layout
        Returns a namespace with a list of namespaces containing
        the type, name, array length and offset of each element, and the total size of the C++ structure
    """
    elms     : list = []
    maxAlign : int  = 0


    typeName = {
        'bvec2': 'bv2',    'ivec2': 'i32v2',    'uvec2': 'u32v2',    'vec2': 'f32v2',    'dvec2': 'f64v2',
        'bvec3': 'bv3',    'ivec3': 'i32v3',    'uvec3': 'u32v3',    'vec3': 'f32v3',    'dvec3': 'f64v3',
        'bvec4': 'bv4',    'ivec4': 'i32v4',    'uvec4': 'u32v4',    'vec4': 'f32v4',    'dvec4': 'f64v4',
         'bool': 'bool',     'int': 'i32',       'uint': 'u32',     'float': 'f32',     'double': 'f64'
    }
    typeSize = {
        'bvec2': 4 * 2,    'ivec2': 4 * 2,      'uvec2': 4 * 2,      'vec2': 4 * 2,      'dvec2': 8 * 2,
        'bvec3': 4 * 4,    'ivec3': 4 * 4,      'uvec3': 4 * 4,      'vec3': 4 * 4,      'dvec3': 8 * 4,
        'bvec4': 4 * 4,    'ivec4': 4 * 4,      'uvec4': 4 * 4,      'vec4': 4 * 4,      'dvec4': 8 * 4,
         'bool': 4,          'int': 4,           'uint': 4,         'float': 4,         'double': 8
    }


#FIXME calculate array offset
    offset : int = 0
    for rInfo in re.finditer(                                    # For each binding member
        r'(?P<type>([biud]?vec[234])|(double|float|bool|(u?int))) ' # Get type name
        r'(?P<name>[a-zA-Z_]{1,}[a-zA-Z0-9_]*)'                     # Get member name
        r'(?P<iArr>\[(?P<aLen>.*?)?\])?'                            # Check if it's an array and get its length
        r';',                                                       # Anchor to instruction end
    glsl):                                                      #
        align:int = typeSize[rInfo['type']]                         # Get type alignment
        maxAlign = max(maxAlign, align)                             # Recalculate maximum alignment #TODO check if this actually works


        offset = roundUp(offset, align)                             # Recalculate element offset
        aLen:int = 0                                                # Get array length
        if rInfo['iArr'] == None:    aLen = None                        # None if the element is not an array
        elif len(rInfo['aLen']) > 0: aLen = eval(rInfo['aLen'])         # Evaulate the expression if it is

        size:int = align * aLen if aLen != None else align          # Calculate element size #FIXME add std140 support
        elms += [ns(**{                                             # Save element informations
            'type': typeName[rInfo['type']],                            # Get the type and translate it to C++
            'name': rInfo['name'],                                      # Get the name from regex
            'size': size,                                               # Save the size of the element
            'aLen': aLen,                                               # Save the array length
            'ofst': offset                                              # Save the calculated offset
        })]                                                        #
        offset += size                                             # Calculate raw offset for the next element #TODO check if this actually works


    # Return a dictionary containing the translated members, the external bindings and the padded structure size
    # The size of the structure must be a multiple of 16 #BUG THE NORMAL SIZE MAKES THE ENGINE CRASH (prob buffer overflow?)
    # return dict({ 'func' : ret, 'ext' : ext, 'size' : roundUp(offset, max(maxAlign, 16)) + 64})    #BUG ok
    # return dict({ 'func' : ret, 'ext' : ext, 'size' : roundUp(offset, max(maxAlign, 16))})         #BUG crash
    return ns(**{
        'elms' : elms,
        'size' : roundUp(offset, max(maxAlign, 256))
    })
    # return dict({ 'cpp' : cpp, 'types' : types, 'size' : roundUp(offset, max(maxAlign, 256)) })
    # !NVIDIA has a huge alignment of 256 bytes. #TODO use a different alignment based on the GPU, ig
    #FIXME use different alignment for storage buffers








# Layout parsing ----------------------------------------------------------------------------------------------------------------#








# Translates a single layout
def parseLayout(glsl:str) :
    rInfo = re.match(
        r'layout.*?\(std(?P<stdv>\d{3}).*?binding=(?P<indx>\d+)\)'
        r'(?P<type>buffer|uniform) (?P<iExt>ext_)?(?P<name>.*?)\{(?P<elms>.*?)\}',
        glsl
    )

    elmsInfo = parseElms(rInfo['elms'])
    return ns(**{
        'type': 'storage' if rInfo['type'] == 'buffer' else 'uniform',
        'stdv': int(rInfo['stdv']),
        'name': rInfo['name'],
        'iExt': rInfo['iExt'] != None and len(rInfo['iExt']) > 0,
        'indx': rInfo['indx'],
        'cstr': 'l_' + rInfo['name'],
        'elms': elmsInfo.elms,
        'size': elmsInfo.size
    })







def getLayouts(glsl:str):
    """!
        Finds any layout definition, parses them and translates them to C++ structs through the parseLayout function

        @type glsl: str
        @param glsl: The raw GLSL shader code

        @rtype: dict
        @return: A dictionary containing informations about the layouts
        ├─ layouts:list             A list of layouts
        │   ├─ type:str                 The type of the layout. 'storage' or 'uniform'
        │   ├─ stdv:int                 The standard used for the layout. 140 or 430
        │   ├─ name:str                 The name of the layout, without the external specifier
        │   ├─ iExt:bool                True if the binding is external, False otherwise
        │   ├─ indx:int                 The binding index
        │   ├─ cstr:str                 The name of the generated C++ structure
        │   ├─ elms:list                A list of elements
        │   │   ├─ type:str                 The corresponding C++ type of the element
        │   │   ├─ name:str                 The name of the element
        │   │   ├─ size:int                 The size of the element in bytes
        │   │   ├─ aLen:int                 The length of the array, or None if the element is not an array
        │   │   └─ ofst:int                 The offset of the element in the memory allocation, in bytes
        │   └─ size:int                 The size of the required allocation in bytes
        ├─ storageNum:int           The number of storage buffers used in the shader
        └─ uniformNum:int           The number of uniform buffers used in the shader
    """


    rInfo = re.findall(r'(layout\(.*?(buffer|uniform) (?:.*?)\{(?:.*?)\};)', glsl)

    layouts    : list = []                          # List of parsed layout infos
    storageNum : int  = 0                           # Number storage bindings in the shader
    uniformNum : int  = 0                           # Number of uniform bindings in the shader

    for i, rLayout in enumerate(rInfo):             # For each layout
        layouts += [ parseLayout(rLayout[0]) ]          # Translate declaration
        if rLayout[1] == 'uniform': uniformNum += 1     # Count buffer types
        else: storageNum += 1

    return ns(**{
        'layouts'   : layouts,
        'storageNum': storageNum,
        'uniformNum': uniformNum
    })








# File output ####################################################################################################################








def capitalize1(s:str):
    if len(s) > 1:
        return s[0].upper() + s[1:]
    else:
        return s.upper()




def fixTabs(s:str, indent:int = 0):
    ls = s.split('\n')
    for i, l in enumerate(ls):
        wsLen:int = 0
        while True:
            r = re.match(r'^((?:(?: {,3})\t)|(?: {4}))', l)
            if r == None: break
            l = l[len(r.group(1)):]
            wsLen += 1
        ls[i] = '\t' * (wsLen + indent * (len(l) > 0)) + l

    return '\n'.join(ls)




# Parses a GLSL compute shader and writes 2 C++ interface files
def parseShader(pathr:str, etop:str, isEngine:bool):
    ptoe:str = os.path.relpath('.', etop)

    #Check files
    if not os.path.exists(pathr):
        print("File does not exist")
        return 1


    #Get output shader path and name
    shOutPath : str = os.path.relpath(f'{ etop }/.engine/src/Generated/Shaders' if isEngine else './src/Generated/Shaders', '.')
    shOutName : str = os.path.basename(pathr).rsplit('.', maxsplit = 1)[0]
    shName    : str = shOutName.replace('.', '_')
    if re.match(r'[a-zA-Z_](\w|\.)*', shOutName) == None:
        print(f'Invalid shader name: "{ shOutName }". The name of a shader can only contain alphanumeric characters, periods or underscores and must start with a letter or an underscore')
        return 2



    # Expand macros
    code:str = subprocess.check_output(
        ['glslangValidator', f'{ os.path.relpath(pathr, ".") }', '-E'],
        text = True
    )
    # Parse out unnecessary whitespace and comments from the shader code #TODO move to module
    ncode:str = (
        re.sub(r'([-+])''\x07', r'\g<1> \g<1>',                         # Prevent - - and + + from being merged #! GLSL has no pointers. * * is a syntax error
        re.sub(r' ?([()\[\]{}+*-\/.!<>=&^|?:%,;])( )?',  r'\g<1>',      # Remove spaces near opeartors
        re.sub(r'([-+]) \1', r'\g<1>''\x07',                            # Prevent - - and + + from being merged
        re.sub(r'\\n',      r'\n',                                      # Remove newlines
        re.sub(r'\n',       r'',                                        # Remove newlines
        re.sub(r'(#.*?)\n', r'\g<1>\\n',                                # Remove newlines
        re.sub(r' +',       r' ',                                       # Remove whitespace
        code.expandtabs(4)                                              # Convert tabs to spaces
    ))))))))




    #Parse layouts
    pGlsl = getLayouts(ncode)

    layouts:list = []
    externs:list = []
    for l in pGlsl.layouts:
        if l.iExt: externs += [ l ]
        else:      layouts += [ l ]








    #Write output files
    with open(f'{ shOutPath }/{ shName }.gsi.hpp', 'w') as fh, open(f'{ shOutPath }/{ shName }.gsi.cpp', 'w') as fc:


        # Write header
        fh.write(fixTabs(
            f'\n//####################################################################################'
            f'\n// This file was generated by { "" if isEngine else ptoe + "/" }Tools/Build/GlslToCpp'
            f'\n// Changes could be overwritten without notice'
            f'\n//####################################################################################\n'
            f'\n#pragma once'                                                       # Include guard
            f'\n#include \"Lynx/Core/Render/Shaders/ShaderInterface_t.hpp\"'        # Base Shader struct
            f'\n#include "Lynx/Core/Init.hpp\"'                                 # Auto init
            f'\n'
            f'\n'
            f'\n'
            f'\n'
            f'\nnamespace lnx::shd::gsi{{'                                          # Write namespace and struct declaration
            f'\n    struct { shName } : public ShaderInterface_b {{'
            f'\n        static ShaderInterface_b::Layout layout;'
            f'\n        static uint32 pipelineIndex;'
        ))


        # Write cpp
        fc.write(fixTabs(
            f'\n//####################################################################################'
            f'\n// This file was generated by { "" if isEngine else ptoe + "/" }Tools/Build/GlslToCpp'
            f'\n// Changes could be overwritten without notice'
            f'\n//####################################################################################'
            f'\n#include "Generated/Shaders/{ shName }.gsi.hpp"'
            f'\n'
            f'\n#include "Lynx/Core/Init.hpp\"'                         # Auto init
            f'\n#include "Lynx/Core/Render/Shaders/Shader.hpp\"'            # Engine shader header
            f'\n#include "Lynx/Core/Render/Render.hpp\"'                    # Render core definition
            f'\n////#define LNX_H_{ shName.upper() }'                           # Auto init define
            f'\n'
            f'\n'
            f'\n'
            f'\nnamespace lnx::shd::gsi{{'                                  # Write namespace declaration
        ))




        # Write layout structs #FIXME move definition to .cpp
        fh.write('\n\n' + fixTabs(
            '\n\n'.join(((
            f'\nstruct { l.cstr } : public ShaderElm_b<e{ l.type.capitalize() }> {{'
            f'\n    alwaysInline { l.cstr }(const bool vExt) : ShaderElm_b() {{}}'    # External layout constructor
            f'\n    inline { l.cstr }() : ShaderElm_b({ l.size }) {{}}'               # Default constructor (Partial construction)
            # Copy constructor
            f'\n    inline { l.cstr }(const { l.cstr }& p{ capitalize1(l.name) }) {{'
            f'\n    	ShaderElm_b:: data = p{ capitalize1(l.name) }. data;'
            f'\n    	ShaderElm_b::vdata = p{ capitalize1(l.name) }.vdata;'
            f'\n    }}'
            # Copy assignment
            f'\n    inline { l.cstr }& operator=(const { l.cstr }& p{ capitalize1(l.name) }) {{'
            f'\n    	ShaderElm_b:: data = p{ capitalize1(l.name) }. data;'
            f'\n    	ShaderElm_b::vdata = p{ capitalize1(l.name) }.vdata;'
            f'\n    	return *this;'
            f'\n    	//FIXME automatically update render data after calling this function'
            f'\n    }}'
            # Construct with vdata only #FIXME write specific struct
            f'\n    inline { l.cstr }(const vram::ptr<auto, eVRam, e{ l.type.capitalize() }>& pVPtr){{' #FIXME check the length
            f'\n        vdata = (vram::ptr<char, eVRam, e{ l.type.capitalize() }>)pVPtr;'
            f'\n    }}' #TODO add operator= for different buffer types
            # Copy from vdata only #FIXME write specific struct
            f'\n    inline auto& operator=(const vram::ptr<auto, eVRam, e{ l.type.capitalize() }>& pVPtr){{' #FIXME check the length
            f'\n        vdata = (vram::ptr<char, eVRam, e{ l.type.capitalize() }>)pVPtr;'
            f'\n        return *this;'
            f'\n    }}') + #TODO add operator= for different buffer types
            # Member references
            f''     ''.join((
            f'\n    { m.type }& { m.name } = *({ m.type }*)(ShaderElm_b::data + { m.ofst });' + ('' if m.aLen == None else
            f'\n    uint64 { m.name }_tmp_size = { m.size };') +  #!^ offset includes the length of the arrays  #TODO use an actual array #FIXME use an array that automatically reallocates the whole block when resizing the unknown size array
            f''     ) for m in l.elms) +
            f'\n}};'
            f'\n{ l.cstr } { l.name }' + ('{ true }' if l.iExt else '') + ';'
            f'' ) for l in pGlsl.layouts),
        2))




        # Write the shader spawn functions
        fh.write('\n\n' + fixTabs(
            f'\nvoid spawn(' + (
            f''     ','.join((
            f'\n    const { e.cstr }& p{ capitalize1(e.name) }')for e in externs) + ','
            f'\n    const u32v3 vGroupCount, core::RenderCore& pRenderCore'
            f'\n);') +
            f'\nvoid createDescriptorSets();'
            f'\nvoid createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);'
            f'\nvoid updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore);'
            f'\nvoid destroy();',
        2))




        fc.write('\n\n' + fixTabs(
            f'\nvoid { shName }::spawn(' + (
            f''     ','.join((
            f'\n    const { e.cstr }& p{ capitalize1(e.name) }' )for e in externs) + ','
            f'\n    const u32v3 vGroupCount, core::RenderCore& pRenderCore'
            f'\n){{') +
            f'\n    pRenderCore.addObject_m.lock();' + (
            f''         ''.join((f'\n\t\t{ e.name } = p{ capitalize1(e.name) };') for e in externs)) +
            f'\n'
            f'\n        createDescriptorSets();'
            f'\n        createCommandBuffers(vGroupCount, pRenderCore);'
            f'\n        pRenderCore.swp.shadersCBs.add(commandBuffers[0]);'
            f'\n    pRenderCore.addObject_m.unlock();'
            f'\n}}',
        1))




        fc.write('\n\n' + fixTabs(
            f'\nvoid { shName }::createDescriptorSets(){{'
            f'\n    vk::DescriptorPoolSize sizes[2] = {{'
            f'\n        '   f'vk::DescriptorPoolSize().setType(vk::DescriptorType::eStorageBuffer).setDescriptorCount({ str(pGlsl.storageNum) }),' + (
            f'\n        ' + f'vk::DescriptorPoolSize().setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount({ str(pGlsl.uniformNum) })' if pGlsl.uniformNum > 0 else '{}' ) +
            f'\n    }};'
            f'\n    auto poolInfo = vk::DescriptorPoolCreateInfo()'
            f'\n        .setFlags         (vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet)'
            f'\n        .setMaxSets       (1)'
            f'\n        .setPoolSizeCount ({ str(2 if pGlsl.uniformNum > 0 else 1) })'
            f'\n        .setPPoolSizes    (sizes)'
            f'\n    ;'
            f'\n    switch(core::dvc::graphics.ld.createDescriptorPool(&poolInfo, nullptr, &descriptorPool)){{'
            f'\n        case vk::Result::eErrorFragmentationEXT:  dbg::logError(\"Fragmentation error\");  break;'
            f'\n        vkDefaultCases;'
            f'\n    }}'
            f'\n\n\n'
            f'\n    auto allocateSetInfo = vk::DescriptorSetAllocateInfo()'
            f'\n        .setDescriptorPool     (descriptorPool)'
            f'\n        .setDescriptorSetCount (1)'
            f'\n        .setPSetLayouts        (&{ shName }::layout.descriptorSetLayout)'
            f'\n    ;'
            f'\n    switch(core::dvc::graphics.ld.allocateDescriptorSets(&allocateSetInfo, &descriptorSet)){{'
            f'\n        case vk::Result::eErrorFragmentedPool:    dbg::logError(\"Fragmented pool\");      break;'
            f'\n        case vk::Result::eErrorOutOfPoolMemory:   dbg::logError(\"Out of pool memory\");   break;'
            f'\n        vkDefaultCases;'
            f'\n    }}'
            f'\n\n\n'
            f'\n    vk::WriteDescriptorSet writeSets[{ str(len(pGlsl.layouts)) }];' + (
            f''     '\n'.join((
            f'\n    auto bufferInfo{ str(i) } = vk::DescriptorBufferInfo()'
            f'\n        .setBuffer ({ l.name }.vdata.cell->csc.buffer)'
            f'\n        .setOffset ({ l.name }.vdata.cell->localOffset)'
            f'\n        .setRange  ({ l.name }.vdata.cell->cellSize)'
            f'\n    ;'
            f'\n    writeSets[{ str(i) }] = vk::WriteDescriptorSet()'
            f'\n        .setDstSet          (descriptorSet)'
            f'\n        .setDstBinding      ({ str(l.indx) })'
            f'\n        .setDescriptorCount (1)'
            f'\n        .setDescriptorType  (vk::DescriptorType::e{ l.type.capitalize() }Buffer)'
            f'\n        .setPBufferInfo     (&bufferInfo{ str(i) })'
            f'\n    ;'
            f''     ) for i, l in enumerate(pGlsl.layouts))) +
            f'\n\tcore::dvc::graphics.ld.updateDescriptorSets({ str(len(pGlsl.layouts)) }, writeSets, 0, nullptr);'
            f'\n}}',
        1))




        fc.write('\n' * 8 + fixTabs(
            f'\nvoid { shName }::createCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){{'
            f'\n    auto allocateCbInfo = vk::CommandBufferAllocateInfo()'
            f'\n        .setCommandPool        (pRenderCore.commandPool)'
            f'\n        .setLevel              (vk::CommandBufferLevel::ePrimary)'
            f'\n        .setCommandBufferCount (1)'
            f'\n    ;'
            f'\n    commandBuffers.resize(1);'
            f'\n    switch(core::dvc::graphics.ld.allocateCommandBuffers(&allocateCbInfo, commandBuffers.begin())){{ vkDefaultCases; }}'
            f'\n'
            f'\n    auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);'
            f'\n    switch(commandBuffers[0].begin(beginInfo)){{ vkDefaultCases; }}'
            f'\n    commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[{ shName }::pipelineIndex]);'
            f'\n    commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, { shName }::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);'
            f'\n    commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);'
            f'\n    switch(commandBuffers[0].end()){{ vkDefaultCases; }}'
            #TODO WRITE ALL COMMAND BUFFERS AT ONCE
            #TODO or use multiple descriptor sets for multiple objects, but in the same command buffer
            f'\n}}',
        1))




        fc.write('\n' * 8 + fixTabs(
            f'\nvoid { shName }::updateCommandBuffers(const u32v3 vGroupCount, core::RenderCore& pRenderCore){{'
            f'\n    auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);'
            f'\n    switch(commandBuffers[0].begin(beginInfo)){{ vkDefaultCases; }}'
            f'\n    commandBuffers[0].bindPipeline       (vk::PipelineBindPoint::eCompute, pRenderCore.pipelines[{ shName }::pipelineIndex]);'
            f'\n    commandBuffers[0].bindDescriptorSets (vk::PipelineBindPoint::eCompute, { shName }::layout.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);'
            f'\n    commandBuffers[0].dispatch           (vGroupCount.x, vGroupCount.y, vGroupCount.z);'
            f'\n    switch(commandBuffers[0].end()){{ vkDefaultCases; }}'
            f'\n}}',
        1))




        fc.write('\n' * 8 + fixTabs(
            f'\nvoid { shName }::destroy(){{'
            f'\n    //TODO'
            f'\n}}',
        1))




        fc.write('\n' * 8 + fixTabs(
            f'\nShaderInterface_b::Layout { shName }::layout;'
            f'\nuint32 { shName }::pipelineIndex = core::shaders::pipelineNum++;'
            f'\n_lnx_init_fun_(LNX_H_{ shName.upper() }){{'
            f'\n    core::shaders::pipelineLayouts.resize(core::shaders::pipelineNum);'
            f'\n    core::shaders::pipelineLayouts[{ shName }::pipelineIndex] = &{ shName }::layout;'
            f'\n    {{ //Create descriptor set layout'
            f'\n        vk::DescriptorSetLayoutBinding bindingLayouts[{ str(len(pGlsl.layouts)) }];' + (
            f''         '\n'.join((
            f'\n        bindingLayouts[{ str(i) }] = vk::DescriptorSetLayoutBinding()'
            f'\n            .setBinding            ({ str(l.indx) })'
            f'\n            .setDescriptorType     (vk::DescriptorType::e{ l.type.capitalize() }Buffer)'
            f'\n            .setDescriptorCount    (1)'
            f'\n            .setStageFlags         (vk::ShaderStageFlagBits::eCompute)'
            f'\n            .setPImmutableSamplers (nullptr)'
            f'\n        ;'
            f''         ) for i, l in enumerate(pGlsl.layouts))) +
            f'\n'
            f'\n        auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()'
            f'\n            .setBindingCount ({ str(len(pGlsl.layouts)) })'
            f'\n            .setPBindings    (bindingLayouts)'
            f'\n        ;'
            f'\n        //Create the descriptor set layout'
            f'\n        switch(core::dvc::graphics.ld.createDescriptorSetLayout(&layoutCreateInfo, nullptr, &{ shName }::layout.descriptorSetLayout)){{ vkDefaultCases; }}'
            f'\n    }}'
            f'\n'
            f'\n'
            f'\n'
            f'\n'
            f'\n    {{ //Create pipeline layout'
            f'\n        uint64 fileLength = 0;'
            f'\n        uint32* code = core::shaders::loadSpv(&fileLength, \"{ shOutPath }/{ shName }.spv\");'
            f'\n        { shName }::layout.shaderModule = core::shaders::createModule(core::dvc::graphics.ld, code, fileLength);'
            f'\n'
            f'\n        { shName }::layout.shaderStageCreateInfo = vk::PipelineShaderStageCreateInfo()'
            f'\n            .setStage  (vk::ShaderStageFlagBits::eCompute)'
            f'\n            .setModule ({ shName }::layout.shaderModule)'
            f'\n            .setPName  (\"main\")'
            f'\n        ;'
            f'\n'
            f'\n        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()'
            f'\n            .setSetLayoutCount (1)'
            f'\n            .setPSetLayouts    (&{ shName }::layout.descriptorSetLayout)'
            f'\n        ;'
            f'\n        switch(core::dvc::graphics.ld.createPipelineLayout(&pipelineLayoutCreateInfo, nullptr, &{ shName }::layout.pipelineLayout)){{ vkDefaultCases; }}'
            f'\n    }}'
            f'\n}}',
        1))

        #TODO ADD DESTROY FUNCTION (copy from shaders::destroyShader)

        fh.write(f'\n\t}};');         # }; //Struct }
        fh.write(f'\n\n\n\t_lnx_init_fun_dec(LNX_H_{ shName.upper() });')

        fh.write(f'\n}}');            # } //Namespace
        fc.write(f'\n}}');            # } //Namespace
    #TODO STRUCTURES HAVE A MINIMUM ALIGNMENT OF 16

    return 0







if len(sys.argv) != 4: sys.exit('GlslToCpp: Wrong number of arguments')
sys.exit(parseShader(sys.argv[1], sys.argv[2], sys.argv[3] == 'e'))