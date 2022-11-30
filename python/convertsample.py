#!/usr/bin/env python
# -*- coding: utf-8 -*-

import glue
import numpy as np
import matplotlib.pyplot as plt

# start = 1100.0
# end = 1300.0
start = 1600.
end = 1800.
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=np.int32)

# print(wl_dest.dtype, spectrum_dest.dtype, flg.dtype)

fname = '/Users/motohisa/Documents/experiment/20221125/a/mod/D1_a0_73.SPE'

wl, spectrum, coef, numFrames, ydim = glue.readspe(fname)
# print(numFrames, ydim)
# fig = plt.plot(wl, spectrum)
# plt.show()

xdim, ydim, NumFrames, flag_wlcen, ierror = glue.pyspeconvert(
    wl_dest, spectrum_dest, flg, fname, start, end, resolution, 0)
# print(xdim, ydim, NumFrames, flag_wlcen, ierror)
# print(spectrum_dest)
# print(flg)
if flag_wlcen:
    print("Warning: Calibration data and Center of Spectrometer does not much")

fig = plt.figure(figsize=(5, 5))
fig.tight_layout()
ax1 = fig.add_subplot(1, 1, 1)
ax1.plot(wl, spectrum, color='red')
# ax1.set_title(r'carrier density, $\gamma_{ph}$='+str(gamma_ph),size=18)
# ax1.legend(loc='best',fontsize=14)
ax1.set_xlabel(r'wavelength $\lambda$ (nm)', size=18)
ax1.set_ylabel(r'intensity', size=18)
ax1.tick_params(axis='x', labelsize=18)
ax1.tick_params(axis='y', labelsize=18)

ax2 = ax1.twinx()
ax2.plot(wl_dest, spectrum_dest, color='blue', linestyle='--')
ax2.tick_params(axis='y', labelsize=18)

plt.show()
