#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import sys
import glob

import speutils
import speutils.binread as rd


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

    start = 200
    length = 80
    for fn0 in fnames:
        # print(fn,os.path.splitext(fn)[1][1:].lower())
        fnames2 = glob.glob(fn0)
        for fn in fnames2:
            if os.path.splitext(fn)[1][1:].lower() == 'spe':
                with open(fn, 'rb') as f:
                    if not args.none:
                        speutils.outputter(
                            fn, sup, 'exp_sec', read_float(f, 10))
                        speutils.outputter(fn, sup, 'xDimDet', read_WORD(f, 6))
                        speutils.outputter(
                            fn, sup, 'yDimDet', read_WORD(f, 18))
                        speutils.outputter(
                            fn, sup, 'date', read_string(f, 20, 10))  # date
                        speutils.outputter(
                            fn, sup, 'xdim', read_WORD(f, 42))  # xdim
                        speutils.outputter(fn, sup, 'ydim', read_WORD(f, 656))
                        speutils.outputter(
                            fn, sup, 'NumFrames', read_WORD(f, 1446))
                        speutils.outputter(
                            fn, sup, 'SpecCenterWlNm', read_float(f, 72))
                        speutils.outputter(
                            fn, sup, '1', rd.read_string(f, start, length))
                        speutils.outputter(
                            fn, sup, '2', rd.read_string(f, start+length, length))
                        speutils.outputter(fn, sup, '3', rd.read_string(
                            f, start+length*2, length))
                        speutils.outputter(fn, sup, '4', rd.read_string(
                            f, start+length*3, length))
                        speutils.outputter(fn, sup, '5', rd.read_string(
                            f, start+length*4, length))

                        speutils.outputter(
                            fn, sup, 'lnoscan', read_LONG(f, 664))  # lnoscan
                        speutils.outputter(
                            fn, sup, 'lavgexp', read_LONG(f, 668))  # lavgexp
                        speutils.outputter(
                            fn, sup, 'NumFrames', read_LONG(f, 1446))  # NumFrames
                        speutils.outputter(
                            fn, sup, 'file_header_ver', read_float(f, 1992))
                        speutils.outputter(
                            fn, sup, 'offset_x', read_double(f, 3000))
