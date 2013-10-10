#include <aes.h>

static TEDINFO rs_tedinfo[] = {
/*0*/	{"Text", "", "", GDOS_PROP,5003,TE_LEFT,0x1180,10,-1,5,1},
/*1*/	{"Don't Click Jim's Head", "", "", GDOS_PROP,5003,TE_LEFT,0x1280,18,-1,23,1},
/*2*/	{"Please don't click on", "", "", IBM,6,TE_LEFT,0x1180,0,-1,22,1},
/*3*/	{"Jim's Head...", "", "", IBM,6,TE_LEFT,0x1180,0,-1,14,1},
/*4*/	{"Text", "", "", GDOS_PROP,5003,TE_LEFT,0x1180,10,-1,5,1},
/*5*/	{"Please don't click on", "", "", IBM,6,TE_LEFT,0x1180,0,-1,22,1},
/*6*/	{"Jim's Head...", "", "", IBM,6,TE_LEFT,0x1180,0,-1,14,1},
/*7*/	{"  Don't Click Jim's Head", "", "", IBM,5003,TE_LEFT,0x1280,18,-1,25,1},
/*8*/	{"Text", "", "", GDOS_PROP,5003,TE_LEFT,0x1180,10,-1,5,1},
/*9*/	{"Don't Click Jim's Head", "", "", GDOS_PROP,5003,TE_LEFT,0x1280,18,-1,23,1},
/*10*/	{"Please don't click on", "", "", IBM,6,TE_LEFT,0x1180,0,-1,22,1},
/*11*/	{"Jim's Head...", "", "", IBM,6,TE_LEFT,0x1180,0,-1,14,1},
/*12*/	{"Text", "", "", GDOS_PROP,5003,TE_LEFT,0x1180,10,-1,5,1},
/*13*/	{"Please don't click on", "", "", IBM,6,TE_LEFT,0x1180,0,-1,22,1},
/*14*/	{"Jim's Head...", "", "", IBM,6,TE_LEFT,0x1180,0,-1,14,1},
/*15*/	{"  Don't Click Jim's Head", "", "", IBM,5003,TE_LEFT,0x1280,18,-1,25,1},
};

