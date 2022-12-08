#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from .binread import *


def readspe(fn):
    """
    Read data in SPE file
    Returns wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm
    """
    if os.path.splitext(fn)[1][1:].lower() == 'spe':
        with open(fn, 'rb') as f:
            exp_sec = read_float(f, 10)
            xdim = read_WORD(f, 42)
            SpecCenterWlNm = read_float(f, 72)
            datatype = read_WORD(f, 108)
            ydim = read_WORD(f, 656)
            lavgexp = read_LONG(f, 668)
            numFrames = read_WORD(f, 1446)
            coef = np.zeros(6)
            for i in np.arange(6):
                coef[i] = read_double(f, 3263+i*8)
            wl = np.polynomial.polynomial.polyval(
                np.arange(1, xdim+1, 1), coef)
            if(datatype == 0):  # 0 floating point
                data = read_float_array(f, 4100, xdim*ydim*numFrames)
            elif (datatype == 1):
                data = read_LONG_array(f, 4100, xdim*ydim*numFrames)
            elif(datatype == 2):
                data = read_short_array(f, 4100, xdim*ydim*numFrames)
            elif(datatype == 3):
                data = read_ushort_array(f, 4100, xdim*ydim*numFrames)
        return wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm
    else:
        print(fn, ": Not an SPE file.")
        return


def readspe0(fn, sup):
    if os.path.splitext(fn)[1][1:].lower() == 'spe':
        with open(fn, 'rb') as f:

            # float   exp_sec          ;//             10  alternitive exposure, in sec.
            # WORD    xdim             ;//             42  actual # of pixels on x axis
            # short   datatype         ;//            108  experiment datatype
            #                           //                 0 =   float (4 bytes)
            #                           //                 1 =   long (4 bytes)
            #                           //                 2 =   short (2 bytes)
            #                           //                 3 =   unsigned short (2 bytes)
            # LONG    lavgexp          ;//            668  Number of Accumulations
            # LONG    NumFrames     ;//           1446  number of frames in file.
            # char    polynom_order_x         ;//      3101  ORDER of calibration POLYNOM
            # double  polynom_coeff_x[6]      ;//      3263  polynom COEFFICIENTS

            exp_sec = read_float(f, 10)
            outputter(fn, sup, 'exp_sec', exp_sec)
            xdim = read_WORD(f, 42)
            outputter(fn, sup, 'xdim', xdim)
            SpecCenterWlNm = read_float(f, 72)
            outputter(fn, sup, 'SpecCenterWlNm', SpecCenterWlNm)
            datatype = read_WORD(f, 108)
            outputter(fn, sup, 'datatype', datatype)
            ydim = read_WORD(f, 656)
            outputter(fn, sup, 'ydim', ydim)
            lavgexp = read_LONG(f, 668)
            outputter(fn, sup, 'lavgexp', lavgexp)
            numFrames = read_WORD(f, 1446)
            outputter(fn, sup, 'NumFrames', numFrames)
            outputter(
                fn, sup, 'polynom_order', read_WORD(f, 3101))
            coef = np.zeros(6)
            for i in np.arange(6):
                coef[i] = read_double(f, 3263+i*8)
                outputter(
                    fn, sup, 'calib '+str(i), coef[i])  # 3263
            wl = np.polynomial.polynomial.polyval(
                np.arange(1, xdim+1, 1), coef)
            # print(wl)
            if(datatype == 0):  # 0 floating point
                data = read_float_array(f, 4100, xdim*ydim*numFrames)
            elif (datatype == 1):
                data = read_LONG_array(f, 4100, xdim*ydim*numFrames)
            elif(datatype == 2):
                data = read_short_array(f, 4100, xdim*ydim*numFrames)
            elif(datatype == 3):
                data = read_ushort_array(f, 4100, xdim*ydim*numFrames)
        return wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm


def checkspecalib(fname, xdim, coef, SpecCenterWlNm, verbose):
    """
    Check calibration and Centeral wavelength of the spectrometer
    """
    wl = np.polynomial.polynomial.polyval((xdim+1)/2., coef)
    if verbose == True:
        print("Spectormeter Center: ", SpecCenterWlNm,
              ", Calibratied Center: ", wl)
    if np.abs(wl-SpecCenterWlNm) < 1:
        return True
    else:
        return False


def printspespan(fname, xdim, coef):
    """
    Print wavelength span
    """
    wlstart1 = np.polynomial.polynomial.polyval(0.5, coef)
    wlstart = np.polynomial.polynomial.polyval(1, coef)
    wlstart2 = np.polynomial.polynomial.polyval(1.5, coef)
    wlend1 = np.polynomial.polynomial.polyval(xdim-0.5, coef)
    wlend = np.polynomial.polynomial.polyval(xdim, coef)
    wlend2 = np.polynomial.polynomial.polyval(xdim+0.5, coef)
    print(fname, ": wavelength span [",
          wlstart,"(",wlstart1," ,",wlstart2,"), ",
          wlend, "(",wlend1," ,",wlend2,")]")
    
    return


def outputter(fn, fnflag, label, val):
    if(not fnflag):
        print(fn, ':', label, ':', val)
    else:
        print(label, ':', val)


def readspecomments(fn, sup):
    """
    Read commments in SPE files
    """
    start = 200
    length = 80
    if os.path.splitext(fn)[1][1:].lower() == 'spe':
        with open(fn, 'rb') as f:
            outputter(fn, sup, '1', read_string(f, start, length))
            outputter(fn, sup, '2', read_string(f, start+length, length))
            outputter(fn, sup, '3', read_string(f, start+length*2, length))
            outputter(fn, sup, '4', read_string(f, start+length*3, length))
            outputter(fn, sup, '5', read_string(f, start+length*4, length))


def writespectrum_csv(fname, wl, spectrum):
    """
    Write spectrum into file
    """
    df = pd.DataFrame({'wavelength': wl,
                       'intensity': spectrum,
                       })
    df.to_csv(fname)
    return
