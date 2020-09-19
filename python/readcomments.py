#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from binread import *
import argparse
import sys
import glob

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
    # parser.add_argument('-r', '--rangey',\
    #                     nargs='?',\
    #                     type=float,\
    #                     help='yrange full')
    # parser.add_argument("--alert", help="optional", action="store_true")

    # 結果を受ける
    args = parser.parse_args()

    return(args)


args = get_args()
fnames=args.fnames
print(fnames)
# fn='/Users/motohisa/Documents/experiment/20200124/W001.spe'

# commments start at, with length
start=200
length=80

for fn in fnames:
    with open(fn,'rb') as f:
        print(read_short(f,0)) #ControllerVersion
        # f.seek(4,0)
        # exposure = f.read(2)
        
        print('exp_sec: ',read_float(f,10))
        print('xDimDet: ',read_WORD(f,6))
        print('yDimDet: ',read_WORD(f,18))
        print(read_string(f,20,10)) # date
        print('SpecCenterWlNm: ',read_float(f,72))
        #    print(read_DWORD(f,114)) # PulserRepeatExp
    
        if not args.none:
            print(fn,':',read_string(f,start,length))
            print(fn,':',read_string(f,start+length,length))
            print(fn,':',read_string(f,start+length*2,length))
            print(fn,':',read_string(f,start+length*3,length))
            print(fn,':',read_string(f,start+length*4,length))
            
        print('lnoscan: ',read_LONG(f,664)) # lnoscan
        print('lavgexp: ',read_LONG(f,668)) # lavgexp
        print('NumFrames: ',read_LONG(f,1446)) # NumFrames
        print('file_header_ver :', read_float(f,1992))
        print('offset_x :',read_double(f,3000))
