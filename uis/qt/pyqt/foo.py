#!/usr/bin/python
# -*- coding: utf-8 -*-

from sys import argv
from PyQt4 import QtGui
from PyQt4 import QtCore

from xmlrpclib import ServerProxy

class GUI(QtGui.QWidget):
    def __init__(self):
        super(GUI, self).__init__()
        self.xmlrpc = None
        self.initUI()


    def initUI(self):
        self.setWindowTitle('rfm12')
        self.button = QtGui.QPushButton('Set Host', self)
        self.button.move(20, 20)
        self.connect(self.button, QtCore.SIGNAL('clicked()'), self.showDialog)


    def showDialog(self):
        text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Set host:')
        if not ok:
            return False

        try:
            self.xmlrpc = ServerProxy(str(text))
            self.devs = self.xmlrpc.get_config()
        except:
            msg_box = QtGui.QMessageBox(
            QtGui.QMessageBox.Warning,
            'Warning',
            'Fetching data from host failed',
            QtGui.QMessageBox.Ok,
            self)
            msg_box.exec_()
            return False

        self.button.hide()
        i = 0
        for dev in self.devs:
            print "fetched config for device: %i, %s (%s)" % (i, dev['id'], dev['label'])
            self.devs[i]['_'] = QtGui.QCheckBox(dev['label'], self)
            self.devs[i]['_'].setObjectName(str(i))
            self.devs[i]['_'].move(10, i*20+10)
            self.connect(self.devs[i]['_'], QtCore.SIGNAL('stateChanged(int)'), self.switch)
            if dev['state'] == 1:
                self.devs[i]['_'].setChecked(True)
            self.devs[i]['_'].show()
            i += 1


    def switch(self, value):
        if value:
            self.xmlrpc.control(
                self.devs[int(self.sender().objectName())]['id'],
                1
            )
        else:
            self.xmlrpc.control(
                self.devs[int(self.sender().objectName())]['id'],
                0
            )


if __name__ == '__main__':
    app = QtGui.QApplication(argv)
    _ = GUI()
    _.show()
    app.exec_()
