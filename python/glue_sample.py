#!/usr/bin/env python
# -*- coding: utf-8 -*-

import glue
import numpy as np
import matplotlib.pyplot as plt

start = 800.
end = 1000.
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, dtype=np.float64)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=np.int32)

fname1 = '/Users/motohisa/Documents/experiment/20221125/a/D1_a0_120.SPE'
fname2 = '/Users/motohisa/Documents/experiment/20221125/a/D1_a0_121.SPE'

wl1, spectrum1, coef1, numFrames1, xdim1, ydim1, exp_sec1, lavgexp1, SpecCenterWlNm1 = glue.readspe(
    fname1)
wl2, spectrum2, coef2, numFrames2, xdim2, ydim2, exp_sec2, lavgexp2, SpecCenterWlNm2 = glue.readspe(
    fname2)

if ydim1 != 1 or ydim2 != 1 or numFrames1 != 1 or numFrames2 != 1:
    print("numFrames and ydim should be 1 in gluing spectra")
    exit

if glue.checkspecalib(xdim1, coef1, SpecCenterWlNm1) == False:
    print("Calibration error in ", fname1, ". Exiting.")
    exit

if glue.checkspecalib(xdim2, coef2, SpecCenterWlNm2) == False:
    print("Calibration error in ", fname2, ". Exiting.")
    exit

spectrum1_dest = np.empty_like(wl_dest)
flg1_dest = np.empty_like(flg)
spectrum2_dest = np.empty_like(wl_dest)
flg2_dest = np.empty_like(flg)

# print(spectrum1.dtype, coef1.dtype, spectrum1_dest.dtype, flg1_dest.dtype)

glue.pyspeconvert0(spectrum1.astype(np.float64), coef1, wl_dest, spectrum1_dest,
                   flg1_dest, start, end, resolution)
glue.pyspeconvert0(spectrum2.astype(np.float64), coef2, wl_dest, spectrum2_dest,
                   flg2_dest, start, end, resolution)

# xdim1, ydim1, NumFrames1, flg_wlcen1, ierror1 = glue.pyspeconvert(
#     wl_dest, spectrum1_dest, flg1_dest, fname1, start, end, resolution, 0)
# xdim2, ydim2, NumFrames2, flg_wlcen2, ierror2 = glue.pyspeconvert(
#     wl_dest, spectrum2_dest, flg2_dest, fname2, start, end, resolution, 0)

fig = plt.figure(figsize=(5, 5))
fig.tight_layout()
ax1 = fig.add_subplot(111)
ax1.plot(wl1, spectrum1, color='red')
ax1.plot(wl2, spectrum2, color='blue')
ax2 = ax1.twinx()
ax2.plot(wl_dest, spectrum1_dest, color='red', linestyle='--')
ax2.plot(wl_dest, spectrum2_dest, color='blue', linestyle='--')
fig.show()


spectrum_2glued = np.empty_like(wl_dest)
flg_2glued = np.empty_like(wl_dest, dtype=np.int32)
glue.pyspeglue2(
    wl_dest, spectrum1_dest, flg1_dest, spectrum2_dest, flg2_dest,
    spectrum_2glued, flg_2glued)

fig = plt.figure(figsize=(5, 5))
fig.tight_layout()
ax1 = fig.add_subplot(1, 1, 1)
ax1.plot(wl1, spectrum1, color='red')
# ax1.set_title(r'carrier density, $\gamma_{ph}$='+str(gamma_ph),size=18)
# ax1.legend(loc='best',fontsize=14)
ax1.set_xlabel(r'wavelength $\lambda$ (nm)', size=12)
ax1.set_ylabel(r'intensity', size=12)
ax1.tick_params(axis='x', labelsize=12)
ax1.tick_params(axis='y', labelsize=12)
ax1.plot(wl2, spectrum2, color='blue')

ax2 = ax1.twinx()
ax2.plot(wl_dest, spectrum_2glued, color='black', linestyle='--')
ax2.tick_params(axis='y', labelsize=12)
plt.show()
