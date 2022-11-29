#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from binread import *
import argparse
import os
import sys
import glob

def outputter(fname,fnameflag,label,val):
    if(not fnameflag) :
        print(fname,':',label,':',val)
    else:
        print(label,':',val)
        

def get_args():
    # 準備
    parser = argparse.ArgumentParser(description='Read Winspec Header')
    
    # 標準入力以外の場合
    # if sys.stdin.isatty():
    #     parser.add_argument('basefname', help='base file name', type=str)
    parser.add_argument('fnames',type=str, #argparse.FileType('r'),
                        nargs='+',
                        help="SPE filen name")
    parser.add_argument('-n', '--none',\
                        action="store_true", \
                        help='without comment output')
    parser.add_argument('-s', '--sup',\
                        action="store_true", \
                        help='suppress filename output')
    # parser.add_argument('-r', '--rangey',\
    #                     nargs='?',\
    #                     type=float,\
    #                     help='yrange full')
    # parser.add_argument("--alert", help="optional", action="store_true")

    # 結果を受ける
    args = parser.parse_args()

    return(args)

if __name__ == '__main__':

    args = get_args()
    fnames=args.fnames
    sup=args.sup
    # print(fnames)
    # fn='/Users/motohisa/Documents/experiment/20200124/W001.spe'
    
    # commments start at, with length
    start=200
    length=80

    for fn0 in fnames:
        # print(fn,os.path.splitext(fn)[1][1:].lower())
        fnames2=glob.glob(fn0)
        for fn in fnames2:
            if os.path.splitext(fn)[1][1:].lower() == 'spe' :
                with open(fn,'rb') as f:
                    # print(read_short(f,0)) #ControllerVersion
                    # f.seek(4,0)
                    # exposure = f.read(2)
                    
                    outputter(fn,sup,'exp_sec',read_float(f,10))
                    outputter(fn,sup,'xDimDet',read_WORD(f,6))
                    outputter(fn,sup,'yDimDet',read_WORD(f,18))
                    outputter(fn,sup,'date',read_string(f,20,10)) # date
                    outputter(fn,sup,'xdim',read_WORD(f,42)) # xdim
                    outputter(fn,sup,'ydim',read_WORD(f,656))
                    outputter(fn,sup,'NumFrames',read_WORD(f,1446))
                    outputter(fn,sup,'SpecCenterWlNm',read_float(f,72))
                    #    print(read_DWORD(f,114)) # PulserRepeatExp
                
                    if not args.none:
                        outputter(fn,sup,'1',read_string(f,start,length))
                        outputter(fn,sup,'2',read_string(f,start+length,length))
                        outputter(fn,sup,'3',read_string(f,start+length*2,length))
                        outputter(fn,sup,'4',read_string(f,start+length*3,length))
                        outputter(fn,sup,'5',read_string(f,start+length*4,length))
            
                    outputter(fn,sup,'lnoscan',read_LONG(f,664)) # lnoscan
                    outputter(fn,sup,'lavgexp',read_LONG(f,668)) # lavgexp
                    outputter(fn,sup,'NumFrames',read_LONG(f,1446)) # NumFrames
                    outputter(fn,sup,'file_header_ver', read_float(f,1992))
                    outputter(fn,sup,'offset_x',read_double(f,3000))
                