static short BBDATA0[] = {
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x5f00,0x0000,0x0000,0x0000,
0x0002,0xefe0,0x0000,0x0000,
0x0000,0x0007,0xfffc,0x0000,
0x0000,0x0000,0x00bb,0xfdff,
0x8000,0x0000,0x0000,0x03ff,
0xfaff,0xf000,0x0000,0x0000,
0x0eff,0xfbbf,0xe800,0x0000,
0x0000,0x77ff,0xfddf,0xfc00,
0x0000,0x0000,0xbffd,0xfeff,
0xfa00,0x0000,0x0001,0xfffb,
0xff7f,0xfe00,0x0000,0x0002,
0xff5d,0xd7fe,0xee00,0x0000,
0x0007,0xfebb,0xb7ff,0xff80,
0x0000,0x0003,0xffff,0xffff,
0xfb80,0x0000,0x0007,0xffff,
0xffff,0xffe0,0x0000,0x000f,
0xefef,0xfeff,0xfee0,0x0000,
0x0017,0xffff,0xffff,0xfffc,
0x0000,0x000b,0xbbbf,0xfffb,
0xffba,0x0000,0x001f,0xffff,
0xffff,0xffff,0x0000,0x002e,
0xfefb,0xffff,0xbffe,0x8000,
0x007f,0xfffd,0xffff,0xdfff,
0xc000,0x003f,0xffff,0xfeff,
0xffff,0xb000,0x007f,0xffff,
0xffff,0xffff,0xf000,0x007f,
0xffdf,0xfff7,0xfff6,0xf800,
0x00ff,0xffbf,0xffaf,0xffef,
0xfc00,0x00f6,0xfff7,0xf576,
0xfdff,0xb800,0x00ef,0x7fbf,
0xfaed,0xfbff,0xfc00,0x00ff,
0xfffe,0xffff,0x7efb,0xfc00,
0x00ff,0xffff,0xfffe,0xfffd,
0xfc00,0x00ff,0xfbbb,0xbbbb,
0xbbbf,0xbc00,0x00ff,0xf7ff,
0xffff,0xffff,0xf800,0x007f,
0xfeef,0xeeee,0xeefe,0xf800,
0x007f,0xffff,0xffff,0xffff,
0xf800,0x007f,0xfbfd,0xaabb,
0xbfff,0xf800,0x007f,0xfffb,
0xffff,0xffff,0xf800,0x007f,
0xfffe,0xb20a,0xaaef,0xf800,
0x007f,0xffff,0xee17,0xffff,
0xf800,0x007f,0xffda,0xc000,
0x052b,0xf800,0x007f,0xff9f,
0xa000,0x005f,0xf800,0x007f,
0x196d,0x0000,0x014f,0xf800,
0x007e,0x1878,0x0000,0x0017,
0xf800,0x007d,0xe5ec,0x0005,
0x413f,0xfc00,0x0079,0xe1f8,
0x0002,0x005f,0xfe00,0x006e,
0xf7e8,0x14aa,0xacee,0xae00,
0x007f,0xe7f0,0x017f,0xf1ff,
0xff00,0x005b,0x4e6c,0x054a,
0xc4bb,0xab00,0x001c,0x3e7a,
0x003d,0xa3ff,0xff00,0x001c,
0x03e8,0x12ab,0x50ae,0xaf00,
0x0018,0x07f0,0x077e,0x81ff,
0xf700,0x0018,0x05b0,0x054b,
0x50aa,0xca00,0x0018,0x01e0,
0x003c,0x01ff,0x9e00,0x0014,
0x04b0,0x0010,0x012b,0x0c00,
0x0000,0x0168,0x0000,0x0076,
0x1800,0x0002,0x0030,0x0000,
0x5055,0x3000,0x0004,0x0040,
0x0000,0x0022,0x6000,0x0006,
0x0050,0x0001,0x5012,0xc000,
0x0000,0x0000,0x0000,0x080f,
0xc000,0x0003,0x0050,0x0000,
0x5053,0xc000,0x0000,0x0000,
0x0000,0x0007,0x8000,0x0000,
0x8010,0x0000,0x5557,0x8000,
0x0000,0x0000,0x0000,0x088f,
0x0000,0x0000,0x3050,0x0000,
0x00ab,0x0000,0x0000,0x6000,
0x0000,0x01de,0x0000,0x0000,
0x2d50,0x0000,0x04ae,0x0000,
0x0000,0x3800,0x0000,0x0174,
0x0000,0x1800,0x0c50,0x0000,
0x12ac,0x0000,0x1f80,0x1800,
0x0000,0x05d8,0x0000,0x084f,
0x0c10,0x0001,0x52a8,0x0000,
0x101f,0x1800,0x0000,0x0f78,
0x0000,0x0000,0xac04,0x0000,
0x54b0,0x0000,0x0001,0xf800,
0x0000,0x03d0,0x0000,0x0000,
0x5000,0x0000,0x0530,0x0000,
0x0000,0x0800,0x0000,0x00f0,
0x0000,0x0000,0x0000,0x0000,
0x00b0,0x0000,0x0000,0x0000,
0x0000,0x01f8,0x0000,0x0000,
0x0000,0x0100,0x04ec,0x0000,
0x0000,0x0000,0x0000,0x017e,
0x0200,0x0000,0x0000,0x0054,
0x000b,0x8200,0x0000,0x0000,
0x0000,0x001f,0xfc00,0x0000,
0x0000,0x0014,0x0001,0x2800,
};

