#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from .readspe import *
import speutils


def glue2spe(fname1, fname2, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose):
    """
    Glue spectrum of two SPE file
    Returns wl_dest, spectrum_2glued, flg_2glued
    """
    wl1, spectrum1, coef1, numFrames1, xdim1, ydim1, exp_sec1, lavgexp1, SpecCenterWlNm1 = readspe(
        fname1)
    wl2, spectrum2, coef2, numFrames2, xdim2, ydim2, exp_sec2, lavgexp2, SpecCenterWlNm2 = readspe(
        fname2)

    if ydim1 != 1 or ydim2 != 1 or numFrames1 != 1 or numFrames2 != 1:
        print("numFrames and ydim should be 1 in gluing spectra")
    exit

    if checkspecalib(fname1, xdim1, coef1, SpecCenterWlNm1, verbose) == False:
        print("Calibration error in ", fname1, ". Exiting.")
    exit

    if checkspecalib(fname2, xdim2, coef2, SpecCenterWlNm2, verbose) == False:
        print("Calibration error in ", fname2, ". Exiting.")
    exit

    if verbose == True:
        printspespan(fname1, xdim1, coef1)
        printspespan(fname2, xdim2, coef2)

    if norm_exp_sec == True:
        spectrum1 = spectrum1.astype(np.float64)/exp_sec1
        spectrum2 = spectrum2.astype(np.float64)/exp_sec2
    else:
        spectrum1 = spectrum1.astype(np.float64)
        spectrum2 = spectrum2.astype(np.float64)

    wl_dest = np.arange(start, end+resolution, resolution)
    flg = np.empty_like(wl_dest, dtype=np.int32)
    spectrum1_dest = np.empty_like(wl_dest)
    flg1_dest = np.empty_like(flg)
    spectrum2_dest = np.empty_like(wl_dest)
    flg2_dest = np.empty_like(flg)

    speutils.pyspeconvert0(spectrum1.astype(np.float64), coef1, wl_dest, spectrum1_dest,
                           flg1_dest, start, end, resolution)
    speutils.pyspeconvert0(spectrum2.astype(np.float64), coef2, wl_dest, spectrum2_dest,
                           flg2_dest, start, end, resolution)

    spectrum_2glued = np.empty_like(wl_dest)
    flg_2glued = np.empty_like(wl_dest, dtype=np.int32)
    speutils.pyspeglue2(
        wl_dest, spectrum1_dest, flg1_dest, spectrum2_dest, flg2_dest,
        spectrum_2glued, flg_2glued, edge_processing_mode)

    return wl_dest, spectrum_2glued, flg_2glued


def gluespe1(wl_dest, spectrum0, flg0, fname2, norm_exp_sec, edge_processing_mode, verbose):
    """
    Glue converted spectrum (spectrum0) and Spectrum of SPE file
    Returns wl_dest, spectrum_2glued, flg_2glued

    """
    start = wl_dest[0]
    end = wl_dest[wl_dest.shape[0]-1]
    resolution = wl_dest[1]-wl_dest[0]

    wl2, spectrum2, coef2, numFrames2, xdim2, ydim2, exp_sec2, lavgexp2, SpecCenterWlNm2 = readspe(
        fname2)

    if ydim2 != 1 or numFrames2 != 1:
        print("numFrames and ydim should be 1 in gluing spectra")
    exit

    if checkspecalib(fname2, xdim2, coef2, SpecCenterWlNm2, verbose) == False:
        print("Calibration error in ", fname2, ". Exiting.")
    exit

    if verbose == True:
        print("Start, end, resution=", start, end, resolution)
        printspespan(fname2, xdim2, coef2)

    if norm_exp_sec == True:
        spectrum2 = spectrum2.astype(np.float64)/exp_sec2
    else:
        spectrum2 = spectrum2.astype(np.float64)

    spectrum2_dest = np.empty_like(wl_dest)
    flg2_dest = np.empty_like(flg0)
    speutils.pyspeconvert0(spectrum2, coef2, wl_dest, spectrum2_dest,
                           flg2_dest, start, end, resolution)

    spectrum_2glued = np.empty_like(wl_dest)
    flg_2glued = np.empty_like(flg0)
    speutils.pyspeglue2(
        wl_dest, spectrum0, flg0, spectrum2_dest, flg2_dest,
        spectrum_2glued, flg_2glued, edge_processing_mode)

    return wl_dest, spectrum_2glued, flg_2glued


def gluemultiplespe(fname_list, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose):
    """
    Glue spectrum in the fname_list
    Returns wl_dest, spectrum0, flg0
    """
    if len(fname_list) <= 1:
        print("Error: fname_list should contain elements more than two")
        exit

    if len(fname_list) == 2:
        fname1 = fname_list[0]
        fname2 = fname_list[1]
        wl_dest, spectrum0, flg0 = glue2spe(
            fname1, fname2, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose)
    else:
        fname1 = fname_list.pop(0)
        fname2 = fname_list.pop(0)
        print(fname1, fname2)
        wl_dest, spectrum0, flg0 = glue2spe(
            fname1, fname2, start, end, resolution, norm_exp_sec, edge_processing_mode, verbose)
        for fname2 in fname_list:
            wl_dest, spectrum0, flg0 = gluespe1(
                wl_dest, spectrum0, flg0, fname2, norm_exp_sec, edge_processing_mode, verbose)

    return wl_dest, spectrum0, flg0
