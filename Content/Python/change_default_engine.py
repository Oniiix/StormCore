from PySide6.QtWidgets import *
from PySide6.QtCore import *
import os
import unreal 



class change_default_engine_interface(QWidget):

    def __init__(self):
        super().__init__()
        self.config_path = unreal.Paths.project_config_dir() + "DefaultEngine.ini"
        self.draw_ui()
        self.bind_ui()


    def draw_ui(self):
        self.layout = QHBoxLayout(self)
        self.setFixedSize(300, 100)
        self.setWindowTitle("Change Default Engine Window")

        self.null_btn = QPushButton("Null")
        self.steam_btn = QPushButton("Steam")

        self.layout.addWidget(self.null_btn)
        self.layout.addWidget(self.steam_btn)


    def bind_ui(self):
        self.null_btn.clicked.connect(self.change_to_null)
        self.steam_btn.clicked.connect(self.change_to_steam)


    def change_to_steam(self):
        ini = open(self.config_path, "r")
        content = ini.read()
        content = content.replace("DefaultPlatformService=NULL", "DefaultPlatformService=Steam")
        content = content.replace("bEnabled=false", "bEnabled=true")
        ini = open(self.config_path, "w")
        ini.write(content)

    def change_to_null(self):
        ini = open(self.config_path, "r")
        content = ini.read()
        content = content.replace("DefaultPlatformService=Steam", "DefaultPlatformService=NULL")
        content = content.replace("bEnabled=true", "bEnabled=false")
        ini = open(self.config_path, "w")
        ini.write(content)







        


main = None
if not QApplication.instance():
    main = QApplication()
else :
    main = QApplication.instance()


app = change_default_engine_interface()
app.show()
