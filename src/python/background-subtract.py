#!/usr/bin/env python

import numpy as np
import cv2 as cv
import sys

if __name__ == '__main__':
    if len(sys.argv) == 1:
        sys.exit('error: no input file!')
    
    image = None
    for i in range(1, len(sys.argv)):
        image = cv.imread(sys.argv[i])
        if image is not None:
            break

    if image is None:
        sys.exit('error: no valid file!')
    
    fgbg = cv.createBackgroundSubtractorMOG2()
    # convert to grayscale
    gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)
    # convert to binary
    binary = cv.adaptiveThreshold(gray,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv.THRESH_BINARY,11,2)
    # find foreground mask
    fgmask = fgbg.apply(gray)
    # apply foreground mask
    foreground = cv.bitwise_and(image,image,mask=fgmask)
    
    cv.imshow('original image', image)
    cv.imshow('binary image', binary)
    cv.imshow('foreground mask', fgmask)
    cv.imshow('foreground', foreground)
    
    cv.waitKey(0)
