/* http://www.konkoly.hu/staff/racz/Winhead.html
                                  WINHEAD.TXT 

                            $Date: 10/18/96 12:26p $ 

                Header Structure For WinView/WinSpec (WINX) Files 

    The current data file used for WINX files consists of a 4100 (1004 Hex) 
    byte header followed by the data. 
    Up through version 1.4.3, the original CSMA header was kept intact so 
    the header contains many items which are present only to keep 
    compatability with the Princeton Instruments DOS based products (DSMA,CSMA) 
    Additional items used by WINX only were added after header offset 3000 
    (BB8 Hex). 

              **************** IMPORTANT CHANGES **************** 

    Beginning with Version 1.6, many more items were added to the header to 
    make it a complete as possible record of the data collection. 
    All data files created under previous versions of WinView/WinSpec CAN 
    still be read correctly. 
    HOWEVER, files created under the new versions (1.6 and higher) CANNOT be 
    read by previous versions of WinView/WinSpec OR by the CSMA software 
    package. 

              *************************************************** 
 

                                  Decimal Byte 
                                     Offset 
-----------
*/
// typedef
struct WINXHEAD{
unsigned int  dioden;            /*    0  num of physical pixels (X axis)    */ 
int           avgexp;            /*    2  number of accumulations per scan   */ 
                                 /*         if > 32767, set to -1 and        */ 
                                 /*         see lavgexp below (668)          */ 
int           exposure;          /*    4  exposure time (in milliseconds)    */ 
                                 /*         if > 32767, set to -1 and        */ 
                                 /*         see lexpos below (660)           */ 
unsigned int  xDimDet;           /*    6  Detector x dimension of chip       */ 
int           mode;              /*    8  timing mode                        */ 
float         exp_sec;           /*   10  alternative exposure, in secs.     */ 
int           asyavg;            /*   14  number of asynchron averages       */ 
int           asyseq;            /*   16  number of asynchron sequential     */ 
unsigned int  yDimDet;           /*   18  y dimension of CCD or detector.    */ 
char          date[10];          /*   20  date as MM/DD/YY                   */ 
int           ehour;             /*   30  Experiment Time: Hours (as binary) */ 
int           eminute;           /*   32  Experiment Time: Minutes(as binary)*/ 
int           noscan;            /*   34  number of multiple scans           */ 
                                 /*       if noscan == -1 use lnoscan        */ 
int           fastacc;           /*   36                                     */ 
int           seconds;           /*   38  Experiment Time: Seconds(as binary)*/ 
int           DetType;           /*   40  CCD/DiodeArray type                */ 
unsigned int  xdim;              /*   42  actual # of pixels on x axis       */ 
int           stdiode;           /*   44  trigger diode                      */ 
float         nanox;             /*   46                                     */ 
float         calibdio[10];      /*   50  calibration diodes                 */ 
char          fastfile[16];      /*   90  name of pixel control file         */ 
int           asynen;            /*  106  asynchron enable flag  0 = off     */ 
int           datatype;          /*  108  experiment data type               */ 
                                 /*         0 =   FLOATING POINT             */ 
                                 /*         1 =   LONG INTEGER               */ 
                                 /*         2 =   INTEGER                    */ 
                                 /*         3 =   UNSIGNED INTEGER           */ 
float         calibnan[10];      /*  110  calibration nanometer              */ 
int           BackGrndApplied;   /*  150  set to 1 if background sub done    */ 
int           astdiode;          /*  152                                     */ 
unsigned int  minblk;            /*  154  min. # of strips per skips         */ 
unsigned int  numminblk;         /*  156  # of min-blocks before geo skps    */ 
double        calibpol[4];       /*  158  calibration coeffients             */ 
unsigned int  ADCrate;           /*  190  ADC rate                           */ 
unsigned int  ADCtype;           /*  192  ADC type                           */ 
unsigned int  ADCresolution;     /*  194  ADC resolution                     */ 
unsigned int  ADCbitAdjust;      /*  196  ADC bit adjust                     */ 
unsigned int  gain;              /*  198  gain                               */ 
char          exprem[5][80];     /*  200  experiment remarks                 */ 
unsigned int  geometric;         /*  600  geometric operations rotate 0x01   */ 
                                 /*       reverse 0x02, flip 0x04            */ 
char          xlabel[16];        /*  602  Intensity display string           */ 
unsigned int  cleans;            /*  618  cleans                             */ 
unsigned int  NumSkpPerCln;      /*  620 number of skips per clean.          */ 
char          califile[16];      /*  622  calibration file name (CSMA)       */ 
char          bkgdfile[16];      /*  638  background file name               */ 
int           srccmp;            /*  654  number of source comp. diodes      */ 
unsigned int  ydim;              /*  656  y dimension of raw data.           */ 
int           scramble;          /*  658  0 = scrambled, 1 = unscrambled     */ 
long          lexpos;            /*  660  long exposure in milliseconds      */ 
                                 /*         used if exposure set to -1       */ 
long          lnoscan;           /*  664  long num of scans                  */ 
                                 /*         used if noscan set to -1         */ 
long          lavgexp;           /*  668  long num of accumulations          */ 
                                 /*         used if avgexp set to -1         */ 
char          stripfil[16];      /*  672  stripe file (st130)                */ 
char    sw_version[16];    /*  688  Version of SW creating this file 
int           type;              /*  704   1 = new120 (Type II)              */ 
                                 /*        2 = old120 (Type I )              */ 
                                 /*        3 = ST130                         */ 
                                 /*        4 = ST121                         */ 
                                 /*        5 = ST138                         */ 
                                 /*        6 = DC131 (PentaMax)              */ 
                                 /*        7 = ST133 (MicroMax/SpectroMax),  */ 
                                 /*        8 = ST135 (GPIB)                  */ 
                                 /*        9 = VICCD                         */ 
                                 /*       10 = ST116 (GPIB)                  */ 
                                 /*       11 = OMA3 (GPIB)                   */ 
                                 /*       12 = OMA4                          */ 
int           flatFieldApplied;  /*  706  Set to 1 if flat field was applied */ 
int           spare[8];          /*  708  reserved                           */ 
int           kin_trig_mode;     /*  724  Kinetics Trigger Mode              */ 
char          dlabel[16];        /*  726  Data label.       */ 
char          empty[686];        /*  742  EMPTY BLOCK FOR EXPANSION          */ 
float         clkspd_us;         /* 1428 Vert Clock Speed in micro-sec       */ 
int           HWaccumFlag;       /* 1432 set to 1 if accum done by Hardware  */ 
int           StoreSync;         /* 1434 set to 1 if store sync used.        */ 
int           BlemishApplied;    /* 1436 set to 1 if blemish removal applied */ 
int           CosmicApplied;     /* 1438 set to 1 if cosmic ray removal done */ 
int           CosmicType;        /* 1440 if cosmic ray applied, this is type */ 
float         CosmicThreshold;   /* 1442 Threshold of cosmic ray removal.    */ 
long          NumFrames;         /* 1446 number of frames in file.           */ 
float         MaxIntensity;      /* 1450 max intensity of data (future)      */ 
float         MinIntensity;      /* 1454 min intensity of data (future)      */ 
char          ylabel[LABELMAX];  /* 1458 y axis label.                       */ 
unsigned int  ShutterType;       /* 1474 shutter type.                       */ 
float         shutterComp;       /* 1476 shutter compensation time.          */ 
unsigned int  readoutMode;       /* 1480 Readout mode, full,kinetics, etc    */ 
unsigned int  WindowSize;        /* 1482 window size for kinetics only.      */ 
unsigned int  clkspd;            /* 1484 clock speed for kinetics &          */ 
                                 /*      frame transfer.                     */ 
unsigned int  interface_type;    /* 1486 computer interface (isa-taxi,       */ 
                                 /*      pci, eisa, etc.)                    */ 
unsigned long ioAdd1;            /* 1488 I/O address of inteface card.       */ 
unsigned long ioAdd2;            /* 1492 if more than one address for card.  */ 
unsigned long ioAdd3;            /* 1496                                     */ 
unsigned int  intLevel;          /* 1500 interrupt level interface card      */ 
unsigned int  GPIBadd;           /* 1502  GPIB address (if used)             */ 
unsigned int  ControlAdd;        /* 1504  GPIB controller address (if used)  */ 
unsigned int  controllerNum;     /* 1506  if multiple controller system will */ 
                                 /*       have controller # data came from.  */ 
                                 /*       (Future Item)                      */ 
unsigned int  SWmade;            /* 1508  Software which created this file   */ 
int           NumROI;            /* 1510  number of ROIs used. if 0 assume 1 */ 
                                 /* 1512 - 1630  ROI information             */ 
struct ROIinfo {                 /*                                          */ 
 unsigned int startx;            /* left x start value.                      */ 
 unsigned int endx;              /* right x value.                           */ 
 unsigned int groupx;            /* amount x is binned/grouped in hw.        */ 
 unsigned int starty;            /* top y start value.                       */ 
 unsigned int endy;              /* bottom y value.                          */ 
 unsigned int groupy;            /* amount y is binned/grouped in hw.        */ 
} ROIinfoblk[10];                /*    ROI Starting Offsets:                 */ 
                                 /*            ROI  1 = 1512                 */ 
                                 /*            ROI  2 = 1524                 */ 
                                 /*            ROI  3 = 1536                 */ 
                                 /*            ROI  4 = 1548                 */ 
                                 /*            ROI  5 = 1560                 */ 
                                 /*            ROI  6 = 1572                 */ 
                                 /*            ROI  7 = 1584                 */ 
                                 /*            ROI  8 = 1596                 */ 
                                 /*            ROI  9 = 1608                 */ 
                                 /*            ROI 10 = 1620                 */ 
char          FlatField[120];    /* 1632 Flat field file name.               */ 
char          background[120];   /* 1752 Background sub. file name.          */ 
char          blemish[120];      /* 1872 Blemish file name.                  */ 
float    file_header_ver;   /* 1992 Version of this file header   */ 
char          UserInfo[1000];    /* 1996-2995 user data.                     */ 
long          WinView_id;        /* 2996 Set to 0x01234567L if file was      */ 
                                 /*      created by WinX                     */ 
/*
------------------------------------------------------------------------------- 

                        START OF X CALIBRATION STRUCTURE 
*/
double        offset;            /* 3000  offset for absolute data scaling   */ 
double        factor;            /* 3008  factor for absolute data scaling   */ 
char          current_unit;      /* 3016  selected scaling unit              */ 
char          reserved1;         /* 3017  reserved                           */ 
char          string[40];        /* 3018  special string for scaling         */ 
char          reserved2[40];     /* 3058  reserved                           */ 
char          calib_valid;       /* 3098  flag if calibration is valid       */ 
char          input_unit;        /* 3099  current input units for            */ 
                                 /*       "calib_value"                      */ 
char          polynom_unit;      /* 3100  linear UNIT and used               */ 
                                 /*       in the "polynom_coeff"             */ 
char          polynom_order;     /* 3101  ORDER of calibration POLYNOM       */ 
char          calib_count;       /* 3102  valid calibration data pairs       */ 
double        pixel_position[10];/* 3103  pixel pos. of calibration data     */ 
double        calib_value[10];   /* 3183  calibration VALUE at above pos     */ 
double        polynom_coeff[6];  /* 3263  polynom COEFFICIENTS               */ 
double        laser_position;    /* 3311  laser wavenumber for relativ WN    */ 
char          reserved3;         /* 3319  reserved                           */ 
unsigned char new_calib_flag;    /* 3320  If set to 200, valid label below   */ 
char          calib_label[81];   /* 3321  Calibration label (NULL term'd)    */ 
char          expansion[87];     /* 3402  Calibration Expansion area         */ 
/*
------------------------------------------------------------------------------- 

                        START OF Y CALIBRATION STRUCTURE 
*/
double        offset;            /* 3489  offset for absolute data scaling   */ 
double        factor;            /* 3497  factor for absolute data scaling   */ 
char          current_unit;      /* 3505  selected scaling unit              */ 
char          reserved1;         /* 3506  reserved                           */ 
char          string[40];        /* 3507  special string for scaling         */ 
char          reserved2[40];     /* 3547  reserved                           */ 
char          calib_valid;       /* 3587  flag if calibration is valid       */ 
char          input_unit;        /* 3588  current input units for            */ 
                                 /*       "calib_value"                      */ 
char          polynom_unit;      /* 3589  linear UNIT and used               */ 
                                 /*       in the "polynom_coeff"             */ 
char          polynom_order;     /* 3590  ORDER of calibration POLYNOM       */ 
char          calib_count;       /* 3591  valid calibration data pairs       */ 
double        pixel_position[10];/* 3592  pixel pos. of calibration data     */ 
double        calib_value[10];   /* 3672  calibration VALUE at above pos     */ 
double        polynom_coeff[6];  /* 3752  polynom COEFFICIENTS               */ 
double        laser_position;    /* 3800  laser wavenumber for relativ WN    */ 
char          reserved3;         /* 3808  reserved                           */ 
unsigned char new_calib_flag;    /* 3809  If set to 200, valid label below   */ 
	char          calib_label[81];   /* 3810  Calibration label (NULL term'd)    */ 
	char          expansion[87];     /* 3891  Calibration Expansion area         */ 
/*
                         END OF CALIBRATION STRUCTURES 

    --------------------------------------------------------------------- 
*/
	char          Istring[40];       /* 3978  special Intensity scaling string   */ 
	char          empty3[80];        /* 4018  empty block to reach 4100 bytes    */ 
	int           lastvalue;         /* 4098 Always the LAST value in the header */ 
};
/*
                                 START OF DATA 
                             -------------------- 

    The data follows the header beginning at offset 4100. 

    In WinView/WinSpec, the data is always stored exactly as it is 
    collected.  The order of the data depends on the placement of the 
    shift register. 

    In the diagram below, the shift register is on the RIGHT SIDE of the 
    chip.  Each COLUMN of data is first shifted RIGHT into the shift 
    register and then DOWN.  The data is read (and stored) in this order: 

           First column read:    XnYm, XnYm-1, ... XnY2, XnY1 
 
           Last column read :    X1Ym, X1Ym-1, ... X1Y2, X1Y1 

            .------------------------------------------.---. 
            |   X1Y1       X2Y1       ...       XnY1   | S | 
            |   X1Y2       X2Y2       ...       XnY2   | H |  | 
            |     :         :                    :     | I |  | 
            |   X1Ym-1     X2Ym-1     ...       XnYm-1 | F |  V 
            |   X1Ym       X2Ym       ...       XnYm   | T | 
            `------------------------------------------|---| 
                              ------>                  | | | 
                                                         | 
                                                         `-----> A/D 

    In the diagram below, the shift register is on the BOTTOM of the 
    chip.  Each ROW of data is first shifted DOWN into the shift 
    register and then RIGHT.  The data is read (and stored) in this order: 

           First row read:    XnYm, ... X2Ym, X1Ym 
 
           Last row read :    XnY1, ... X2Y1, X1Y1 

            .------------------------------------------. 
            |   X1Y1       X2Y1       ...       XnY1   | 
            |   X1Y2       X2Y2       ...       XnY2   |  | 
            |     :         :                    :     |  | 
            |   X1Ym-1     X2Ym-1     ...       XnYm-1 |  V 
            |   X1Ym       X2Ym       ...       XnYm   | 
            `------------------------------------------|---. 
            |              SHIFT REGISTER              | | | 
                              ----->                     | 
                                                         `-----> A/D 

                      READING STRIPS AND FRAMES OF DATA 
                     ----------------------------------- 

 DATA FILES FROM CSMA AND UP TO AND INCLUDING WINX VERSION 1.4.3 
 --------------------------------------------------------------- 
    Data is stored as sequential points.  The X, Y and Frame dimensions are 
    determined by the header.  The X dimension is in "faccount" ( Offset  42 ). 
    The total number of strips in the file is in "noscan" ( Offset  34 ) OR 
    if the total number is > 32767, "noscan" is set to -1 and the number of 
    strips is stored as a LONG in "lnoscan" ( Offset 664 ).  The number of 
    strips per frame is stored in "stripe" ( Offset 656 ).  To determine the 
    number of frames, divide "noscan" (or "lnoscan") by "stripe". 

    Thus: 

        char header[4100]; 
        int  X_dimension; 
        int  temp_y_dim; 
        long Y_dimension; 
        int  Total_strips; 
        int  Num_frames; 

        X_dimension = (int)header[42]; 

        temp_y_dim  = (int)header[34]; 
        if( temp_y_dim == -1 ) 
            Y_dimension = (long)header[664]; 
        else 
            Y_dimension = (long)temp_y_dim; 

        Total_strips = (int)header[656]; 
        Num_frames   = (long)Total_strips / Y_dimension; 
 
 

 DATA FILES FROM VERSION 1.6 AND BEYOND... 
 ----------------------------------------- 
    Data is still stored as sequential points.  The X, Y and Frame dimensions 
    are determined by the header. 
    The X dimension of the stored data is in "xdim" ( Offset 42  ). 
    The Y dimension of the stored data is in "ydim" ( Offset 656 ). 
    The number of frames of data stored is in "NumFrames" ( Offset 1446 ). 

    Thus (modifying the example above): 

        char header[4100]; 
        unsigned int X_dimension; 
        unsigned int Y_dimension; 
        long         Num_frames; 
*/
             /* Now there is Direct Access of data dimensions */ 
/*        X_dimension = (unsigned int)header[42]; 
        Y_dimension = (unsigned int)header[664]; 
        Num_frames  = (long)header[1446]; 
*/
