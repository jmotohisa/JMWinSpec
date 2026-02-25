#!/usr/bin/env python
# -*- coding: utf-8 -*-
""" Example: view SPE file """

import argparse
import os
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
    parser.add_argument('-s', '--separate',
                        action="store_true",
                        help="separate display")

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fnames = []
    logscale = args.log
    separate = args.separate

    for arg in args.fname:
        fnames += glob.glob(arg)

    norm_exp_sec = True

    wl_list = []
    spectra_list = []
    
    for i, fname in enumerate(fnames):
        wl, spectrum = speutils.readspe_simple(fname, norm_exp_sec)
        wl_list.extend([wl])
        spectra_list.extend([spectrum])

    if(separate == False):
        fig = plt.figure(figsize=(5, 5))
        fig.tight_layout()
        ax1 = fig.add_subplot(111)
        if(logscale):
            ax1.set_yscale('log')
        for i in np.arange(len(wl_list)):
            ax1.plot(wl_list[i], spectra_list[i],
                     label=os.path.splitext(os.path.basename(fnames[i]))[0])
        plt.legend(loc='best')
        plt.show()
    else:
        for i in np.arange(len(wl_list)):
            fig = plt.figure(figsize=(5, 5))
            fig.tight_layout()
            ax1 = fig.add_subplot(111)
            ax1.plot(wl_list[i], spectra_list[i], color='red',
                                 label=os.path.splitext(os.path.basename(fnames[i]))[0])
            
            if(logscale):
                ax1.set_yscale('log')

            plt.title(os.path.split(fnames[i])[0])
            plt.legend(loc='best')
            plt.show()
