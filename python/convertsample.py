#!/usr/bin/env python
# -*- coding: utf-8 -*-

import glue
import numpy as np

start = 1150
end = 1250
resolution = 1.0

wl_dest = np.arange(start, end+resolution, resolution, endpoint, dtype=float)
spectrum_dest = np.empty_like(wl_dest)
flg = np.empty_like(wl_dest, dtype=int)

fname = 'test.SPE'

wl_det, spectrum_dest, flg, xdim, ydim, NumFrames, flag_wlcen, ierror = glue.pyspeconvert(
    fname, start, end, resolution, False)
