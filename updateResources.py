import shutil
import os

# textures
texDir = './resources/textures'
outTexDir = './bin/debug/wanderer/resources/textures'

files = os.listdir(texDir)
shutil.copytree(texDir, outTexDir)
print ("Textures copied successfully!")
