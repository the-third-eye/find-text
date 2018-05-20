#!/usr/bin/env python

import numpy as np
import cv2 as cv

if __name__ == '__main__':
    image = cv.imread('../../photos/signs.jpg')
    fgbg = cv.createBackgroundSubtractorMOG2() 
    fgmask = fgbg.apply(image)
    cv.imshow('mask', fgmask)
    cv.waitKey(0)
    cv.destroyAllWindows()
