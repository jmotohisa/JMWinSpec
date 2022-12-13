#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import glob
import numpy as np
import matplotlib.pyplot as plt
import speutils


def get_args():
    parser = argparse.ArgumentParser(
        description='view SPE spectra')
    parser.add_argument('fname', help='dat file name', type=str, nargs='+')
    parser.add_argument('--log',
                        action="store_true",
                        help="logscale")
    parser.add_argument('-a', '--autosave',
                        action="store_true",
                        help="automatically save graph")

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fnames = []
    logscale = args.log

    for arg in args.fname:
        fnames += glob.glob(arg)

    norm_exp_sec = True

    wl_list = []
    spectra_list = []

    for i, fname in enumerate(fnames):
        wl, spectrum = speutils.readspe_simple(fname, norm_exp_sec)
        wl_list.extend([wl])
        spectra_list.extend([spectrum])

    fig = plt.figure(figsize=(5, 5))
    fig.tight_layout()
    ax1 = fig.add_subplot(111)
    if(logscale):
        ax1.set_yscale('log')
    for i in np.arange(len(wl_list)):
        ax1.plot(wl_list[i], spectra_list[i], color='red')

    plt.show()
