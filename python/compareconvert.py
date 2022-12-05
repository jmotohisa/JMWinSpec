#!/usr/bin/env python
# -*- coding: utf-8 -*-

import glue
import numpy as np
import matplotlib.pyplot as plt
import readspe


def getspectra_sub_convert(fname, start, end, resolution, norm_exp_sec, verbose):
    wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = glue.readspe(
        fname)
    if norm_exp_sec == True:
        data = data.astype(np.float64)/exp_sec
    else:
        data = data.astype(np.float64)

    wl_dest = np.arange(start, end+resolution, resolution)
    flg_dest = np.empty_like(wl_dest, dtype=np.int32)
    spectrum_dest = np.empty_like(wl_dest)

    glue.pyspeconvert0(data, coef, wl_dest, spectrum_dest,
                       flg_dest, start, end, resolution)
    return wl, data, wl_dest, spectrum_dest


start = 800.
end = 1000.
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=np.int32)

bdir = '/Users/motohisa/Documents/experiment/20221125/a'
bname = 'D1_a0'

startindex = 120
nfiles = 3
skip = 1

fname_list = []
for i in np.arange(0, nfiles, skip):
    fname_list.extend([bdir+'/'+bname+'_'+str(i+startindex)+'.SPE'])

print(fname_list)

norm_exp_sec = True
verbose = True

for i, fname in enumerate(fname_list):
    wl, spectrum, wl_dest, spectrum_dest = getspectra_sub_convert(
        fname, start, end, resolution, norm_exp_sec, verbose)
    fig = plt.figure(figsize=(5, 5))
    fig.tight_layout()
    ax1 = fig.add_subplot(111)
    ax1.plot(wl, spectrum, color='red')
    ax1.set_yscale('log')
    ax2 = ax1.twinx()
    ax2.set_yscale('log')
    ax2.plot(wl_dest, spectrum_dest, color='black')
    plt.show()
