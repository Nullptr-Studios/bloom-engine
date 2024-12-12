import shutil
import os

# textures
texDir = './resources/textures'
outTexDir = './bin/debug/sandbox/resources/textures'

files = os.listdir(texDir)
shutil.copytree(texDir, outTexDir)
print ("Textures copied successfully!")
