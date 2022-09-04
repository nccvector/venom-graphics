import cv2
import numpy as np
import pymugen

app = pymugen.PythonAppHandler()

key = cv2.waitKey(1)
while not key == ord('q'):
    frame = app.getNextFrame()
    frame = np.swapaxes(frame, 0, 1)
    # frame = frame[:, :, ::-1]
    # frame = frame.T

    cv2.imshow('image', frame)
    key = cv2.waitKey(0)

cv2.destroyAllWindows()