static short BBDATA1[] = {
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x5f00,0x0000,0x0000,0x0000,
0x0002,0xefe0,0x0000,0x0000,
0x0000,0x0007,0xfffc,0x0000,
0x0000,0x0000,0x00bb,0xfdff,
0x8000,0x0000,0x0000,0x03ff,
0xfaff,0xf000,0x0000,0x0000,
0x0eff,0xfbbf,0xe800,0x0000,
0x0000,0x77ff,0xfddf,0xfc00,
0x0000,0x0000,0xbffd,0xfeff,
0xfa00,0x0000,0x0001,0xfffb,
0xff7f,0xfe00,0x0000,0x0002,
0xff5d,0xd7fe,0xee00,0x0000,
0x0007,0xfebb,0xb7ff,0xff80,
0x0000,0x0003,0xffff,0xffff,
0xfb80,0x0000,0x0007,0xffff,
0xffff,0xffe0,0x0000,0x000f,
0xefef,0xfeff,0xfee0,0x0000,
0x0017,0xffff,0xffff,0xfffc,
0x0000,0x000b,0xbbbf,0xfffb,
0xffba,0x0000,0x001f,0xffff,
0xffff,0xffff,0x0000,0x002e,
0xfefb,0xffff,0xbffe,0x8000,
0x007f,0xfffd,0xffff,0xdfff,
0xc000,0x003f,0xffff,0xfeff,
0xffff,0xb000,0x007f,0xffff,
0xffff,0xffff,0xf000,0x007f,
0xffdf,0xfff7,0xfff6,0xf800,
0x00ff,0xffbf,0xffaf,0xffef,
0xfc00,0x00f6,0xfff7,0xf576,
0xfdff,0xb800,0x00ef,0x7fbf,
0xfaed,0xfbff,0xfc00,0x00ff,
0xfffe,0xffff,0x7efb,0xfc00,
0x00ff,0xffff,0xfffe,0xfffd,
0xfc00,0x00ff,0xfbbb,0xbbbb,
0xbbbf,0xbc00,0x00ff,0xf7ff,
0xffff,0xffff,0xf800,0x007f,
0xfeef,0xeeee,0xeefe,0xf800,
0x007f,0xffff,0xffff,0xffff,
0xf800,0x007f,0xfbfd,0xaabb,
0xbfff,0xf800,0x007f,0xfffb,
0xffff,0xffff,0xf800,0x007f,
0xfffe,0xb20a,0xaaef,0xf800,
0x007f,0xffff,0xee17,0xffff,
0xf800,0x007f,0xffda,0xc000,
0x052b,0xf800,0x007f,0xff9f,
0xa000,0x005f,0xf800,0x007f,
0x196d,0x0000,0x014f,0xf800,
0x007e,0x1878,0x0000,0x0017,
0xf800,0x007d,0xe5ec,0x0005,
0x413f,0xfc00,0x0079,0xe1f8,
0x0002,0x005f,0xfe00,0x006e,
0xf7e8,0x14aa,0xacee,0xae00,
0x007f,0xe7f0,0x017f,0xf1ff,
0xff00,0x005b,0x4e6c,0x054a,
0xc4bb,0xab00,0x001c,0x3e7a,
0x003d,0xa3ff,0xff00,0x001c,
0x03e8,0x12ab,0x50ae,0xaf00,
0x0018,0x07f0,0x077e,0x81ff,
0xf700,0x0018,0x05b0,0x054b,
0x50aa,0xca00,0x0018,0x01e0,
0x003c,0x01ff,0x9e00,0x0014,
0x04b0,0x0010,0x012b,0x0c00,
0x0000,0x0168,0x0000,0x0076,
0x1800,0x0002,0x0030,0x0000,
0x5055,0x3000,0x0004,0x0040,
0x0000,0x0022,0x6000,0x0006,
0x0050,0x0001,0x5012,0xc000,
0x0000,0x0000,0x0000,0x080f,
0xc000,0x0003,0x0050,0x0000,
0x5053,0xc000,0x0000,0x0000,
0x0000,0x0007,0x8000,0x0000,
0x8010,0x0000,0x5557,0x8000,
0x0000,0x0000,0x0000,0x088f,
0x0000,0x0000,0x3050,0x0000,
0x00ab,0x0000,0x0000,0x6000,
0x0000,0x01de,0x0000,0x0000,
0x2d50,0x0000,0x04ae,0x0000,
0x0000,0x3800,0x0000,0x0174,
0x0000,0x1800,0x0c50,0x0000,
0x12ac,0x0000,0x1f80,0x1800,
0x0000,0x05d8,0x0000,0x084f,
0x0c10,0x0001,0x52a8,0x0000,
0x101f,0x1800,0x0000,0x0f78,
0x0000,0x0000,0xac04,0x0000,
0x54b0,0x0000,0x0001,0xf800,
0x0000,0x03d0,0x0000,0x0000,
0x5000,0x0000,0x0530,0x0000,
0x0000,0x0800,0x0000,0x00f0,
0x0000,0x0000,0x0000,0x0000,
0x00b0,0x0000,0x0000,0x0000,
0x0000,0x01f8,0x0000,0x0000,
0x0000,0x0100,0x04ec,0x0000,
0x0000,0x0000,0x0000,0x017e,
0x0200,0x0000,0x0000,0x0054,
0x000b,0x8200,0x0000,0x0000,
0x0000,0x001f,0xfc00,0x0000,
0x0000,0x0014,0x0001,0x2800,
};

