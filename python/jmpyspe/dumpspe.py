#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import os
import sys
import glob
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

import glue

bdir = '/Users/motohisa/Documents/experiment/20221125/a'
bname = 'D1_a0'
refspe = '/Users/motohisa/Documents/experiment/20221125/gluetest/glued/g02.SPE'

fn=refspe
fnout='g02.csv'

wl, spectrum, coef, numFrames, xdim, ydim, exp_sec, lavgexp, SpecCenterWlNm = glue.readspe(fn)
spectrum=spectrum/exp_sec
glue.writespectrum_csv(fnout, wl, spectrum)
fig=plt.figure()
plt.plot(wl,spectrum)
fig.show()
