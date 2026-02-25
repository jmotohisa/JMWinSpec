#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import numpy as np
import matplotlib.pyplot as plt
import speutils

def get_args():
    parser = argparse.ArgumentParser(
        description='check exp_sec and lavgexp in SPE file')
    parser.add_argument('fnames', type=str,
                        nargs='+',
                        help="SPE fiile name")

    args = parser.parse_args()

    return(args)

if __name__ == '__main__':
    args = get_args()
    fnames = args.fnames
    
    for fname in fnames:
        wl, data, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = speutils.readspe(fname)
        print(fname,exp_sec,lavgexp)
        
