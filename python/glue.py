#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import sys
import glob
import numpy as np
import matplotlib.pyplot as plt
import readspe


def get_args():
    # 準備
    parser = argparse.ArgumentParser(description='Read Winspec Header')

    # 標準入力以外の場合
    # if sys.stdin.isatty():
    #     parser.add_argument('basefname', help='base file name', type=str)
    parser.add_argument('fnames', type=str,  # argparse.FileType('r'),
                        nargs='+',
                        help="SPE filen name")
    parser.add_argument('-s', '--start',
                        nargs='?',
                        type=float,
                        help='starting wavelength')
    parser.add_argument('-e', '--end',
                        nargs='?',
                        type=float,
                        help='ending wavelength')
    parser.add_argument('-r', '--resolution',
                        nargs='?',
                        type=float,
                        help='resolution')
    # parser.add_argument('-s', '--sup',
    #                     action="store_true",
    #                     help='suppress filename output')
    # parser.add_argument("--alert", help="optional", action="store_true")

    # 結果を受ける
    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fnames = args.fnames
    start = args.start
    end = args.end
    resolusion = args.resolution

    wl_dest = np.linspace(start, end, resolution)
    # print(fnames)
    # fn='/Users/motohisa/Documents/experiment/20200124/W001.spe'

    for fn0 in fnames:
        fnames2 = glob.glob(fn0)
        for fn in fnames2:
            # print(data)
            wl, data, coef, numFrames, ydim = readspe(fn)
            if(numFrames == 1 and ydim == 1):
                wl, data, coef, numFrames, ydim = readspe.readspe(fn)
