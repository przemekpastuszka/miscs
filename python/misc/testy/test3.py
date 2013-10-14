import sys
import urllib.request
from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Form(QDialog):
    def __init__(self, parent=None):
        super(Form, self).__init__(parent)
        
        date = self.getdata()
        rates = sorted(self.rates.keys())
        
        date_label = QLabel(date)
        self.from_combo_box = QComboBox()
        self.from_combo_box.addItems(rates)
        self.from_spin_box = QDoubleSpinBox()
        self.from_spin_box.setRange(0.01, 100000000.00)
        self.from_spin_box.setValue(1.00)
        self.to_combo_box = QComboBox()
        self.to_combo_box.addItems(rates)
        self.to_label = QLabel("1.00")
        
        grid = QGridLayout()
        grid.addWidget(date_label, 0, 0)
        grid.addWidget(self.from_combo_box, 1, 0)
        grid.addWidget(self.from_spin_box, 1, 1)
        grid.addWidget(self.to_combo_box, 2, 0)
        grid.addWidget(self.to_label, 2, 1)
        self.setLayout(grid)
        
        self.connect(self.from_combo_box, SIGNAL("currentIndexChanged(int)"), self.updateUi)
        self.connect(self.to_combo_box, SIGNAL("currentIndexChanged(int)"), self.updateUi)
        self.connect(self.from_spin_box, SIGNAL("valueChanged(double)"), self.updateUi)
        self.setWindowTitle("Currency")
        
    def updateUi(self):
        to = self.to_combo_box.currentText()
        from_ = self.from_combo_box.currentText()
        result = (self.rates[from_] / self.rates[to]) * self.from_spin_box.value()
        self.to_label.setText(str(result))
        
    def getdata(self):
        self.rates = {}
        try:
            date = "unknown"
            fh = urllib.request.urlopen("http://www.bankofcanada.ca/en/markets/csv/exchange_eng.csv")
            
            for line in fh:
                line = line.decode("utf-8")
                if not line or line.startswith(("#", "Closing ")):
                    continue
                fields = line.split(",")
                if line.startswith("Date "):
                    date = fields[-1]
                else:
                    try:
                        value = float(fields[-1])
                        self.rates[fields[0]] = value
                    except ValueError:
                        pass
            return date
        except:
           return "failed"
            
app = QApplication(sys.argv)
form = Form()
form.show()
app.exec_()
                    
