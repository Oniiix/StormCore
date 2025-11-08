import os

project_name = "RobotHunter"

# Get project path
script_path = os.path.dirname(os.path.abspath(__file__))
index = script_path.rfind(project_name)
project_path = script_path[:index + len(project_name)]
print(project_path)

# Launch project
project_path = os.path.join(project_path, project_name + ".uproject")
os.startfile(project_path)
