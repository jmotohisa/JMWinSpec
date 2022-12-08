#!/usr/bin/env python
# -*- coding: utf-8 -*-

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


start = 800.
end = 1100.
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=np.int32)

bdir = '/Users/motohisa/Documents/experiment/20221125/a'
bname = 'D1_a0'
refspe = '/Users/motohisa/Documents/experiment/20221125/gluetest/glued/g02.SPE'

startindex = 120
nfiles = 3
skip = 2

fname_list = []
for i in np.arange(0, nfiles, skip):
    fname_list.extend([bdir+'/'+bname+'_'+str(i+startindex)+'.SPE'])

print(fname_list)

norm_exp_sec = True
verbose = True
edge_processing_mode = 2

wl_list = []
spectra_list = []

for i, fname in enumerate(fname_list):
    wl, spectrum = getspectra_sub(fname, norm_exp_sec)
    wl_list.extend([wl])
    spectra_list.extend([spectrum])

if(len(refspe) > 0):
    wl_ref, spectrum_ref = getspectra_sub(refspe, norm_exp_sec)


wl_dest, spectrum0, flg0 = speutils.gluemultiplespe(
    fname_list, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose)

fig = plt.figure(figsize=(5, 5))
fig.tight_layout()
ax1 = fig.add_subplot(111)
ax1.set_yscale('log')
for i in np.arange(len(wl_list)):
    ax1.plot(wl_list[i], spectra_list[i], color='red')

ax2 = ax1.twinx()
ax2.set_yscale('log')
ax2.plot(wl_dest, spectrum0, color='black')

if(len(refspe) > 0):
    wl_ref, spectrum_ref = getspectra_sub(refspe, norm_exp_sec)
    ax2.plot(wl_ref, spectrum_ref, color='blue', linestyle='--')

plt.show()
speutils.writespectrum_csv('glued.csv', wl_dest, spectrum0)
speutils.writespectrum_csv('ref.csv', wl_ref, spectrum_ref)

fig = plt.figure(figsize=(5, 5))
ax1 = fig.add_subplot(111)
ax1.plot(wl_ref, spectrum_ref/spectrum0-1)
plt.show()