static short BBDATA2[] = {
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x5f00,0x0000,0x0000,0x0000,
0x0002,0xefe0,0x0000,0x0000,
0x0000,0x0007,0xfffc,0x0000,
0x0000,0x0000,0x00bb,0xfdff,
0x8000,0x0000,0x0000,0x03ff,
0xfaff,0xf000,0x0000,0x0000,
0x0eff,0xfbbf,0xe800,0x0000,
0x0000,0x77ff,0xfddf,0xfc00,
0x0000,0x0000,0xbffd,0xfeff,
0xfa00,0x0000,0x0001,0xfffb,
0xff7f,0xfe00,0x0000,0x0002,
0xff5d,0xd7fe,0xee00,0x0000,
0x0007,0xfebb,0xb7ff,0xff80,
0x0000,0x0003,0xffff,0xffff,
0xfb80,0x0000,0x0007,0xffff,
0xffff,0xffe0,0x0000,0x000f,
0xefef,0xfeff,0xfee0,0x0000,
0x0017,0xffff,0xffff,0xfffc,
0x0000,0x000b,0xbbbf,0xfffb,
0xffba,0x0000,0x001f,0xffff,
0xffff,0xffff,0x0000,0x002e,
0xfefb,0xffff,0xbffe,0x8000,
0x007f,0xfffd,0xffff,0xdfff,
0xc000,0x003f,0xffff,0xfeff,
0xffff,0xb000,0x007f,0xffff,
0xffff,0xffff,0xf000,0x007f,
0xffdf,0xfff7,0xfff6,0xf800,
0x00ff,0xffbf,0xffaf,0xffef,
0xfc00,0x00f6,0xfff7,0xf576,
0xfdff,0xb800,0x00ef,0x7fbf,
0xfaed,0xfbff,0xfc00,0x00ff,
0xfffe,0xffff,0x7efb,0xfc00,
0x00ff,0xffff,0xfffe,0xfffd,
0xfc00,0x00ff,0xfbbb,0xbbbb,
0xbbbf,0xbc00,0x00ff,0xf7ff,
0xffff,0xffff,0xf800,0x007f,
0xfeef,0xeeee,0xeefe,0xf800,
0x007f,0xffff,0xffff,0xffff,
0xf800,0x007f,0xfbfd,0xaabb,
0xbfff,0xf800,0x007f,0xfffb,
0xffff,0xffff,0xf800,0x007f,
0xfffe,0xb20a,0xaaef,0xf800,
0x007f,0xffff,0xee17,0xffff,
0xf800,0x007f,0xffda,0xc000,
0x052b,0xf800,0x007f,0xff9f,
0xa000,0x005f,0xf800,0x007f,
0x196d,0x0000,0x014f,0xf800,
0x007e,0x1878,0x0000,0x0017,
0xf800,0x007d,0xe5ec,0x0005,
0x413f,0xfc00,0x0079,0xe1f8,
0x0002,0x005f,0xfe00,0x006e,
0xf7e8,0x14aa,0xacee,0xae00,
0x007f,0xe7f0,0x017f,0xf1ff,
0xff00,0x005b,0x4e6c,0x054a,
0xc4bb,0xab00,0x001c,0x3e7a,
0x003d,0xa3ff,0xff00,0x001c,
0x03e8,0x12ab,0x50ae,0xaf00,
0x0018,0x07f0,0x077e,0x81ff,
0xf700,0x0018,0x05b0,0x054b,
0x50aa,0xca00,0x0018,0x01e0,
0x003c,0x01ff,0x9e00,0x0014,
0x04b0,0x0010,0x012b,0x0c00,
0x0000,0x0168,0x0000,0x0076,
0x1800,0x0002,0x0030,0x0000,
0x5055,0x3000,0x0004,0x0040,
0x0000,0x0022,0x6000,0x0006,
0x0050,0x0001,0x5012,0xc000,
0x0000,0x0000,0x0000,0x080f,
0xc000,0x0003,0x0050,0x0000,
0x5053,0xc000,0x0000,0x0000,
0x0000,0x0007,0x8000,0x0000,
0x8010,0x0000,0x5557,0x8000,
0x0000,0x0000,0x0000,0x088f,
0x0000,0x0000,0x3050,0x0000,
0x00ab,0x0000,0x0000,0x6000,
0x0000,0x01de,0x0000,0x0000,
0x2d50,0x0000,0x04ae,0x0000,
0x0000,0x3800,0x0000,0x0174,
0x0000,0x1800,0x0c50,0x0000,
0x12ac,0x0000,0x1f80,0x1800,
0x0000,0x05d8,0x0000,0x084f,
0x0c10,0x0001,0x52a8,0x0000,
0x101f,0x1800,0x0000,0x0f78,
0x0000,0x0000,0xac04,0x0000,
0x54b0,0x0000,0x0001,0xf800,
0x0000,0x03d0,0x0000,0x0000,
0x5000,0x0000,0x0530,0x0000,
0x0000,0x0800,0x0000,0x00f0,
0x0000,0x0000,0x0000,0x0000,
0x00b0,0x0000,0x0000,0x0000,
0x0000,0x01f8,0x0000,0x0000,
0x0000,0x0100,0x04ec,0x0000,
0x0000,0x0000,0x0000,0x017e,
0x0200,0x0000,0x0000,0x0054,
0x000b,0x8200,0x0000,0x0000,
0x0000,0x001f,0xfc00,0x0000,
0x0000,0x0014,0x0001,0x2800,
};

