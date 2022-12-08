#!/usr/bin/env python
# -*- coding: utf-8 -*-

# python glues.py -s 800 -e 1700 -i 120 -n 9 -o glue120 -g [spefiles]

import argparse
import globs
import numpy as np
import matplotlib.pyplot as plt
import speutils


def getspectra_sub(fname, norm_exp_sec):
    wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = speutils.readspe(
        fname)
    if norm_exp_sec == True:
        data = data.astype(np.float64)/exp_sec
    else:
        data = data.astype(np.float64)

    return wl, data


def get_args():
    # 準備
    parser = argparse.ArgumentParser(
        description='Glue SPE spectra with name: dir/basename_index:')
    # 標準入力以外の場合
    # if sys.stdin.isatty():
    #     parser.add_argument('basefname', help='base file name', type=str)
    parser.add_argument('-s', '--start',
                        nargs='?',
                        type=float,
                        help='starting wavelength')
    parser.add_argument('-e', '--end',
                        nargs='?',
                        type=float,
                        help='ending wavelength')
    parser.add_argument('-r', '--res',
                        nargs='?',
                        type=float,
                        default=1,
                        help='resolution')
    parser.add_argument('-i', '--index',
                        nargs='?',
                        type=int,
                        help='starting index')
    parser.add_argument('-n', '--num',
                        nargs='?',
                        type=int,
                        default=1,
                        help='number of files')
    parser.add_argument('--skip',
                        nargs='?',
                        type=int,
                        help='skips (default=1)', default=1)
    parser.add_argument('--dump',
                        action="store_true",
                        help='dump glued spectra')
    parser.add_argument('-g', '--graph',
                        action="store_true",
                        help='plot original and glued spectra')
    parser.add_argument('-o', '--out', type=str,
                        nargs='?',
                        help="output csv fiile name")
    parser.add_argument('fnames', type=str,
                        nargs='+',
                        help="SPE fiile name")
    parser.add_argument('--log',
                        action="store_true",
                        help='plot logarizmic scale')
    parser.add_argument('-a', '--autosave',
                        action="store_true",
                        help="automatically save graph")
    parser.add_argument('-v', '--verbose',
                        action="store_true",
                        help='verbose')

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    start = args.start
    end = args.end
    resolution = args.res
    startindex = args.index
    nfiles = args.num
    skip = args.skip
    out = args.out
    dump = args.dump
    verbose = args.verbose
    graph = args.graph
    fnames = args.fnames
    autosave = args.autosave
    logscale = args.log

    norm_exp_sec = True
    edge_processing_mode = 2

    # print(fnames)
    # fn='/Users/motohisa/Documents/experiment/20200124/W001.spe'

    # bdir = '/Users/motohisa/Documents/experiment/20221125/a'
    # bname = 'D1_a0'
    # refspe = '/Users/motohisa/Documents/experiment/20221125/gluetest/glued/g02.SPE'

    # start = 800.
    # end = 1100.
    # resolution = 1.0

    # startindex = 120
    # nfiles = 3
    # skip = 2

    wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
    spectrum_dest = np.empty_like(wl_dest)
    flg = np.empty_like(wl_dest, dtype=np.int32)

    fname_list = []
    for fn0 in fnames:
        fnames2 = glob.glob(fn0)
        for fn in fnames2:
            fname_list.extend(fn)
        # print(fn,os.path.splitext(fn)[1][1:].lower())
    # for i in np.arange(0, nfiles, skip):
    #     fname_list.extend([bdir+'/'+bname+'_'+str(i+startindex)+'.SPE'])

    print(fname_list)

    wl_list = []
    spectra_list = []

    for i, fname in enumerate(fname_list):
        wl, spectrum = getspectra_sub(fname, norm_exp_sec)
        wl_list.extend([wl])
        spectra_list.extend([spectrum])

    wl_dest, spectrum0, flg0 = speutils.gluemultiplespe(
        fname_list, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose)

    if(len(out) > 0):
        speutils.writespectrum_csv(out + '.csv', wl_dest, spectrum0)

    if(dump):
        for i, wl0 in enumerate(wl_ref):
            print(wl0, spectrum0[i])

    if(graph):
        fig = plt.figure(figsize=(5, 5))
        fig.tight_layout()
        ax1 = fig.add_subplot(111)
        if(logscale):
            ax1.set_yscale('log')
        for i in np.arange(len(wl_list)):
            ax1.plot(wl_list[i], spectra_list[i], color='red')

        ax2 = ax1.twinx()
        if(logscale):
            ax2.set_yscale('log')
        ax2.plot(wl_dest, spectrum0, color='black')

        if(autosave):
            if(len(out) > 0):
                plt.savefig(out+'.png')
        plt.show()

        # fig = plt.figure(figsize=(5, 5))
        # ax1 = fig.add_subplot(111)
        # ax1.plot(wl_ref, spectrum_ref/spectrum0-1)
        # plt.show()
