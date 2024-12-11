import os

shaderdir = "./resources/shaders"
outdir = "./bin/debug/sandbox/resources/shaders"

shaders = os.listdir(shaderdir)
shaders = [shaderdir + '/' + shader for shader in shaders if shader.endswith('.frag') or shader.endswith('.vert')]

if not os.path.exists(outdir):
    print (f"Output directory doesn't exist, creating {outdir}...")
    os.makedirs(outdir)

for s in shaders:
    print (f"Compiling {s}...")
    os.system(f"glslc {s} -o {outdir}/{os.path.basename(s)}.spv")

print ("Shaders compiled successfully!")