static short BBDATA3[] = {
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x5f00,0x0000,0x0000,0x0000,
0x0002,0xefe0,0x0000,0x0000,
0x0000,0x0007,0xfffc,0x0000,
0x0000,0x0000,0x00bb,0xfdff,
0x8000,0x0000,0x0000,0x03ff,
0xfaff,0xf000,0x0000,0x0000,
0x0eff,0xfbbf,0xe800,0x0000,
0x0000,0x77ff,0xfddf,0xfc00,
0x0000,0x0000,0xbffd,0xfeff,
0xfa00,0x0000,0x0001,0xfffb,
0xff7f,0xfe00,0x0000,0x0002,
0xff5d,0xd7fe,0xee00,0x0000,
0x0007,0xfebb,0xb7ff,0xff80,
0x0000,0x0003,0xffff,0xffff,
0xfb80,0x0000,0x0007,0xffff,
0xffff,0xffe0,0x0000,0x000f,
0xefef,0xfeff,0xfee0,0x0000,
0x0017,0xffff,0xffff,0xfffc,
0x0000,0x000b,0xbbbf,0xfffb,
0xffba,0x0000,0x001f,0xffff,
0xffff,0xffff,0x0000,0x002e,
0xfefb,0xffff,0xbffe,0x8000,
0x007f,0xfffd,0xffff,0xdfff,
0xc000,0x003f,0xffff,0xfeff,
0xffff,0xb000,0x007f,0xffff,
0xffff,0xffff,0xf000,0x007f,
0xffdf,0xfff7,0xfff6,0xf800,
0x00ff,0xffbf,0xffaf,0xffef,
0xfc00,0x00f6,0xfff7,0xf576,
0xfdff,0xb800,0x00ef,0x7fbf,
0xfaed,0xfbff,0xfc00,0x00ff,
0xfffe,0xffff,0x7efb,0xfc00,
0x00ff,0xffff,0xfffe,0xfffd,
0xfc00,0x00ff,0xfbbb,0xbbbb,
0xbbbf,0xbc00,0x00ff,0xf7ff,
0xffff,0xffff,0xf800,0x007f,
0xfeef,0xeeee,0xeefe,0xf800,
0x007f,0xffff,0xffff,0xffff,
0xf800,0x007f,0xfbfd,0xaabb,
0xbfff,0xf800,0x007f,0xfffb,
0xffff,0xffff,0xf800,0x007f,
0xfffe,0xb20a,0xaaef,0xf800,
0x007f,0xffff,0xee17,0xffff,
0xf800,0x007f,0xffda,0xc000,
0x052b,0xf800,0x007f,0xff9f,
0xa000,0x005f,0xf800,0x007f,
0x196d,0x0000,0x014f,0xf800,
0x007e,0x1878,0x0000,0x0017,
0xf800,0x007d,0xe5ec,0x0005,
0x413f,0xfc00,0x0079,0xe1f8,
0x0002,0x005f,0xfe00,0x006e,
0xf7e8,0x14aa,0xacee,0xae00,
0x007f,0xe7f0,0x017f,0xf1ff,
0xff00,0x005b,0x4e6c,0x054a,
0xc4bb,0xab00,0x001c,0x3e7a,
0x003d,0xa3ff,0xff00,0x001c,
0x03e8,0x12ab,0x50ae,0xaf00,
0x0018,0x07f0,0x077e,0x81ff,
0xf700,0x0018,0x05b0,0x054b,
0x50aa,0xca00,0x0018,0x01e0,
0x003c,0x01ff,0x9e00,0x0014,
0x04b0,0x0010,0x012b,0x0c00,
0x0000,0x0168,0x0000,0x0076,
0x1800,0x0002,0x0030,0x0000,
0x5055,0x3000,0x0004,0x0040,
0x0000,0x0022,0x6000,0x0006,
0x0050,0x0001,0x5012,0xc000,
0x0000,0x0000,0x0000,0x080f,
0xc000,0x0003,0x0050,0x0000,
0x5053,0xc000,0x0000,0x0000,
0x0000,0x0007,0x8000,0x0000,
0x8010,0x0000,0x5557,0x8000,
0x0000,0x0000,0x0000,0x088f,
0x0000,0x0000,0x3050,0x0000,
0x00ab,0x0000,0x0000,0x6000,
0x0000,0x01de,0x0000,0x0000,
0x2d50,0x0000,0x04ae,0x0000,
0x0000,0x3800,0x0000,0x0174,
0x0000,0x1800,0x0c50,0x0000,
0x12ac,0x0000,0x1f80,0x1800,
0x0000,0x05d8,0x0000,0x084f,
0x0c10,0x0001,0x52a8,0x0000,
0x101f,0x1800,0x0000,0x0f78,
0x0000,0x0000,0xac04,0x0000,
0x54b0,0x0000,0x0001,0xf800,
0x0000,0x03d0,0x0000,0x0000,
0x5000,0x0000,0x0530,0x0000,
0x0000,0x0800,0x0000,0x00f0,
0x0000,0x0000,0x0000,0x0000,
0x00b0,0x0000,0x0000,0x0000,
0x0000,0x01f8,0x0000,0x0000,
0x0000,0x0100,0x04ec,0x0000,
0x0000,0x0000,0x0000,0x017e,
0x0200,0x0000,0x0000,0x0054,
0x000b,0x8200,0x0000,0x0000,
0x0000,0x001f,0xfc00,0x0000,
0x0000,0x0014,0x0001,0x2800,
};

