import sys
from PySide2.QtCore import *
from PySide2.QtWidgets import *
from PySide2.QtGui import *

import qdarktheme
import PyVenom

import time


class VenomDockWidget(QDockWidget):
    def __init__(self, title="Dock Widget", parent=None):
        super().__init__(title, parent)

        self.setMinimumHeight(200)
        self.setMinimumWidth(200)

        # self.setStyleSheet("QDockWidget"
        #                    "{"
        #                    "background : rgb(50, 50, 90);"
        #                    "}"
        #                    "QDockWidget::title"
        #                    "{"
        #                    "background : rgb(50, 50, 90);"
        #                    "}"
        #                    "QDockWidget QPushButton"
        #                    "{"
        #                    "border : 2px solid black;"
        #                    "background : darkgreen;"
        #                    "}"
        #                    )


class Viewport3D(QLabel):
    def __init__(self, graphicsApp, title="Viewport", parent=None):
        super().__init__(title, parent)

        self.titlebarHeight = 0
        # self.titlebarHeight = self.style().pixelMetric(
        #     QStyle.PM_TitleBarHeight) * 1.2

        self.drawRegion = QLabel(self)

        # Ogre3D app
        self.graphicsApp = graphicsApp
        self.graphicsApp.init(self.drawRegion.winId())

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.renderLoop)

        self.currentTime = time.time()

    def resizeEvent(self, event: QResizeEvent):
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


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Venom Graphics")
        self.resize(1280, 720)

        # Setting theme and attributes
        self.setStyleSheet(qdarktheme.load_stylesheet())

        self.statusBar = QStatusBar()
        self.setStatusBar(self.statusBar)

        # Central widget is viewport now...
        # self.centralWidget = QCalendarWidget()
        # self.setCentralWidget(self.centralWidget)

        layout = QHBoxLayout()
        bar = self.menuBar()
        file = bar.addMenu('File')
        file.addAction('New')
        file.addAction('Save')
        file.addAction('quit')

        # Ogre graphics app instance
        self.graphicsApp = PyVenom.PythonAppHandler()

        # Viewport3D
        self.viewport = Viewport3D(self.graphicsApp, parent=self)
        # self.viewport.setFloating(False)
        # self.addDockWidget(Qt.RightDockWidgetArea, self.viewport)
        self.setCentralWidget(self.viewport)

        # Heirachy panel
        self.heirarchyPanel = VenomDockWidget("Heirarchy", parent=self)
        self.heirarchyPanel.setFloating(False)
        self.addDockWidget(Qt.LeftDockWidgetArea, self.heirarchyPanel)

        # Inspector panel
        self.inspectorPanel = VenomDockWidget("Inspector", parent=self)
        self.inspectorPanel.setFloating(False)
        self.addDockWidget(Qt.RightDockWidgetArea, self.inspectorPanel)

        self.setLayout(layout)

        self.viewport.startRendering()


if __name__ == '__main__':
    # Staring Qt app
    app = QApplication(sys.argv)
    mainWin = MainWindow()

    # Anti-aliasing
    app.setAttribute(Qt.ApplicationAttribute.AA_UseHighDpiPixmaps)

    mainWin.show()
    res = app.exec_()
    sys.exit(res)
