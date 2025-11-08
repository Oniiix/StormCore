import sys
import subprocess

REQUIRED_MODULES = [
    "re", "os",
    "PySide6.QtWidgets", "PySide6.QtCore"
]

def install_and_import(package):
    try:
        __import__(package)
    except ImportError:
        print(f"Le module {package} n'est pas installé. Installation en cours...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", package.split(".")[0]])

for module in REQUIRED_MODULES:
    try:
        install_and_import(module.split('.')[0])
    except Exception as e:
        print(f"Erreur lors de l'installation du module {module}: {e}")

# --- ici commence ton code comme tu l'avais ---
import re
import os
from PySide6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QComboBox, QFormLayout,
    QLineEdit, QLabel, QPushButton, QMessageBox, QHBoxLayout,
    QScrollArea, QFrame
)
from PySide6.QtCore import Qt

def parse_header(path):
    structs = {}
    current_struct = None
    inside_struct = False
    with open(path, 'r', encoding='utf-8') as file:
        lines = list(file)
        i = 0
        while i < len(lines):
            line = lines[i].strip()
            if line.startswith('USTRUCT'):
                while i < len(lines) and not re.search(r'struct\s+\w+', lines[i]):
                    i += 1
                if i < len(lines):
                    struct_line = lines[i]
                    struct_match = re.search(r'struct\s+(\w+)', struct_line)
                    if struct_match:
                        current_struct = struct_match.group(1)
                        structs[current_struct] = []
                        inside_struct = True
                while i < len(lines) and '{' not in lines[i]:
                    i += 1
            elif inside_struct and 'UPROPERTY' in line:
                i += 1
                while i < len(lines):
                    var_line = lines[i].strip()
                    if var_line and not var_line.startswith('//') and not var_line.startswith('/*') and ';' in var_line:
                        match_var = re.match(
                            r'((?:[\w:<>]+)\s+)+(\w+)\s*(?:=\s*([^;]+))?;', var_line)
                        if match_var:
                            var_type = match_var.group(1).strip()
                            var_name = match_var.group(2).strip()
                            default_value = match_var.group(3).strip() if match_var.group(3) else ""
                            structs[current_struct].append({
                                "type": var_type,
                                "name": var_name,
                                "default": default_value
                            })
                        break
                    i += 1
            elif inside_struct and line.startswith('};'):
                inside_struct = False
                current_struct = None
            i += 1
    return structs

def find_project_root(start_path_):
    current = os.path.abspath(start_path_)
    while True:
        if any(fname.endswith('.uproject') for fname in os.listdir(current)):
            return current
        parent = os.path.dirname(current)
        if parent == current:
            raise FileNotFoundError("Aucun fichier .uproject trouvé en partant de " + start_path_)
        current = parent

class StructEditor(QWidget):
    def __init__(self, structs, header_path):
        super().__init__()
        self.setWindowTitle("Éditeur de struct Unreal (.h)")
        self.structs = structs
        self.header_path = header_path
        self.init_ui()
    def init_ui(self):
        layout = QVBoxLayout()
        self.combo_struct = QComboBox()
        self.combo_struct.addItems(self.structs.keys())
        self.combo_struct.currentTextChanged.connect(self.show_struct)
        layout.addWidget(QLabel("Struct à éditer"))
        layout.addWidget(self.combo_struct)
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.form_widget = QWidget()
        self.form_layout = QFormLayout()
        self.form_widget.setLayout(self.form_layout)
        self.scroll_area.setWidget(self.form_widget)
        layout.addWidget(self.scroll_area, 1)
        button_layout = QHBoxLayout()
        self.btn_accept = QPushButton("Accepter les modifications")
        self.btn_accept.clicked.connect(self.save_struct)
        button_layout.addStretch()
        button_layout.addWidget(self.btn_accept)
        layout.addLayout(button_layout)
        self.setLayout(layout)
        self.show_struct(self.combo_struct.currentText())
    def show_struct(self, struct_name):
        for i in reversed(range(self.form_layout.count())):
            widget = self.form_layout.itemAt(i).widget()
            if widget is not None:
                widget.deleteLater()
        self.edit_lines = []
        for var in self.structs[struct_name]:
            label = f'{var["type"]} {var["name"]}'
            line = QLineEdit(var["default"])
            self.form_layout.addRow(QLabel(label), line)
            self.edit_lines.append(line)
    def save_struct(self):
        struct_name = self.combo_struct.currentText()
        new_values = [line.text() for line in self.edit_lines]
        for i, var in enumerate(self.structs[struct_name]):
            var["default"] = new_values[i]
        try:
            apply_modifications_on_file(self.header_path, self.structs)
            QMessageBox.information(self, "Succès", "Fichier modifié avec succès.")
        except Exception as e:
            QMessageBox.critical(self, "Erreur", str(e))

def apply_modifications_on_file(header_path, structs):
    with open(header_path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    current_struct = None
    inside_struct = False
    struct_vars_idx = 0
    for i, line in enumerate(lines):
        stripped = line.strip()
        struct_match = re.search(r'struct\s+(\w+)', stripped)
        if struct_match:
            sname = struct_match.group(1)
            if sname in structs:
                current_struct = sname
                inside_struct = True
                struct_vars_idx = 0
        if inside_struct and "};" in stripped:
            inside_struct = False
            current_struct = None
            struct_vars_idx = 0
        if inside_struct and "UPROPERTY" in line:
            next_idx = i + 1
            while next_idx < len(lines):
                var_line = lines[next_idx].strip()
                found = False
                if var_line and not var_line.startswith('//') and not var_line.startswith('/*') and ';' in var_line:
                    match_var = re.match(
                        r'((?:[\w:<>]+)\s+)+(\w+)\s*(?:=\s*([^;]+))?;', var_line)
                    if match_var and current_struct and struct_vars_idx < len(structs[current_struct]):
                        var_default = structs[current_struct][struct_vars_idx]["default"]
                        if var_default.strip() != "":
                            new_var_line = re.sub(
                                r'=(.*?);', f'= {var_default};', var_line)
                            if '=' not in var_line:
                                new_var_line = var_line.replace(
                                    ';', f' = {var_default};')
                            lines[next_idx] = lines[next_idx].replace(var_line, new_var_line)
                        else:
                            lines[next_idx] = re.sub(r'=\s*[^;]+;', ';', lines[next_idx])
                        struct_vars_idx += 1
                        found = True
                        break
                next_idx += 1
    with open(header_path, "w", encoding="utf-8") as f:
        f.writelines(lines)

if __name__ == "__main__":
    start_path = os.path.dirname(__file__)
    try:
        project_root = find_project_root(start_path)
        uprojects = [f for f in os.listdir(project_root) if f.endswith('.uproject')]
        if not uprojects:
            raise Exception("Aucun .uproject trouvé !")
        if len(uprojects) > 1:
            raise Exception("Plusieurs .uproject trouvés, ajuste le script ou précise le projet.")
        project_name = os.path.splitext(uprojects[0])[0]
        header_path = os.path.join(project_root, "Source", project_name, "SaveSystem", "SettingsSave.h")
        if not os.path.isfile(header_path):
            raise Exception("Le header n'existe pas.")
        structs = parse_header(header_path)
        if not structs:
            raise Exception("Aucune struct ou variable trouvée dans le header.")

        # ==== Correction QApplication ====
        app = QApplication.instance()
        if app is None:
            app = QApplication(sys.argv)
        ed = StructEditor(structs, header_path)
        ed.resize(520, 600)
        ed.show()
        sys.exit(app.exec())
    except Exception as e:
        print("Erreur:", e)