BITBLK BBOB0 = {BBDATA0,10,80,0,0,1};
BITBLK BBOB1 = {BBDATA1,10,80,0,0,1};
BITBLK BBOB2 = {BBDATA2,10,80,0,0,1};
BITBLK BBOB3 = {BBDATA3,10,80,0,0,1};

OBJECT JIMS_HEAD_M_L[] = {
/*0*/	{-1,1,5,G_BOX,0x400,0x0,(void *)0x21100,1539,1028,36,16},
/*1*/	{2,-1,-1,G_IMAGE,0x405,0x10,&BBOB0,2,4,10,11},
/*2*/	{3,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[0],15,1,4,1},
/*3*/	{4,-1,-1,G_TEXT,0x400,0x10,&rs_tedinfo[1],4,1,29,2},
/*4*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[2],13,6,22,1},
/*5*/	{0,-1,-1,G_TEXT,0x420,0x0,&rs_tedinfo[3],15,9,13,1},
};

OBJECT JIMS_HEAD_R_L[] = {
/*0*/	{-1,1,7,G_BOX,0x400,0x0,(void *)0x21100,1539,1028,38,15},
/*1*/	{2,-1,-1,G_IMAGE,0x5,0x10,&BBOB1,2,4,11,10},
/*2*/	{3,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[4],15,1,4,1},
/*3*/	{4,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[5],14,7,22,1},
/*4*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[6],16,9,13,1},
/*5*/	{7,6,6,G_BOX,0x400,0x30,(void *)0xff1100,3,1,32,2},
/*6*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[7],3,0,27,2},
/*7*/	{0,-1,-1,G_BUTTON,0x625,0x0," Play Mod ", 19,12,11,1},
};

