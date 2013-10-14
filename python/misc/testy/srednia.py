import sys
import pickle
from PyQt4.QtCore import *
from PyQt4.QtGui import *


def clean_value(value):
    try:
        return float(eval(value))
    except:
        return 0


class Form(QDialog):
    def __init__(self, file_name='dane'):
        super().__init__(None)

        self.file_name = file_name
        self.result = QLabel("Średnia ważona: 0")
        self.add_button = QPushButton("Dodaj przedmiot")
        self.save_button = QPushButton("Zapisz")

        self.elements_layout = QGridLayout()
        self.elements_layout.addWidget(self.result, 0, 0)
        self.elements_layout.addWidget(self.add_button, 1, 0)
        self.elements_layout.addWidget(self.save_button, 1, 1)
        for i, label in enumerate(["Nazwa", "Średnia", "ECTS", "Mnożnik"]):
            self.elements_layout.addWidget(QLabel(label), 2, i)
        self.setLayout(self.elements_layout)
        self.setWindowTitle("Plik do odczytu/zapisu: " + file_name)

        self.connect(self.add_button, SIGNAL("clicked()"), self.add_subject)
        self.connect(self.save_button, SIGNAL("clicked()"), self.save_to_file)

        self.subjects = []
        try:
            with open(self.file_name, 'rb') as data_file:
                for element in sorted(pickle.load(data_file)):
                    self.add_subject(element)
        except:
            pass

    def save_to_file(self):
        with open(self.file_name, 'wb') as data_file:
            elements = [[x.text() for x in elem] for elem in self.subjects]
            pickle.dump(elements, data_file)

    def add_subject(self, values=["Przedmiot", "5.0", "6", "1"]):
        row = self.elements_layout.rowCount() + 1
        widgets = [QLineEdit(name) for name in values]
        for i, x in enumerate(widgets):
            x.selectAll()
            self.elements_layout.addWidget(x, row, i)
            self.connect(x, SIGNAL("textChanged(QString)"), self.calculate_av)
        del_button = QPushButton("Usuń")
        self.elements_layout.addWidget(del_button, row, 4)
        self.connect(del_button, SIGNAL("clicked()"),
            lambda: self.del_subject(widgets, del_button))
        widgets[0].setFocus()
        self.subjects.append(widgets)
        self.calculate_av()

    def del_subject(self, widgets, del_button):
        for widget in widgets:
            widget.hide()
        del_button.hide()
        del self.subjects[self.subjects.index(widgets)]
        self.calculate_av()
        self.adjustSize()

    def calculate_av(self):
        numerator = 0
        denominator = 0
        for name, average, ects, mul in self.subjects:
            average = clean_value(average.text())
            ects = clean_value(ects.text())
            mul = clean_value(mul.text())
            numerator += average * ects * mul
            denominator += ects * mul
        result = numerator / denominator if denominator else 0
        self.result.setText("Średnia ważona: {0:.3f}".format(result))

app = QApplication(sys.argv)
if len(sys.argv) >= 2:
    form = Form(sys.argv[1])
else:
    form = Form()
form.show()
app.exec_()
