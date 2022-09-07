import sys
from PySide2 import QtCore, QtWidgets, QtGui
from qt_material import apply_stylesheet
import PyVenom

import time


class Viewport3D(QtWidgets.QDockWidget):
    def __init__(self, graphicsApp, title="Viewport", parent=None):
        super().__init__(title, parent)

        self.titlebarHeight = self.style().pixelMetric(
            QtWidgets.QStyle.PM_TitleBarHeight) * 1.2
        self.drawRegion = QtWidgets.QLabel(self)

        # Ogre3D app
        self.graphicsApp = graphicsApp
        self.graphicsApp.init(self.drawRegion.winId())

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.renderLoop)

        self.currentTime = time.time()

        self.setStyleSheet("QDockWidget"
                           "{"
                           "background : rgb(50, 50, 90);"
                           "}"
                           "QDockWidget::title"
                           "{"
                           "background : rgb(50, 50, 90);"
                           "}"
                           "QDockWidget QPushButton"
                           "{"
                           "border : 2px solid black;"
                           "background : darkgreen;"
                           "}"
                           )

    def resizeEvent(self, event: QtGui.QResizeEvent):
        self.graphicsApp.resize(event.size().width(), event.size().height())
        return super().resizeEvent(event)

    def startRendering(self):
        self.timer.start(1000/90)

    def renderLoop(self):
        self.drawRegion.setGeometry(0, self.titlebarHeight, self.frameSize().width(),
                                    self.frameSize().height() - self.titlebarHeight)

        self.graphicsApp.renderOneFrame()

        # Calculating FPS
        deltaTime = time.time() - self.currentTime
        self.currentTime = time.time()
        print("FPS: ", 1/deltaTime)


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Venom Graphics")
        self.resize(1280, 720)

        # self.centralWidget = QtWidgets.QCalendarWidget()
        # self.setCentralWidget(self.centralWidget)

        layout = QtWidgets.QHBoxLayout()
        bar = self.menuBar()
        file = bar.addMenu('File')
        file.addAction('New')
        file.addAction('Save')
        file.addAction('quit')

        # Ogre graphics app instance
        self.graphicsApp = PyVenom.PythonAppHandler()

        # Viewport3D
        self.viewport = Viewport3D(self.graphicsApp, parent=self)
        self.viewport.setFloating(False)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.viewport)
        # self.setCentralWidget(self.viewport)

        # Heirachy panel
        self.heirarchyPanel = QtWidgets.QDockWidget("Heirarchy", parent=self)
        self.heirarchyPanel.setFloating(False)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea, self.heirarchyPanel)

        # Inspector panel
        self.inspectorPanel = QtWidgets.QDockWidget("Inspector", parent=self)
        self.inspectorPanel.setFloating(False)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.inspectorPanel)

        self.setLayout(layout)

        self.viewport.startRendering()


if __name__ == '__main__':
    # Staring Qt app
    app = QtWidgets.QApplication(sys.argv)
    mainWin = MainWindow()

    # setup stylesheet
    apply_stylesheet(app, theme='dark_red.xml')

    mainWin.show()
    res = app.exec_()
    sys.exit(res)
