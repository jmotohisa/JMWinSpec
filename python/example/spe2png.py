#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" Example of speutils: convert SPE file to png image """

import os
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
                        help="vertical logscale")
    parser.add_argument('-e', '--energy',
                        action="store_true",
                        help="horizontal energy")
    parser.add_argument('-a', '--autosave',
                        action="store_true",
                        help="automatically save graph")
    parser.add_argument('-n', '--noplot',
                        action="store_true",
                        help="automatically save graph")
    parser.add_argument('-g', '--glued',
                        action="store_true",
                        help="only glued SPE")

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fnames = []
    logscale = args.log
    energyscale = args.energy
    autosave = args.autosave
    noplot = args.noplot
    glued = args.glued

    for arg in args.fname:
        fnames += glob.glob(arg)

    norm_exp_sec = True

    for i, fname in enumerate(fnames):
        with open(fname, 'rb') as f:
            fglued = True
            if(glued):
                fglued = speutils.binread.read_short(f, 76)
                print(fname, ' glued.')
            if(fglued):
                wl, spectrum = speutils.readspe_simple(fname, norm_exp_sec)
                fig = plt.figure(figsize=(5, 5))
                fig.tight_layout()
                ax1 = fig.add_subplot(111)

                base_dir_pair = os.path.split(fname)
                fname0 = os.path.splitext(os.path.basename(fname))[0]

                if(logscale):
                    ax1.set_yscale('log')
                if(energyscale):
                    ax1.plot(1239.8/wl, spectrum, label=fname0)
                else:
                    ax1.plot(wl, spectrum, label=fname0)
                ax1.legend(loc='best')
                ax1.set_title(base_dir_pair[0])

                # cdir = os.getcwd()
                # print(os.path.splitext(fname)[0]+'.png')
                if(autosave):
                    plt.savefig(os.path.splitext(fname)+'.png')
                if(~noplot):
                    plt.show()
