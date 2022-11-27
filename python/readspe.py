#!/usr/bin/env python
# -*- coding: utf-8 -*-

from binread import *
import readcomments
import argparse
import os
import sys
import glob
import numpy as np
import matplotlib.pyplot as plt


def get_args():
    # 準備
    parser = argparse.ArgumentParser(description='Read Winspec Header')

    # 標準入力以外の場合
    # if sys.stdin.isatty():
    #     parser.add_argument('basefname', help='base file name', type=str)
    parser.add_argument('fnames', type=str,  # argparse.FileType('r'),
                        nargs='+',
                        help="SPE filen name")
    parser.add_argument('-n', '--none',
                        action="store_true",
                        help='without comment output')
    parser.add_argument('-s', '--sup',
                        action="store_true",
                        help='suppress filename output')
    # parser.add_argument('-r', '--rangey',\
    #                     nargs='?',\
    #                     type=float,\
    #                     help='yrange full')
    # parser.add_argument("--alert", help="optional", action="store_true")

    # 結果を受ける
    args = parser.parse_args()

    return(args)


def readspe(fn):
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
            readcomments.outputter(fn, sup, 'exp_sec', exp_sec)
            npoint = read_WORD(f, 42)
            readcomments.outputter(fn, sup, 'npoint', npoint)
            datatype = read_WORD(f, 108)
            readcomments.outputter(fn, sup, 'datatype', datatype)
            ydim = read_WORD(f, 656)
            readcomments.outputter(fn, sup, 'ydim', ydim)
            lavgexp = read_LONG(f, 668)
            readcomments.outputter(fn, sup, 'lavgexp', lavgexp)
            numFrames = read_WORD(f, 1446)
            readcomments.outputter(fn, sup, 'NumFrames', numFrames)
            readcomments.outputter(
                fn, sup, 'polynom_order', read_WORD(f, 3101))
            coef = np.zeros(6)
            for i in np.arange(6):
                coef[i] = read_double(f, 3263+i*8)
                readcomments.outputter(
                    fn, sup, 'calib '+str(i), coef[i])  # 3263
            wl = np.polynomial.polynomial.polyval(np.arange(npoint), coef)
            # print(wl)
            if(datatype == 0):  # 0 floating point
                data = read_float_array(f, 4100, npoint*numFrames)
            elif (datatype == 1):
                data = read_LONG_array(f, 4100, npoint*numFrames)
            elif(datatype == 2):
                data = read_short_array(f, 4100, npoint*numFrames)
            elif(datatype == 3):
                data = read_ushort_array(f, 4100, npoint*numFrames)

        return wl, data/exp_sec, coef, numFrames, ydim


if __name__ == '__main__':
    args = get_args()
    fnames = args.fnames
    sup = args.sup
    # print(fnames)
    # fn='/Users/motohisa/Documents/experiment/20200124/W001.spe'

    for fn0 in fnames:
        # print(fn,os.path.splitext(fn)[1][1:].lower())
        fnames2 = glob.glob(fn0)
        for fn in fnames2:
            # print(data)
            wl, data, coef, numFrames, ydim = readspe(fn)
            if(numFrames == 1 and ydim == 1):
                fig = plt.plot(wl, data)
                plt.show()
