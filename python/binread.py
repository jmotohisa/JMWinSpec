#!/usr/bin/env python
# -*- coding: utf-8 -*-

import struct
# import binascii

# def hex_to_double(s):
#     if s.startswith('0x'):
#         s = s[2:]
#     s = s.replace(' ', '')
#     return struct.unpack('>d', binascii.unhexlify(s))[0]

# def hex_to_float(s):
#     if s.startswith('0x'):
#         s = s[2:]
#     s = s.replace(' ', '')
#     return struct.unpack('>f', binascii.unhexlify(s))[0]

def read_float(f,offset):
    f.seek(offset,0)
    x=f.read(4)
    return hex_to_float(x)

def read_WORD(f,offset):
    f.seek(offset,0)
    x=f.read(2)
    return hex_to_WORD(x)

def read_short(f,offset):
    f.seek(offset,0)
    x=f.read(2)
    return struct.unpack('<h',x)[0]

def read_DWORD(f,offset):
    f.seek(offset,0)
    x=f.read(4)
    return struct.unpack('<L',x)[0]

def read_LONG(f,offset):
    f.seek(offset,0)
    x=f.read(4)
    return struct.unpack('<l',x)[0]

def read_double(f,offset):
    f.seek(offset,0)
    x=f.read(8)
    return struct.unpack('<d',x)[0]

def read_string(f,offset,length):
    f.seek(offset,0)
    x=f.read(length)
    str=x.decode('ascii')
    str = str.replace('\0', '')    #  \0を削除
    str = str.strip('\r\n')        #  改行文字を削除
    return str

def hex_to_WORD(s):
    return struct.unpack('<H',s)[0]

def hex_to_float(s):
    return struct.unpack('<f',s)[0]
