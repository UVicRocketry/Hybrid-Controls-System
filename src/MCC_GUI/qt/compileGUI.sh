#!/bin/bash
cd src/MCC_GUI/qt
rm main.py
rm PortSel.py
rm confirm.py
rm debug.py
python3 -m PyQt5.uic.pyuic PortSel.ui -o PortSel.py
python3 -m PyQt5.uic.pyuic main.ui -o main.py
python3 -m PyQt5.uic.pyuic debug.ui -o debug.py
python3 -m PyQt5.uic.pyuic confirm.ui -o confirm.py
