#!/usr/bin/env python
import numpy as np
import cv2 as cv

cap = cv.VideoCapture("vtest.avi")

cap.release()
cv.destroyAllWindows()
