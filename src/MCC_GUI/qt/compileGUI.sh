#!/bin/bash
rm main.py
rm VCPort.py
python3 -m PyQt5.uic.pyuic VCPort.ui -o VCPort.py
python3 -m PyQt5.uic.pyuic main.ui -o main.py