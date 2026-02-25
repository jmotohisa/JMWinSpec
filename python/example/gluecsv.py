#!/usr/bin/env python
# -*- coding: utf-8 -*-

# glue_csv.py

# python gluecsv.py -s 800 -e 1700 -d a -i 120 -n 9 -o glue120 -g D1_a0
# python gluecsv.py -s 800 -e 1700 -d ~/Documents/experiment/20221125_orig/h -i 110 -n 10 -o glue_h110 -g D1_h

import argparse
import numpy as np
import matplotlib.pyplot as plt
import speutils


def get_args():
    # 準備
    parser = argparse.ArgumentParser(
        description='Glue csv files (wavelength,intensity format) with name: dir/basename_index:')
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
    parser.add_argument('-H', '--header', type=int,
                        nargs='?',
                        default=0,
                        help='header in csv file 0:no header (default) 1:skip'),
    parser.add_argument('-i', '--index',
                        nargs='?',
                        type=int,
                        help='starting index')
    parser.add_argument('-u', '--underscore',
                        action="store_true",
                        help='with underscore in file name')
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
                        help="output csv file name")
    parser.add_argument('-d', '--directory', type=str,  # argparse.FileType('r'),
                        nargs='?',
                        help="directory",
                        default='./')
    parser.add_argument('-a', '--autosave',
                        action="store_true",
                        help="automatically save graph")
    parser.add_argument('--log',
                        action="store_true",
                        help="logscale")
    parser.add_argument('basename', type=str,
                        help="base name")
    parser.add_argument('-v', '--verbose',
                        action="store_true",
                        help='verbose')

    args = parser.parse_args()

    return (args)


if __name__ == '__main__':
    args = get_args()
    bdir = args.directory
    bname = args.basename
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
    autosave = args.autosave
    logscale = args.log
    underscore = args.underscore
    flag_header = args.header

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
    for i in np.arange(0, nfiles, skip):
        if (underscore):
            fname_list.extend([bdir+'/'+bname+'_'+str(i+startindex)+'.csv'])
        else:
            fname_list.extend([bdir+'/'+bname+str(i+startindex)+'.csv'])

    print(fname_list)

    wl_list = []
    spectra_list = []
    for i, fname in enumerate(fname_list):
        wl, spectrum = speutils.readspectrum_csv(fname, flag_header)
        wl_list.extend([wl])
        spectra_list.extend([spectrum])

    wl_dest, spectrum0, flg0 = speutils.gluemultiplecsv(
        fname_list, start, end, resolution, edge_processing_mode, verbose, flag_header)

    if (graph):
        fig = plt.figure(figsize=(5, 5))
        fig.tight_layout()
        ax1 = fig.add_subplot(111)
        if (logscale):
            ax1.set_yscale('log')
        for i in np.arange(len(wl_list)):
            ax1.plot(wl_list[i], spectra_list[i], color='red')

        ax2 = ax1.twinx()
        if (logscale):
            ax2.set_yscale('log')
        ax2.plot(wl_dest, spectrum0, color='black')

        if (autosave):
            if (len(out) > 0):
                plt.savefig(out+'.png')
        plt.show()

        # fig = plt.figure(figsize=(5, 5))
        # ax1 = fig.add_subplot(111)
        # ax1.plot(wl_ref, spectrum_ref/spectrum0-1)
        # plt.show()

    if (len(out) > 0):
        speutils.writespectrum_csv(out + '.csv', wl_dest, spectrum0)

    if (dump):
        for i, wl0 in enumerate(wl_ref):
            print(wl0, spectrum0[i])