OBJECT JIMS_HEAD_M_H[] = {
/*0*/	{-1,1,5,G_BOX,0x400,0x0,(void *)0x21100,1539,1028,36,11},
/*1*/	{2,-1,-1,G_IMAGE,0x405,0x10,&BBOB2,2,4,10,6},
/*2*/	{3,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[8],15,1,4,1},
/*3*/	{4,-1,-1,G_TEXT,0x400,0x10,&rs_tedinfo[9],4,1,29,2},
/*4*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[10],13,5,22,1},
/*5*/	{0,-1,-1,G_TEXT,0x420,0x0,&rs_tedinfo[11],15,7,13,1},
};

OBJECT JIMS_HEAD_R_H[] = {
/*0*/	{-1,1,7,G_BOX,0x400,0x0,(void *)0x21100,1539,1028,38,11},
/*1*/	{2,-1,-1,G_IMAGE,0x5,0x10,&BBOB3,2,4,10,6},
/*2*/	{3,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[12],15,1,4,1},
/*3*/	{4,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[13],14,5,22,1},
/*4*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[14],16,7,13,1},
/*5*/	{7,6,6,G_BOX,0x400,0x30,(void *)0xff1100,3,1,32,2},
/*6*/	{5,-1,-1,G_TEXT,0x400,0x0,&rs_tedinfo[15],3,0,27,2},
/*7*/	{0,-1,-1,G_BUTTON,0x625,0x0," Play Mod ", 20,9,12,1},
};



static void fix_tree(OBJECT *s,int max)
{
	int i;

	for(i=0; i<=max; i++)
		rsrc_obfix(s,i);
}

void rsrc_init(void)
{
	fix_tree(JIMS_HEAD_M_L,5);
	fix_tree(JIMS_HEAD_R_L,7);
	fix_tree(JIMS_HEAD_M_H,5);
	fix_tree(JIMS_HEAD_R_H,7);
}
