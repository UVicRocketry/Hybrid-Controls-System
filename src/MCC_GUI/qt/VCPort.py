# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'VCPort.ui'
#
# Created by: PyQt5 UI code generator 5.15.8
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(231, 122)
        self.b_VCPort = QtWidgets.QDialogButtonBox(Dialog)
        self.b_VCPort.setGeometry(QtCore.QRect(30, 80, 191, 32))
        self.b_VCPort.setOrientation(QtCore.Qt.Horizontal)
        self.b_VCPort.setStandardButtons(QtWidgets.QDialogButtonBox.Ok)
        self.b_VCPort.setObjectName("b_VCPort")
        self.t_VCPort = QtWidgets.QLineEdit(Dialog)
        self.t_VCPort.setGeometry(QtCore.QRect(20, 40, 191, 21))
        self.t_VCPort.setObjectName("t_VCPort")
        self.l_VCPort = QtWidgets.QLabel(Dialog)
        self.l_VCPort.setGeometry(QtCore.QRect(20, 10, 171, 31))
        self.l_VCPort.setObjectName("l_VCPort")

        self.retranslateUi(Dialog)
        self.b_VCPort.accepted.connect(Dialog.accept) # type: ignore
        self.b_VCPort.rejected.connect(Dialog.reject) # type: ignore
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Valve Cart Connection"))
        self.l_VCPort.setText(_translate("Dialog", "Enter Valve Cart Serial Port"))