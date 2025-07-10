#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import argparse
import numpy as np
import pandas as pd
import speutils
import matplotlib.pyplot as plt


def get_args():
    # 準備

    parser = argparse.ArgumentParser(
        description='convert SPE file to CSV file')
    parser.add_argument('fnames', type=str,  # argparse.FileType('r'),
                        nargs='+',
                        help="SPE filen name")
    parser.add_argument('-f', '--first', type=int,  # starting index
                        nargs='?',
                        default=0,
                        help='first index of the pixel in SPE file')
    parser.add_argument('-l', '--last', type=int,  # ending index
                        nargs='?',
                        default=0,
                        help='last index of the pixel in SPE file')
    parser.add_argument('-s', '--sup',
                        action="store_true",
                        help='suppress filename output')
    parser.add_argument('-p', '--plot',
                        action="store_true",
                        help='plot spectrum')
    # parser.add_argument('-v', '--verbose',
    #                     action="store_true",
    #                     help='verbose')

    args = parser.parse_args()

    return (args)


if __name__ == '__main__':
    args = get_args()
    fnames = args.fname
    # fname_out = args.out
    sup = args.sup
    index_first = args.first
    index_last = args.last
    flag_plot = args.plot

    # if (len(fname_out):
    #    basename_without_ext=os.path.splitext(os.path.basename(filepath))[0]
    #    fname_out=dirname(fname)+"/"+basename_without_ext+".csv"

    for fn0 in fnames:
        # print(fn,os.path.splitext(fn)[1][1:].lower())
        fnames2 = glob.glob(fn0)
        for fn in fname2:
            wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = speutils.readspe0(
                fn, sup)
            dim2list = []
            if (index_fist > 0):
                range1 = index_first
            else:
                range1 = 0
            if (index_last > 0 and index_last < len(wl)):
                range2 = index_last
            else:
                range2 = len(wl)

            for i in range([range1, range2]):
                dim2list = dim2list.append([wl[i], data[i]])
            df = pd.DataFrame(dim2list, columns=['wavelength', 'intensity'])
            basename_without_ext = os.path.splitext(os.path.basename(fn))[0]
            fname_out = dirname(fn)+"/"+basename_without_ext+".csv"

            df.save_csv(fname_out, index=False, header=True)

            if (flag_plot):
                plt.plot(wl, data)
                if (range1 > 0 or range2 > 0):
                    plt.plot(wl[range1, range2], data[range1, range2])
                plt.show()
