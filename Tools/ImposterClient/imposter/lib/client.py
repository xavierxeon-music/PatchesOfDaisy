#!/usr/bin?env python3

import sys
import signal
import json

# pylint: disable=no-name-in-module
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer, QObject, QStandardPaths
from PySide6.QtNetwork import QLocalSocket
# pylint: enable=no-name-in-module


class Client(QObject):

    def __init__(self):

        QObject.__init__(self)

        self._socket = QLocalSocket()
        self._socket.readyRead.connect(self._readyRead)
        self._socket.errorOccurred.connect(self._socketError)

        self._serverFile = QStandardPaths.writableLocation(QStandardPaths.TempLocation) + '/ImposterDaisyPatch.server'
        print('use imposter server @ ', self._serverFile)

        self._app = QApplication([])

        signal.signal(signal.SIGINT, Client._terminate)
        signal.signal(signal.SIGTERM, Client._terminate)
        timer = QTimer()
        timer.start(500)
        timer.timeout.connect(lambda: sys.stdout.flush())  # Let the interpreter run each 500 ms.

    def _run(self):

        self._tryConnect()

        exitCode = self._app.exec()
        print()
        sys.exit(exitCode)

    @staticmethod
    def _terminate(*args):

        QApplication.quit()

    def _tryConnect(self):

        if self._socket.state() == QLocalSocket.ConnectingState:
            return
        if self._socket.state() == QLocalSocket.ConnectedState:
            return

        self._socket.connectToServer(self._serverFile)

    def _readyRead(self):

        inData = self._socket.readAll()
        if not inData:
            return

        inData = bytes(inData).decode()
        requestDict = dict()

        try:
            requestDict = json.loads(inData)
        except json.decoder.JSONDecodeError:
            pass

        if not 'request' in requestDict:
            responseDict = {'reply': 'NoRequest'}
        else:
            responseDict = self._processRequest(requestDict)

        outData = json.dumps(responseDict, indent=0)
        outData = outData.encode()
        self._socket.write(outData)

    def _processRequest(self, requestDict):

        return {'reply': 'UnkownRequest'}

    def _socketError(self, error):

        # print(error)

        reconnectDict = {
            QLocalSocket.ServerNotFoundError: 1000,
            QLocalSocket.PeerClosedError: 1000
        }
        if error in reconnectDict:
            QTimer.singleShot(reconnectDict[error], self._tryConnect)
