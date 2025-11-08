import os
import shutil
import subprocess
from pathlib import Path
import time

# Var
project_name = "RobotHunter"
dir_int = "Intermediate"
dir_bin = "Binaries"
dir_vs = ".vs"
dir_log = "CustomLog"
dir_data_cache = "DerivedDataCache"
dir_saved = "Saved"
visual_exe = "devenv.exe"
rider_exe = "rider64.exe"
unreal_exe = "UnrealEditor.exe"

# Get root project path
script_path = os.path.dirname(os.path.abspath(__file__))
index = script_path.rfind(project_name)
root_path = script_path[:index + len(project_name)]

# shutdown project.vs if running 
try:
    processus = subprocess.check_output(["tasklist"], text=True)
    if visual_exe in processus:
        subprocess.run(["taskkill", "/F", "/IM", visual_exe], check=False)
    if rider_exe in processus:
        subprocess.run(["taskkill", "/F", "/IM", rider_exe], check=False)
except Exception as e:
    print("Error in checking processus")
    os._exit(1)

# Wait until project .uproject and .vs finish to kill
try:
    while True:
        processus = subprocess.check_output(["tasklist"], text=True)
        if (visual_exe or rider_exe) and unreal_exe not in processus:
            break
        time.sleep(0.2)
except Exception as e:
    print("Error in checking processus")
    os._exit(1)

# Remove .sln and .vsconfig if exist
sln_path = Path(os.path.join(root_path, project_name + ".sln"))
if Path.is_file(sln_path):
    os.remove(sln_path)
config_path = Path(os.path.join(root_path, ".vsconfig"))
if Path.is_file(config_path):
    os.remove(config_path)

# Remove folder if exist
intermediate_path = Path(os.path.join(root_path, dir_int))
if Path.is_dir(intermediate_path) :
    shutil.rmtree(intermediate_path)
binaries_path = Path(os.path.join(root_path, dir_bin))
if Path.is_dir(binaries_path) :
    shutil.rmtree(binaries_path)    
vs_path = Path(os.path.join(root_path, dir_vs))
if Path.is_dir(vs_path) :
    shutil.rmtree(vs_path)    
log_path = Path(os.path.join(root_path, dir_log))
if Path.is_dir(log_path) :
    shutil.rmtree(log_path)    
data_cache_path = Path(os.path.join(root_path, dir_data_cache))
if Path.is_dir(data_cache_path) :
    shutil.rmtree(data_cache_path)    
saved_path = Path(os.path.join(root_path, dir_saved))
if Path.is_dir(saved_path) :
    shutil.rmtree(saved_path)

# Launch project
project_path = os.path.join(root_path, project_name + ".uproject")
os.startfile(project_path)
