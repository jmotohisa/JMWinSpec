#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import argparse


def get_args():
    # 準備
    parser = argparse.ArgumentParser(
        description='plot CSV')
    # 標準入力以外の場合
    # if sys.stdin.isatty():
    #     parser.add_argument('basefname', help='base file name', type=str)
    # parser.add_argument('-s', '--start',
    #                     nargs='?',
    #                     type=float,
    #                     help='starting wavelength')
    # parser.add_argument('-e', '--end',
    #                     nargs='?',
    #                     type=float,
    #                     help='ending wavelength')
    # parser.add_argument('-r', '--res',
    #                     nargs='?',
    #                     type=float,
    #                     default=1,
    #                     help='resolution')
    # parser.add_argument('-i', '--index',
    #                     nargs='?',
    #                     type=int,
    #                     help='starting index')
    # parser.add_argument('-u', '--underscore',
    #                     action="store_true",
    #                     help='with underscore in file name')
    # parser.add_argument('-n', '--num',
    #                     nargs='?',
    #                     type=int,
    #                     default=1,
    #                     help='number of files')
    # parser.add_argument('--skip',
    #                     nargs='?',
    #                     type=int,
    #                     help='skips (default=1)', default=1)
    # parser.add_argument('--dump',
    #                     action="store_true",
    #                     help='dump glued spectra')
    # parser.add_argument('-g', '--graph',
    #                     action="store_true",
    #                     help='plot original and glued spectra')
    # parser.add_argument('-o', '--out', type=str,
    #                     nargs='?',
    #                     help="output csv file name")
    # parser.add_argument('-d', '--directory', type=str,  # argparse.FileType('r'),
    #                     nargs='?',
    #                     help="directory",
    #                     default='./')
    # parser.add_argument('-a', '--autosave',
    #                     action="store_true",
    #                     help="automatically save graph")
    # parser.add_argument('--log',
    #                     action="store_true",
    #                     help="logscale")
    parser.add_argument('fname', type=str,
                        help="csv file name")
    # parser.add_argument('-v', '--verbose',
    #                     action="store_true",
    #                     help='verbose')

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fname = args.fname
    f = pd.read_csv(fname)
    plt.plot(f['wavelength'], f['intensity'])
    plt.show()
