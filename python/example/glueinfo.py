#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import sys
import glob
import numpy as np
import matplotlib.pyplot as plt
import speutils
import speutils.binread as rd

  # short   SpecGlueFlag     ;//             76  T/F File is Glued
  # float   SpecGlueStartWlNm;//             78  Starting Wavelength in Nm
  # float   SpecGlueEndWlNm  ;//             82  Starting Wavelength in Nm
  # float   SpecGlueMinOvrlpNm;//            86  Minimum Overlap in Nm
  # float   SpecGlueFinalResNm;//            90  Final Resolution in Nm

def get_args():
    # 準備
    parser = argparse.ArgumentParser(
        description='Glue SPE spectra with name: dir/basename_index:')
    parser.add_argument('fnames', type=str,  # argparse.FileType('r'),
                        nargs='+',
                        help="SPE filen name")
    parser.add_argument('-s', '--sup',
                        action="store_true",
                        help='suppress filename output')

    args = parser.parse_args()

    return(args)


if __name__ == '__main__':
    args = get_args()
    fnames = args.fnames
    sup = args.sup
    for fn0 in fnames:
        # print(fn,os.path.splitext(fn)[1][1:].lower())
        fnames2 = glob.glob(fn0)
        for fn in fnames2:
            if os.path.splitext(fn)[1][1:].lower() == 'spe':
                with open(fn, 'rb') as f:
                    speutils.outputter(fn, sup, 'SpecGlueFlag', rd.read_short(f, 76))
                    speutils.outputter(fn, sup, 'SpecGlueStartWlNm', rd.read_float(f, 78))
                    speutils.outputter(fn, sup, 'SpecGlueEndWlNm', rd.read_float(f, 82))
                    speutils.outputter(fn, sup, 'SpecGlueMinOvrlpNm', rd.read_float(f, 86))
                    speutils.outputter(fn, sup, 'SpecGLueFinalResNm', rd.read_float(f, 90))
