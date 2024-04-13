#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import sys
import glob
import jmpyspe

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
            wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = jmpyspe.readspe0(
                fn, sup)
            if(numFrames == 1 and ydim == 1):
                fig = plt.plot(wl, data)
                plt.show()
