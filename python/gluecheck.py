#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt
import glue
import numpy as np
import matplotlib.pyplot as plt
import readspe


def getspectra_sub(fname, norm_exp_sec):
    wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = glue.readspe(
        fname)
    if norm_exp_sec == True:
        data = data.astype(np.float64)/exp_sec
    else:
        data = data.astype(np.float64)

    return wl, data, coef


start = 800.
end = 1000.
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=np.int32)

bdir = '/Users/motohisa/Documents/experiment/20221125/a'
bname = 'D1_a0'
refspe = '/Users/motohisa/Documents/experiment/20221125/gluetest/glued/g01.SPE'

startindex = 120
nfiles = 2
skip = 1

fname_list = []
for i in np.arange(0, nfiles, skip):
    fname_list.extend([bdir+'/'+bname+'_'+str(i+startindex)+'.SPE'])

print(fname_list)

norm_exp_sec = True
verbose = True

wl_list = []
spectra_list = []

for i, fname in enumerate(fname_list):
    wl, spectrum, coef = getspectra_sub(fname, norm_exp_sec)
    glue.pyspeconvert0(spectrum, coef, wl_dest, spectrum_dest,
                       flg, start, end, resolution)
    glue.writespectrum_csv('glued'+str(i)+'.csv', wl_dest, spectrum_dest)

wl_ref, spectrum_ref, coef_ref = getspectra_sub(refspe, norm_exp_sec)
glue.writespectrum_csv('ref.csv', wl_ref, spectrum_ref)
