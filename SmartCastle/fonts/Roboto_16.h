// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Roboto_16[] PROGMEM = {
	0x0F, // Width: 15
	0x13, // Height: 19
	0x20, // First Char: 32
	0xE0, // Numbers of Chars: 224

	// Jump Table:
	0xFF, 0xFF, 0x00, 0x04,  // 32:65535
	0x00, 0x00, 0x08, 0x04,  // 33:0
	0x00, 0x08, 0x0A, 0x05,  // 34:8
	0x00, 0x12, 0x1C, 0x0A,  // 35:18
	0x00, 0x2E, 0x17, 0x09,  // 36:46
	0x00, 0x45, 0x20, 0x0C,  // 37:69
	0x00, 0x65, 0x1D, 0x0A,  // 38:101
	0x00, 0x82, 0x04, 0x03,  // 39:130
	0x00, 0x86, 0x0F, 0x05,  // 40:134
	0x00, 0x95, 0x0B, 0x06,  // 41:149
	0x00, 0xA0, 0x13, 0x07,  // 42:160
	0x00, 0xB3, 0x17, 0x09,  // 43:179
	0x00, 0xCA, 0x06, 0x03,  // 44:202
	0x00, 0xD0, 0x0B, 0x04,  // 45:208
	0x00, 0xDB, 0x08, 0x04,  // 46:219
	0x00, 0xE3, 0x10, 0x07,  // 47:227
	0x00, 0xF3, 0x17, 0x09,  // 48:243
	0x01, 0x0A, 0x11, 0x09,  // 49:266
	0x01, 0x1B, 0x17, 0x09,  // 50:283
	0x01, 0x32, 0x17, 0x09,  // 51:306
	0x01, 0x49, 0x1A, 0x09,  // 52:329
	0x01, 0x63, 0x17, 0x09,  // 53:355
	0x01, 0x7A, 0x17, 0x09,  // 54:378
	0x01, 0x91, 0x16, 0x09,  // 55:401
	0x01, 0xA7, 0x17, 0x09,  // 56:423
	0x01, 0xBE, 0x17, 0x09,  // 57:446
	0x01, 0xD5, 0x08, 0x04,  // 58:469
	0x01, 0xDD, 0x07, 0x03,  // 59:477
	0x01, 0xE4, 0x14, 0x08,  // 60:484
	0x01, 0xF8, 0x17, 0x09,  // 61:504
	0x02, 0x0F, 0x17, 0x08,  // 62:527
	0x02, 0x26, 0x13, 0x08,  // 63:550
	0x02, 0x39, 0x29, 0x0E,  // 64:569
	0x02, 0x62, 0x1D, 0x0A,  // 65:610
	0x02, 0x7F, 0x1A, 0x0A,  // 66:639
	0x02, 0x99, 0x1D, 0x0A,  // 67:665
	0x02, 0xB6, 0x1D, 0x0B,  // 68:694
	0x02, 0xD3, 0x1A, 0x09,  // 69:723
	0x02, 0xED, 0x17, 0x09,  // 70:749
	0x03, 0x04, 0x1D, 0x0B,  // 71:772
	0x03, 0x21, 0x1D, 0x0B,  // 72:801
	0x03, 0x3E, 0x08, 0x04,  // 73:830
	0x03, 0x46, 0x17, 0x09,  // 74:838
	0x03, 0x5D, 0x1D, 0x0A,  // 75:861
	0x03, 0x7A, 0x17, 0x09,  // 76:890
	0x03, 0x91, 0x26, 0x0E,  // 77:913
	0x03, 0xB7, 0x1D, 0x0B,  // 78:951
	0x03, 0xD4, 0x1D, 0x0B,  // 79:980
	0x03, 0xF1, 0x1A, 0x0A,  // 80:1009
	0x04, 0x0B, 0x1D, 0x0B,  // 81:1035
	0x04, 0x28, 0x1D, 0x0A,  // 82:1064
	0x04, 0x45, 0x1A, 0x0A,  // 83:1093
	0x04, 0x5F, 0x19, 0x0A,  // 84:1119
	0x04, 0x78, 0x1A, 0x0A,  // 85:1144
	0x04, 0x92, 0x1C, 0x0A,  // 86:1170
	0x04, 0xAE, 0x28, 0x0E,  // 87:1198
	0x04, 0xD6, 0x1D, 0x0A,  // 88:1238
	0x04, 0xF3, 0x19, 0x0A,  // 89:1267
	0x05, 0x0C, 0x1A, 0x0A,  // 90:1292
	0x05, 0x26, 0x0C, 0x04,  // 91:1318
	0x05, 0x32, 0x11, 0x07,  // 92:1330
	0x05, 0x43, 0x09, 0x04,  // 93:1347
	0x05, 0x4C, 0x11, 0x07,  // 94:1356
	0x05, 0x5D, 0x14, 0x07,  // 95:1373
	0x05, 0x71, 0x0A, 0x05,  // 96:1393
	0x05, 0x7B, 0x17, 0x09,  // 97:1403
	0x05, 0x92, 0x17, 0x09,  // 98:1426
	0x05, 0xA9, 0x17, 0x08,  // 99:1449
	0x05, 0xC0, 0x17, 0x09,  // 100:1472
	0x05, 0xD7, 0x17, 0x08,  // 101:1495
	0x05, 0xEE, 0x10, 0x06,  // 102:1518
	0x05, 0xFE, 0x17, 0x09,  // 103:1534
	0x06, 0x15, 0x17, 0x09,  // 104:1557
	0x06, 0x2C, 0x08, 0x04,  // 105:1580
	0x06, 0x34, 0x09, 0x04,  // 106:1588
	0x06, 0x3D, 0x17, 0x08,  // 107:1597
	0x06, 0x54, 0x08, 0x04,  // 108:1620
	0x06, 0x5C, 0x26, 0x0E,  // 109:1628
	0x06, 0x82, 0x17, 0x09,  // 110:1666
	0x06, 0x99, 0x17, 0x09,  // 111:1689
	0x06, 0xB0, 0x17, 0x09,  // 112:1712
	0x06, 0xC7, 0x18, 0x09,  // 113:1735
	0x06, 0xDF, 0x0D, 0x05,  // 114:1759
	0x06, 0xEC, 0x14, 0x08,  // 115:1772
	0x07, 0x00, 0x0E, 0x05,  // 116:1792
	0x07, 0x0E, 0x17, 0x09,  // 117:1806
	0x07, 0x25, 0x16, 0x08,  // 118:1829
	0x07, 0x3B, 0x22, 0x0C,  // 119:1851
	0x07, 0x5D, 0x17, 0x08,  // 120:1885
	0x07, 0x74, 0x16, 0x08,  // 121:1908
	0x07, 0x8A, 0x14, 0x08,  // 122:1930
	0x07, 0x9E, 0x0F, 0x05,  // 123:1950
	0x07, 0xAD, 0x09, 0x04,  // 124:1965
	0x07, 0xB6, 0x0E, 0x05,  // 125:1974
	0x07, 0xC4, 0x1D, 0x0B,  // 126:1988
	0x07, 0xE1, 0x11, 0x07,  // 127:2017
	0x07, 0xF2, 0x11, 0x07,  // 128:2034
	0x08, 0x03, 0x11, 0x07,  // 129:2051
	0x08, 0x14, 0x11, 0x07,  // 130:2068
	0x08, 0x25, 0x11, 0x07,  // 131:2085
	0x08, 0x36, 0x11, 0x07,  // 132:2102
	0x08, 0x47, 0x11, 0x07,  // 133:2119
	0x08, 0x58, 0x11, 0x07,  // 134:2136
	0x08, 0x69, 0x11, 0x07,  // 135:2153
	0x08, 0x7A, 0x11, 0x07,  // 136:2170
	0x08, 0x8B, 0x11, 0x07,  // 137:2187
	0x08, 0x9C, 0x11, 0x07,  // 138:2204
	0x08, 0xAD, 0x11, 0x07,  // 139:2221
	0x08, 0xBE, 0x11, 0x07,  // 140:2238
	0x08, 0xCF, 0x11, 0x07,  // 141:2255
	0x08, 0xE0, 0x11, 0x07,  // 142:2272
	0x08, 0xF1, 0x11, 0x07,  // 143:2289
	0x09, 0x02, 0x11, 0x07,  // 144:2306
	0x09, 0x13, 0x11, 0x07,  // 145:2323
	0x09, 0x24, 0x11, 0x07,  // 146:2340
	0x09, 0x35, 0x11, 0x07,  // 147:2357
	0x09, 0x46, 0x11, 0x07,  // 148:2374
	0x09, 0x57, 0x11, 0x07,  // 149:2391
	0x09, 0x68, 0x11, 0x07,  // 150:2408
	0x09, 0x79, 0x11, 0x07,  // 151:2425
	0x09, 0x8A, 0x11, 0x07,  // 152:2442
	0x09, 0x9B, 0x11, 0x07,  // 153:2459
	0x09, 0xAC, 0x11, 0x07,  // 154:2476
	0x09, 0xBD, 0x11, 0x07,  // 155:2493
	0x09, 0xCE, 0x11, 0x07,  // 156:2510
	0x09, 0xDF, 0x11, 0x07,  // 157:2527
	0x09, 0xF0, 0x11, 0x07,  // 158:2544
	0x0A, 0x01, 0x11, 0x07,  // 159:2561
	0xFF, 0xFF, 0x00, 0x04,  // 160:65535
	0x0A, 0x12, 0x09, 0x04,  // 161:2578
	0x0A, 0x1B, 0x17, 0x09,  // 162:2587
	0x0A, 0x32, 0x1A, 0x09,  // 163:2610
	0x0A, 0x4C, 0x20, 0x0B,  // 164:2636
	0x0A, 0x6C, 0x16, 0x08,  // 165:2668
	0x0A, 0x82, 0x09, 0x04,  // 166:2690
	0x0A, 0x8B, 0x1B, 0x0A,  // 167:2699
	0x0A, 0xA6, 0x10, 0x07,  // 168:2726
	0x0A, 0xB6, 0x23, 0x0D,  // 169:2742
	0x0A, 0xD9, 0x11, 0x07,  // 170:2777
	0x0A, 0xEA, 0x14, 0x08,  // 171:2794
	0x0A, 0xFE, 0x14, 0x09,  // 172:2814
	0x0B, 0x12, 0x0B, 0x04,  // 173:2834
	0x0B, 0x1D, 0x23, 0x0D,  // 174:2845
	0x0B, 0x40, 0x10, 0x07,  // 175:2880
	0x0B, 0x50, 0x0D, 0x06,  // 176:2896
	0x0B, 0x5D, 0x17, 0x09,  // 177:2909
	0x0B, 0x74, 0x0E, 0x06,  // 178:2932
	0x0B, 0x82, 0x0E, 0x06,  // 179:2946
	0x0B, 0x90, 0x0A, 0x05,  // 180:2960
	0x0B, 0x9A, 0x17, 0x09,  // 181:2970
	0x0B, 0xB1, 0x14, 0x08,  // 182:2993
	0x0B, 0xC5, 0x08, 0x04,  // 183:3013
	0x0B, 0xCD, 0x09, 0x04,  // 184:3021
	0x0B, 0xD6, 0x0B, 0x06,  // 185:3030
	0x0B, 0xE1, 0x10, 0x07,  // 186:3041
	0x0B, 0xF1, 0x14, 0x08,  // 187:3057
	0x0C, 0x05, 0x20, 0x0C,  // 188:3077
	0x0C, 0x25, 0x20, 0x0C,  // 189:3109
	0x0C, 0x45, 0x20, 0x0C,  // 190:3141
	0x0C, 0x65, 0x15, 0x08,  // 191:3173
	0x0C, 0x7A, 0x1D, 0x0A,  // 192:3194
	0x0C, 0x97, 0x1D, 0x0A,  // 193:3223
	0x0C, 0xB4, 0x1D, 0x0A,  // 194:3252
	0x0C, 0xD1, 0x1D, 0x0A,  // 195:3281
	0x0C, 0xEE, 0x1D, 0x0A,  // 196:3310
	0x0D, 0x0B, 0x1D, 0x0A,  // 197:3339
	0x0D, 0x28, 0x2C, 0x0F,  // 198:3368
	0x0D, 0x54, 0x1D, 0x0A,  // 199:3412
	0x0D, 0x71, 0x1A, 0x09,  // 200:3441
	0x0D, 0x8B, 0x1A, 0x09,  // 201:3467
	0x0D, 0xA5, 0x1A, 0x09,  // 202:3493
	0x0D, 0xBF, 0x1A, 0x09,  // 203:3519
	0x0D, 0xD9, 0x08, 0x04,  // 204:3545
	0x0D, 0xE1, 0x0A, 0x04,  // 205:3553
	0x0D, 0xEB, 0x0A, 0x04,  // 206:3563
	0x0D, 0xF5, 0x0A, 0x04,  // 207:3573
	0x0D, 0xFF, 0x1D, 0x0B,  // 208:3583
	0x0E, 0x1C, 0x1D, 0x0B,  // 209:3612
	0x0E, 0x39, 0x1D, 0x0B,  // 210:3641
	0x0E, 0x56, 0x1D, 0x0B,  // 211:3670
	0x0E, 0x73, 0x1D, 0x0B,  // 212:3699
	0x0E, 0x90, 0x1D, 0x0B,  // 213:3728
	0x0E, 0xAD, 0x1D, 0x0B,  // 214:3757
	0x0E, 0xCA, 0x17, 0x09,  // 215:3786
	0x0E, 0xE1, 0x1D, 0x0B,  // 216:3809
	0x0E, 0xFE, 0x1A, 0x0A,  // 217:3838
	0x0F, 0x18, 0x1A, 0x0A,  // 218:3864
	0x0F, 0x32, 0x1A, 0x0A,  // 219:3890
	0x0F, 0x4C, 0x1A, 0x0A,  // 220:3916
	0x0F, 0x66, 0x19, 0x0A,  // 221:3942
	0x0F, 0x7F, 0x1A, 0x09,  // 222:3967
	0x0F, 0x99, 0x1A, 0x0A,  // 223:3993
	0x0F, 0xB3, 0x17, 0x09,  // 224:4019
	0x0F, 0xCA, 0x17, 0x09,  // 225:4042
	0x0F, 0xE1, 0x17, 0x09,  // 226:4065
	0x0F, 0xF8, 0x17, 0x09,  // 227:4088
	0x10, 0x0F, 0x17, 0x09,  // 228:4111
	0x10, 0x26, 0x17, 0x09,  // 229:4134
	0x10, 0x3D, 0x26, 0x0E,  // 230:4157
	0x10, 0x63, 0x17, 0x08,  // 231:4195
	0x10, 0x7A, 0x17, 0x08,  // 232:4218
	0x10, 0x91, 0x17, 0x08,  // 233:4241
	0x10, 0xA8, 0x17, 0x08,  // 234:4264
	0x10, 0xBF, 0x17, 0x08,  // 235:4287
	0x10, 0xD6, 0x08, 0x04,  // 236:4310
	0x10, 0xDE, 0x0A, 0x04,  // 237:4318
	0x10, 0xE8, 0x0A, 0x04,  // 238:4328
	0x10, 0xF2, 0x0A, 0x04,  // 239:4338
	0x10, 0xFC, 0x17, 0x09,  // 240:4348
	0x11, 0x13, 0x17, 0x09,  // 241:4371
	0x11, 0x2A, 0x17, 0x09,  // 242:4394
	0x11, 0x41, 0x17, 0x09,  // 243:4417
	0x11, 0x58, 0x17, 0x09,  // 244:4440
	0x11, 0x6F, 0x17, 0x09,  // 245:4463
	0x11, 0x86, 0x17, 0x09,  // 246:4486
	0x11, 0x9D, 0x17, 0x09,  // 247:4509
	0x11, 0xB4, 0x17, 0x09,  // 248:4532
	0x11, 0xCB, 0x17, 0x09,  // 249:4555
	0x11, 0xE2, 0x17, 0x09,  // 250:4578
	0x11, 0xF9, 0x17, 0x09,  // 251:4601
	0x12, 0x10, 0x17, 0x09,  // 252:4624
	0x12, 0x27, 0x16, 0x08,  // 253:4647
	0x12, 0x3D, 0x17, 0x09,  // 254:4669
	0x12, 0x54, 0x16, 0x08,  // 255:4692

	// Font Data:
	0x00,0x00,0x00,0xF8,0x67,0x00,0xF8,0x67,	// 33
	0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x78,	// 34
	0x00,0x00,0x00,0x00,0x04,0x00,0x40,0x64,0x00,0xC0,0x3F,0x00,0xF8,0x04,0x00,0x48,0x44,0x00,0x40,0x7F,0x00,0xF8,0x05,0x00,0x48,0x04,0x00,0x40,	// 35
	0x00,0x00,0x00,0x60,0x30,0x00,0xF0,0x61,0x00,0x08,0x41,0x00,0x0E,0xC3,0x00,0x08,0x42,0x00,0x38,0x66,0x00,0x70,0x3C,	// 36
	0x00,0x00,0x00,0x70,0x00,0x00,0x88,0x00,0x00,0x88,0x30,0x00,0xF8,0x0C,0x00,0x00,0x06,0x00,0x80,0x39,0x00,0x60,0x6C,0x00,0x10,0x44,0x00,0x00,0x44,0x00,0x00,0x38,	// 37
	0x00,0x00,0x00,0x00,0x3C,0x00,0xF0,0x66,0x00,0x98,0x43,0x00,0x88,0x43,0x00,0x88,0x4E,0x00,0x70,0x78,0x00,0x00,0x30,0x00,0x00,0x7E,0x00,0x00,0x40,	// 38
	0x00,0x00,0x00,0x78,	// 39
	0x00,0x00,0x00,0x80,0x3F,0x00,0xE0,0xFB,0x00,0x18,0x00,0x03,0x0C,0x00,0x06,	// 40
	0x04,0x00,0x04,0x08,0x00,0x02,0x70,0xC0,0x01,0xC0,0x7F,	// 41
	0x20,0x00,0x00,0x20,0x01,0x00,0xA0,0x00,0x00,0x78,0x00,0x00,0xA0,0x01,0x00,0x20,0x01,0x00,0x20,	// 42
	0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0xC0,0x3F,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,	// 43
	0x00,0x00,0x01,0x00,0xE0,0x01,	// 44
	0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x04,	// 45
	0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x60,	// 46
	0x00,0x80,0x00,0x00,0xF0,0x00,0x00,0x1C,0x00,0x80,0x03,0x00,0x70,0x00,0x00,0x18,	// 47
	0x00,0x00,0x00,0xE0,0x1F,0x00,0x30,0x30,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x30,0x30,0x00,0xE0,0x1F,	// 48
	0x00,0x00,0x00,0x20,0x00,0x00,0x30,0x00,0x00,0x10,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 49
	0x00,0x00,0x00,0x70,0x60,0x00,0x18,0x70,0x00,0x08,0x58,0x00,0x08,0x4C,0x00,0x08,0x46,0x00,0xF0,0x43,0x00,0xE0,0x40,	// 50
	0x00,0x00,0x00,0x30,0x30,0x00,0x18,0x60,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0xF0,0x3E,0x00,0x60,0x1C,	// 51
	0x00,0x08,0x00,0x00,0x0C,0x00,0x00,0x0B,0x00,0xC0,0x09,0x00,0x60,0x08,0x00,0x18,0x08,0x00,0xF8,0x7F,0x00,0x00,0x08,0x00,0x00,0x08,	// 52
	0x00,0x00,0x00,0x00,0x10,0x00,0xF8,0x31,0x00,0x88,0x40,0x00,0x88,0x40,0x00,0x88,0x40,0x00,0x88,0x61,0x00,0x08,0x3F,	// 53
	0x00,0x00,0x00,0xC0,0x1F,0x00,0xE0,0x3B,0x00,0x90,0x60,0x00,0x88,0x40,0x00,0x88,0x40,0x00,0x88,0x61,0x00,0x00,0x3F,	// 54
	0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x40,0x00,0x08,0x70,0x00,0x08,0x1E,0x00,0x88,0x03,0x00,0xE8,0x00,0x00,0x38,	// 55
	0x00,0x00,0x00,0x60,0x3C,0x00,0xF8,0x62,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0xF0,0x62,0x00,0x60,0x3C,	// 56
	0x00,0x00,0x00,0xF0,0x03,0x00,0x18,0x46,0x00,0x08,0x44,0x00,0x08,0x44,0x00,0x18,0x24,0x00,0xF0,0x3F,0x00,0xE0,0x0F,	// 57
	0x00,0x00,0x00,0xC0,0x60,0x00,0xC0,0x60,	// 58
	0x00,0x00,0x01,0xC0,0xE0,0x01,0xC0,	// 59
	0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x05,0x00,0x00,0x05,0x00,0x80,0x0D,0x00,0x80,0x08,0x00,0xC0,0x18,	// 60
	0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x09,0x00,0x00,0x09,0x00,0x00,0x09,0x00,0x00,0x09,0x00,0x00,0x09,0x00,0x00,0x09,	// 61
	0x00,0x00,0x00,0xC0,0x18,0x00,0x80,0x08,0x00,0x80,0x08,0x00,0x00,0x05,0x00,0x00,0x05,0x00,0x00,0x07,0x00,0x00,0x02,	// 62
	0x00,0x00,0x00,0x30,0x00,0x00,0x18,0x00,0x00,0x08,0x66,0x00,0x08,0x03,0x00,0xF8,0x01,0x00,0xF0,	// 63
	0x00,0x00,0x00,0x00,0x7F,0x00,0xC0,0xC1,0x00,0x60,0x00,0x01,0x20,0x3C,0x03,0x10,0x7F,0x02,0x90,0x41,0x02,0x90,0x40,0x02,0x90,0x38,0x02,0x10,0x7F,0x01,0x20,0x40,0x00,0x60,0x40,0x00,0xC0,0x31,0x00,0x00,0x0E,	// 64
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE0,0x0B,0x00,0x78,0x08,0x00,0x78,0x08,0x00,0xE0,0x0B,0x00,0x00,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 65
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0xF0,0x36,0x00,0x60,0x3E,	// 66
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x18,0x60,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x18,0x60,0x00,0x70,0x38,0x00,0x40,0x08,	// 67
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x18,0x60,0x00,0x30,0x30,0x00,0xE0,0x1F,0x00,0x00,0x03,	// 68
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x00,0x40,	// 69
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,	// 70
	0x00,0x00,0x00,0xC0,0x0F,0x00,0xF0,0x3C,0x00,0x18,0x20,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x42,0x00,0x18,0x42,0x00,0x70,0x3E,0x00,0x40,0x3E,	// 71
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0xF8,0x7F,	// 72
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 73
	0x00,0x10,0x00,0x00,0x30,0x00,0x00,0x60,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x60,0x00,0xF8,0x3F,0x00,0xF8,0x0F,	// 74
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x00,0x03,0x00,0x80,0x01,0x00,0xC0,0x06,0x00,0x60,0x0C,0x00,0x38,0x38,0x00,0x08,0x60,0x00,0x08,0x40,	// 75
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,	// 76
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x70,0x00,0x00,0xC0,0x03,0x00,0x00,0x1E,0x00,0x00,0x70,0x00,0x00,0x78,0x00,0x00,0x1E,0x00,0xC0,0x03,0x00,0x70,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 77
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x60,0x00,0x00,0xC0,0x01,0x00,0x00,0x07,0x00,0x00,0x0E,0x00,0x00,0x38,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 78
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x18,0x60,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x18,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 79
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x18,0x03,0x00,0xF0,0x01,	// 80
	0x00,0x00,0x00,0xC0,0x1F,0x00,0x70,0x38,0x00,0x18,0x60,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x18,0xE0,0x00,0xF0,0xBC,0x01,0xC0,0x8F,	// 81
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x0E,0x00,0xB8,0x39,0x00,0xF0,0x61,0x00,0x00,0x40,	// 82
	0x00,0x00,0x00,0x60,0x10,0x00,0xF0,0x31,0x00,0x08,0x41,0x00,0x08,0x43,0x00,0x08,0x42,0x00,0x08,0x42,0x00,0x30,0x64,0x00,0x20,0x3C,	// 83
	0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x08,	// 84
	0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0x30,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x30,0x00,0xF8,0x1F,	// 85
	0x08,0x00,0x00,0x78,0x00,0x00,0xE0,0x03,0x00,0x00,0x1F,0x00,0x00,0x78,0x00,0x00,0x78,0x00,0x00,0x1F,0x00,0xE0,0x03,0x00,0x78,0x00,0x00,0x08,	// 86
	0x08,0x00,0x00,0x78,0x00,0x00,0xE0,0x0F,0x00,0x00,0x7C,0x00,0x00,0x78,0x00,0x80,0x0F,0x00,0xF8,0x00,0x00,0x78,0x00,0x00,0xC0,0x03,0x00,0x00,0x3E,0x00,0x00,0x70,0x00,0x80,0x1F,0x00,0xF8,0x01,0x00,0x18,	// 87
	0x00,0x40,0x00,0x18,0x60,0x00,0x38,0x70,0x00,0xE0,0x1C,0x00,0x80,0x07,0x00,0x80,0x07,0x00,0xE0,0x1C,0x00,0x38,0x30,0x00,0x18,0x60,0x00,0x08,0x40,	// 88
	0x08,0x00,0x00,0x38,0x00,0x00,0xF0,0x00,0x00,0xC0,0x01,0x00,0x00,0x7F,0x00,0x00,0x7F,0x00,0xC0,0x01,0x00,0x70,0x00,0x00,0x18,	// 89
	0x00,0x00,0x00,0x08,0x60,0x00,0x08,0x78,0x00,0x08,0x4C,0x00,0x08,0x47,0x00,0xC8,0x41,0x00,0x68,0x40,0x00,0x38,0x40,0x00,0x08,0x40,	// 90
	0x00,0x00,0x00,0xFC,0xFF,0x01,0xFC,0xFF,0x01,0x04,0x00,0x01,	// 91
	0x08,0x00,0x00,0x38,0x00,0x00,0xE0,0x01,0x00,0x00,0x0F,0x00,0x00,0x38,0x00,0x00,0xE0,	// 92
	0x04,0x00,0x01,0x04,0x00,0x01,0xFC,0xFF,0x01,	// 93
	0x00,0x00,0x00,0x80,0x01,0x00,0xF0,0x00,0x00,0x18,0x00,0x00,0xF0,0x00,0x00,0x80,0x01,	// 94
	0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x80,	// 95
	0x08,0x00,0x00,0x08,0x00,0x00,0x18,0x00,0x00,0x10,	// 96
	0x00,0x00,0x00,0x00,0x39,0x00,0x80,0x6D,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0x80,0x7F,0x00,0x00,0x7F,	// 97
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x3F,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0xC0,0x60,0x00,0x80,0x3F,	// 98
	0x00,0x00,0x00,0x80,0x3F,0x00,0xC0,0x60,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x80,0x31,0x00,0x00,0x11,	// 99
	0x00,0x00,0x00,0x80,0x3F,0x00,0xC0,0x60,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0xF8,0x3F,0x00,0xF8,0x7F,	// 100
	0x00,0x04,0x00,0x80,0x3F,0x00,0xC0,0x64,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0xC0,0x44,0x00,0x80,0x27,0x00,0x00,0x26,	// 101
	0x00,0x00,0x00,0x40,0x00,0x00,0xF0,0x7F,0x00,0xF8,0x7F,0x00,0x48,0x00,0x00,0x08,	// 102
	0x00,0x00,0x00,0x80,0x3F,0x01,0xC0,0x61,0x03,0x40,0x40,0x02,0x40,0x40,0x02,0x40,0x40,0x02,0x80,0xFF,0x01,0xC0,0xFF,	// 103
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0xC0,0x7F,0x00,0x80,0x7F,	// 104
	0x00,0x00,0x00,0xD8,0x7F,0x00,0xD8,0x7F,	// 105
	0x00,0x00,0x02,0xD8,0xFF,0x03,0xD8,0xFF,0x01,	// 106
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x00,0x06,0x00,0x00,0x0F,0x00,0x80,0x39,0x00,0x40,0x60,0x00,0x40,0x40,	// 107
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 108
	0x00,0x00,0x00,0xC0,0x7F,0x00,0x80,0x7F,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0xC0,0x7F,0x00,0x80,0x7F,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0xC0,0x7F,0x00,0x80,0x7F,	// 109
	0x00,0x00,0x00,0xC0,0x7F,0x00,0xC0,0x7F,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0xC0,0x7F,0x00,0x80,0x7F,	// 110
	0x00,0x00,0x00,0x00,0x1F,0x00,0x80,0x20,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x80,0x20,0x00,0x00,0x3F,	// 111
	0x00,0x00,0x00,0xC0,0xFF,0x03,0x80,0xFF,0x03,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0xC0,0x71,0x00,0x80,0x3F,	// 112
	0x00,0x00,0x00,0x80,0x3F,0x00,0xC0,0x60,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0xC0,0xFF,0x03,0xC0,0xFF,0x03,	// 113
	0x00,0x00,0x00,0xC0,0x7F,0x00,0x80,0x00,0x00,0x40,0x00,0x00,0x40,	// 114
	0x00,0x00,0x00,0x80,0x33,0x00,0xC0,0x66,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0xC0,0x4C,0x00,0x80,0x39,	// 115
	0x40,0x00,0x00,0xF0,0x1F,0x00,0xF0,0x7F,0x00,0x40,0x40,0x00,0x40,0x40,	// 116
	0x00,0x00,0x00,0xC0,0x3F,0x00,0xC0,0x7F,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0xC0,0x7F,0x00,0xC0,0x7F,	// 117
	0x40,0x00,0x00,0xC0,0x01,0x00,0x00,0x0F,0x00,0x00,0x78,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xC0,0x01,0x00,0x40,	// 118
	0x40,0x00,0x00,0xC0,0x03,0x00,0x00,0x3E,0x00,0x00,0x70,0x00,0x00,0x1E,0x00,0xC0,0x03,0x00,0xC0,0x03,0x00,0x00,0x1E,0x00,0x00,0x60,0x00,0x00,0x3E,0x00,0xC0,0x03,0x00,0x40,	// 119
	0x40,0x40,0x00,0xC0,0x60,0x00,0x80,0x31,0x00,0x00,0x0E,0x00,0x00,0x1F,0x00,0x80,0x31,0x00,0xC0,0x60,0x00,0x40,0x40,	// 120
	0x40,0x00,0x00,0xC0,0x01,0x02,0x00,0x0F,0x02,0x00,0xF8,0x01,0x00,0x78,0x00,0x00,0x0F,0x00,0xC0,0x01,0x00,0x40,	// 121
	0x00,0x00,0x00,0x40,0x60,0x00,0x40,0x78,0x00,0x40,0x4C,0x00,0x40,0x47,0x00,0xC0,0x41,0x00,0xC0,0x40,	// 122
	0x00,0x04,0x00,0x00,0x04,0x00,0xE0,0xFB,0x00,0x30,0x80,0x01,0x08,0x00,0x02,	// 123
	0x00,0x00,0x00,0xF8,0xFF,0x01,0xF8,0xFF,0x01,	// 124
	0x08,0x00,0x02,0x30,0x80,0x01,0xE0,0xFB,0x00,0x00,0x04,0x00,0x00,0x04,	// 125
	0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x06,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x04,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x0C,0x00,0x00,0x06,	// 126
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 127
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 128
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 129
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 130
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 131
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 132
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 133
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 134
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 135
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 136
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 137
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 138
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 139
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 140
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 141
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 142
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 143
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 144
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 145
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 146
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 147
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 148
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 149
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 150
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 151
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 152
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 153
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 154
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 155
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 156
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 157
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 158
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xC8,0x4C,0x00,0x88,0x47,0x00,0x68,0x58,0x00,0xF8,0x7F,	// 159
	0x00,0x00,0x00,0xC0,0xFC,0x03,0xC0,0xFC,0x03,	// 161
	0x00,0x00,0x00,0x00,0x1F,0x00,0x80,0x31,0x00,0x40,0x40,0x00,0x70,0xC0,0x01,0x40,0x40,0x00,0xC0,0x60,0x00,0x80,0x31,	// 162
	0x00,0x00,0x00,0x00,0x42,0x00,0xF0,0x7F,0x00,0x18,0x42,0x00,0x08,0x42,0x00,0x08,0x42,0x00,0x18,0x40,0x00,0x30,0x40,0x00,0x00,0x40,	// 163
	0x00,0x00,0x00,0xE0,0x7F,0x00,0xC0,0x30,0x00,0x60,0x60,0x00,0x20,0x40,0x00,0x20,0x40,0x00,0x20,0x40,0x00,0x60,0x60,0x00,0xC0,0x30,0x00,0xE0,0x7F,0x00,0x40,0x40,	// 164
	0x08,0x00,0x00,0x38,0x0A,0x00,0xE0,0x0A,0x00,0x80,0x7F,0x00,0x80,0x7F,0x00,0xE0,0x0A,0x00,0x38,0x0A,0x00,0x08,	// 165
	0x00,0x00,0x00,0xF8,0xFB,0x01,0xF8,0xFB,0x01,	// 166
	0x00,0x00,0x00,0x60,0x0E,0x03,0xF0,0x13,0x06,0x88,0x31,0x04,0x08,0x21,0x04,0x08,0x21,0x04,0x08,0x63,0x04,0x30,0xF6,0x03,0x20,0x9C,0x01,	// 167
	0x00,0x00,0x00,0x18,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,	// 168
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x20,0x10,0x00,0x10,0x20,0x00,0xC8,0x4F,0x00,0x28,0x50,0x00,0x28,0x50,0x00,0x28,0x50,0x00,0xC8,0x4C,0x00,0x10,0x20,0x00,0x20,0x18,0x00,0xC0,0x0F,	// 169
	0x00,0x00,0x00,0xD0,0x00,0x00,0x68,0x01,0x00,0x28,0x01,0x00,0xA8,0x00,0x00,0xF0,0x01,	// 170
	0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x1E,0x00,0x00,0x21,0x00,0x00,0x0C,0x00,0x00,0x1E,0x00,0x00,0x21,	// 171
	0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x0E,	// 172
	0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x04,	// 173
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x20,0x10,0x00,0x10,0x20,0x00,0xE8,0x4F,0x00,0x28,0x41,0x00,0x28,0x41,0x00,0x28,0x41,0x00,0xC8,0x6E,0x00,0x10,0x20,0x00,0x60,0x18,0x00,0xC0,0x0F,	// 174
	0x00,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,0x00,0x00,0x10,	// 175
	0x00,0x00,0x00,0x30,0x00,0x00,0x48,0x00,0x00,0x48,0x00,0x00,0x30,	// 176
	0x00,0x00,0x00,0x00,0x41,0x00,0x00,0x41,0x00,0x00,0x41,0x00,0xE0,0x5F,0x00,0x00,0x41,0x00,0x00,0x41,0x00,0x00,0x41,	// 177
	0x00,0x00,0x00,0x18,0x03,0x00,0x88,0x03,0x00,0xC8,0x02,0x00,0x70,0x02,	// 178
	0x00,0x00,0x00,0x18,0x03,0x00,0x28,0x02,0x00,0x28,0x02,0x00,0xD8,0x01,	// 179
	0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x00,0x00,0x08,	// 180
	0x00,0x00,0x00,0xC0,0xFF,0x03,0xC0,0xFF,0x03,0x00,0x40,0x00,0x00,0x40,0x00,0x00,0x40,0x00,0xC0,0x3F,0x00,0xC0,0x7F,	// 181
	0x00,0x00,0x00,0xF0,0x03,0x00,0xF8,0x03,0x00,0xF8,0x07,0x00,0xF8,0x07,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 182
	0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x03,	// 183
	0x00,0x00,0x00,0x00,0x80,0x02,0x00,0x80,0x03,	// 184
	0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x00,0x00,0xF8,0x03,	// 185
	0x00,0x00,0x00,0xF0,0x00,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0xF0,	// 186
	0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x1E,0x00,0x00,0x0C,0x00,0x00,0x21,0x00,0x00,0x1E,0x00,0x00,0x0C,	// 187
	0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x20,0x00,0xF8,0x31,0x00,0x00,0x0C,0x00,0x00,0x06,0x00,0x80,0x21,0x00,0xC0,0x30,0x00,0x20,0x2C,0x00,0x00,0x7E,0x00,0x00,0x7E,	// 188
	0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x20,0x00,0xF8,0x31,0x00,0x00,0x0C,0x00,0x00,0x06,0x00,0x80,0x01,0x00,0xC0,0x66,0x00,0x20,0x62,0x00,0x00,0x52,0x00,0x00,0x4E,	// 189
	0x80,0x00,0x00,0x88,0x01,0x00,0x28,0x01,0x00,0x28,0x21,0x00,0xD8,0x30,0x00,0x00,0x0C,0x00,0x00,0x26,0x00,0x80,0x31,0x00,0xC0,0x2C,0x00,0x20,0x7E,0x00,0x00,0x7E,	// 190
	0x00,0x00,0x00,0x00,0xE0,0x01,0x00,0x30,0x03,0xC0,0x0E,0x02,0xC0,0x06,0x02,0x00,0x00,0x03,0x00,0x80,0x01,	// 191
	0x00,0x40,0x00,0x00,0x78,0x00,0x01,0x0F,0x00,0xE1,0x0B,0x00,0x7B,0x08,0x00,0x7A,0x08,0x00,0xE0,0x0B,0x00,0x00,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 192
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE0,0x0B,0x00,0x78,0x08,0x00,0x7A,0x08,0x00,0xE3,0x0B,0x00,0x01,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 193
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE2,0x0B,0x00,0x7A,0x08,0x00,0x79,0x08,0x00,0xE3,0x0B,0x00,0x02,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 194
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE3,0x0B,0x00,0x79,0x08,0x00,0x79,0x08,0x00,0xE2,0x0B,0x00,0x03,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 195
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE3,0x0B,0x00,0x7A,0x08,0x00,0x78,0x08,0x00,0xE3,0x0B,0x00,0x03,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 196
	0x00,0x40,0x00,0x00,0x78,0x00,0x00,0x0F,0x00,0xE0,0x0B,0x00,0x7B,0x08,0x00,0x7A,0x08,0x00,0xE3,0x0B,0x00,0x00,0x0F,0x00,0x00,0x78,0x00,0x00,0x40,	// 197
	0x00,0x40,0x00,0x00,0x70,0x00,0x00,0x1C,0x00,0x00,0x0E,0x00,0x80,0x0B,0x00,0xE0,0x08,0x00,0x38,0x08,0x00,0xF8,0x08,0x00,0xF8,0x7F,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x00,0x40,	// 198
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x18,0x60,0x00,0x08,0x40,0x00,0x08,0xC0,0x02,0x08,0xC0,0x03,0x18,0x60,0x00,0x70,0x38,0x00,0x40,0x08,	// 199
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF9,0x7F,0x00,0x09,0x41,0x00,0x0B,0x41,0x00,0x0A,0x41,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x00,0x40,	// 200
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x08,0x41,0x00,0x0A,0x41,0x00,0x0B,0x41,0x00,0x09,0x41,0x00,0x08,0x41,0x00,0x00,0x40,	// 201
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xFA,0x7F,0x00,0x0A,0x41,0x00,0x09,0x41,0x00,0x0B,0x41,0x00,0x0A,0x41,0x00,0x08,0x41,0x00,0x00,0x40,	// 202
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x0B,0x41,0x00,0x0A,0x41,0x00,0x08,0x41,0x00,0x0B,0x41,0x00,0x0B,0x41,0x00,0x00,0x40,	// 203
	0x01,0x00,0x00,0xFB,0x7F,0x00,0xFA,0x7F,	// 204
	0x00,0x00,0x00,0xFA,0x7F,0x00,0xFB,0x7F,0x00,0x01,	// 205
	0x02,0x00,0x00,0xFA,0x7F,0x00,0xF9,0x7F,0x00,0x03,	// 206
	0x03,0x00,0x00,0xFA,0x7F,0x00,0xF8,0x7F,0x00,0x03,	// 207
	0x00,0x01,0x00,0x00,0x01,0x00,0xF8,0x7F,0x00,0x08,0x41,0x00,0x08,0x41,0x00,0x08,0x40,0x00,0x08,0x40,0x00,0x10,0x20,0x00,0xE0,0x1F,0x00,0x80,0x07,	// 208
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x63,0x00,0x00,0xC1,0x01,0x00,0x01,0x07,0x00,0x02,0x0E,0x00,0x03,0x38,0x00,0xF8,0x7F,0x00,0xF8,0x7F,	// 209
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x19,0x60,0x00,0x09,0x40,0x00,0x0B,0x40,0x00,0x0A,0x40,0x00,0x18,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 210
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x18,0x60,0x00,0x08,0x40,0x00,0x0A,0x40,0x00,0x0B,0x40,0x00,0x19,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 211
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x1A,0x60,0x00,0x0A,0x40,0x00,0x09,0x40,0x00,0x0B,0x40,0x00,0x1A,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 212
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x1B,0x60,0x00,0x09,0x40,0x00,0x09,0x40,0x00,0x0A,0x40,0x00,0x1B,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 213
	0x00,0x00,0x00,0xC0,0x0F,0x00,0x70,0x38,0x00,0x1B,0x60,0x00,0x0A,0x40,0x00,0x08,0x40,0x00,0x0B,0x40,0x00,0x1B,0x60,0x00,0x70,0x38,0x00,0xC0,0x0F,	// 214
	0x00,0x00,0x00,0xC0,0x18,0x00,0x80,0x0D,0x00,0x00,0x07,0x00,0x00,0x07,0x00,0x80,0x0D,0x00,0xC0,0x18,0x00,0x80,0x10,	// 215
	0x00,0x00,0x00,0xC0,0x8F,0x00,0x70,0x78,0x00,0x18,0x70,0x00,0x08,0x4C,0x00,0x08,0x42,0x00,0x88,0x41,0x00,0x78,0x60,0x00,0xF0,0x38,0x00,0xC8,0x0F,	// 216
	0x00,0x00,0x00,0xF8,0x1F,0x00,0x01,0x30,0x00,0x01,0x40,0x00,0x03,0x40,0x00,0x02,0x40,0x00,0x00,0x40,0x00,0x00,0x30,0x00,0xF8,0x1F,	// 217
	0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0x30,0x00,0x00,0x40,0x00,0x02,0x40,0x00,0x03,0x40,0x00,0x01,0x40,0x00,0x00,0x30,0x00,0xF8,0x1F,	// 218
	0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0x30,0x00,0x02,0x40,0x00,0x02,0x40,0x00,0x01,0x40,0x00,0x03,0x40,0x00,0x02,0x30,0x00,0xF8,0x1F,	// 219
	0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0x30,0x00,0x03,0x40,0x00,0x02,0x40,0x00,0x00,0x40,0x00,0x03,0x40,0x00,0x03,0x30,0x00,0xF8,0x1F,	// 220
	0x08,0x00,0x00,0x38,0x00,0x00,0xF0,0x00,0x00,0xC0,0x01,0x00,0x02,0x7F,0x00,0x03,0x7F,0x00,0xC1,0x01,0x00,0x70,0x00,0x00,0x18,	// 221
	0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0x7F,0x00,0x20,0x08,0x00,0x20,0x08,0x00,0x20,0x08,0x00,0x60,0x0C,0x00,0xC0,0x07,0x00,0x00,0x01,	// 222
	0x00,0x00,0x00,0xE0,0x7F,0x00,0xF0,0x7F,0x00,0x08,0x20,0x00,0x08,0x40,0x00,0x88,0x43,0x00,0xF0,0x46,0x00,0x00,0x6C,0x00,0x00,0x38,	// 223
	0x00,0x00,0x00,0x00,0x39,0x00,0x88,0x6D,0x00,0x48,0x44,0x00,0x58,0x44,0x00,0x50,0x44,0x00,0x80,0x7F,0x00,0x00,0x7F,	// 224
	0x00,0x00,0x00,0x00,0x39,0x00,0x80,0x6D,0x00,0x40,0x44,0x00,0x50,0x44,0x00,0x58,0x44,0x00,0x88,0x7F,0x00,0x00,0x7F,	// 225
	0x00,0x00,0x00,0x00,0x39,0x00,0x90,0x6D,0x00,0x50,0x44,0x00,0x48,0x44,0x00,0x58,0x44,0x00,0x90,0x7F,0x00,0x00,0x7F,	// 226
	0x00,0x00,0x00,0x00,0x39,0x00,0x98,0x6D,0x00,0x48,0x44,0x00,0x48,0x44,0x00,0x50,0x44,0x00,0x98,0x7F,0x00,0x00,0x7F,	// 227
	0x00,0x00,0x00,0x00,0x39,0x00,0x98,0x6D,0x00,0x50,0x44,0x00,0x40,0x44,0x00,0x58,0x44,0x00,0x98,0x7F,0x00,0x00,0x7F,	// 228
	0x00,0x00,0x00,0x00,0x39,0x00,0x80,0x6D,0x00,0x5C,0x44,0x00,0x54,0x44,0x00,0x5C,0x44,0x00,0x80,0x7F,0x00,0x00,0x7F,	// 229
	0x00,0x00,0x00,0x80,0x39,0x00,0xC0,0x6D,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0x40,0x64,0x00,0x80,0x3F,0x00,0x80,0x3F,0x00,0xC0,0x64,0x00,0x40,0x44,0x00,0x40,0x44,0x00,0xC0,0x44,0x00,0x80,0x27,	// 230
	0x00,0x00,0x00,0x80,0x3F,0x00,0xC0,0x60,0x00,0x40,0x40,0x00,0x40,0xC0,0x02,0x40,0xC0,0x03,0x80,0x31,0x00,0x00,0x11,	// 231
	0x00,0x04,0x00,0x80,0x3F,0x00,0xC8,0x64,0x00,0x48,0x44,0x00,0x58,0x44,0x00,0xD0,0x44,0x00,0x80,0x27,0x00,0x00,0x26,	// 232
	0x00,0x04,0x00,0x80,0x3F,0x00,0xC0,0x64,0x00,0x40,0x44,0x00,0x50,0x44,0x00,0xD8,0x44,0x00,0x88,0x27,0x00,0x00,0x26,	// 233
	0x00,0x04,0x00,0x80,0x3F,0x00,0xD0,0x64,0x00,0x50,0x44,0x00,0x48,0x44,0x00,0xD8,0x44,0x00,0x90,0x27,0x00,0x00,0x26,	// 234
	0x00,0x04,0x00,0x80,0x3F,0x00,0xD8,0x64,0x00,0x50,0x44,0x00,0x40,0x44,0x00,0xD8,0x44,0x00,0x98,0x27,0x00,0x00,0x26,	// 235
	0x08,0x00,0x00,0xD8,0x7F,0x00,0xD0,0x7F,	// 236
	0x00,0x00,0x00,0xD0,0x7F,0x00,0xD8,0x7F,0x00,0x08,	// 237
	0x10,0x00,0x00,0xC8,0x7F,0x00,0xD8,0x7F,0x00,0x10,	// 238
	0x18,0x00,0x00,0xD0,0x7F,0x00,0xC0,0x7F,0x00,0x18,	// 239
	0x00,0x00,0x00,0x00,0x3E,0x00,0x08,0x23,0x00,0x58,0x41,0x00,0x30,0x41,0x00,0x30,0x41,0x00,0xF8,0x33,0x00,0x90,0x1F,	// 240
	0x00,0x00,0x00,0xC0,0x7F,0x00,0xD8,0x7F,0x00,0x48,0x00,0x00,0x48,0x00,0x00,0x50,0x00,0x00,0xD8,0x7F,0x00,0x80,0x7F,	// 241
	0x00,0x00,0x00,0x00,0x1F,0x00,0x88,0x20,0x00,0x48,0x40,0x00,0x58,0x40,0x00,0x50,0x40,0x00,0x80,0x20,0x00,0x00,0x3F,	// 242
	0x00,0x00,0x00,0x00,0x1F,0x00,0x80,0x20,0x00,0x40,0x40,0x00,0x50,0x40,0x00,0x58,0x40,0x00,0x88,0x20,0x00,0x00,0x3F,	// 243
	0x00,0x00,0x00,0x00,0x1F,0x00,0x90,0x20,0x00,0x50,0x40,0x00,0x48,0x40,0x00,0x58,0x40,0x00,0x90,0x20,0x00,0x00,0x3F,	// 244
	0x00,0x00,0x00,0x00,0x1F,0x00,0x98,0x20,0x00,0x48,0x40,0x00,0x48,0x40,0x00,0x50,0x40,0x00,0x98,0x20,0x00,0x00,0x3F,	// 245
	0x00,0x00,0x00,0x00,0x1F,0x00,0x98,0x20,0x00,0x50,0x40,0x00,0x40,0x40,0x00,0x58,0x40,0x00,0x98,0x20,0x00,0x00,0x3F,	// 246
	0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x60,0x32,0x00,0x00,0x02,0x00,0x00,0x02,0x00,0x00,0x02,	// 247
	0x00,0x00,0x00,0x00,0x9F,0x00,0x80,0xE0,0x00,0x40,0x70,0x00,0x40,0x4C,0x00,0xC0,0x43,0x00,0xE0,0x20,0x00,0xA0,0x3F,	// 248
	0x00,0x00,0x00,0xC0,0x3F,0x00,0xC8,0x7F,0x00,0x08,0x40,0x00,0x18,0x40,0x00,0x10,0x40,0x00,0xC0,0x7F,0x00,0xC0,0x7F,	// 249
	0x00,0x00,0x00,0xC0,0x3F,0x00,0xC0,0x7F,0x00,0x00,0x40,0x00,0x10,0x40,0x00,0x18,0x40,0x00,0xC8,0x7F,0x00,0xC0,0x7F,	// 250
	0x00,0x00,0x00,0xC0,0x3F,0x00,0xD0,0x7F,0x00,0x10,0x40,0x00,0x08,0x40,0x00,0x18,0x40,0x00,0xD0,0x7F,0x00,0xC0,0x7F,	// 251
	0x00,0x00,0x00,0xC0,0x3F,0x00,0xD8,0x7F,0x00,0x10,0x40,0x00,0x00,0x40,0x00,0x18,0x40,0x00,0xD8,0x7F,0x00,0xC0,0x7F,	// 252
	0x40,0x00,0x00,0xC0,0x01,0x02,0x00,0x0F,0x02,0x10,0xF8,0x01,0x18,0x78,0x00,0x08,0x0F,0x00,0xC0,0x01,0x00,0x40,	// 253
	0x00,0x00,0x00,0xF8,0xFF,0x03,0xF8,0xFF,0x03,0x40,0x40,0x00,0x40,0x40,0x00,0x40,0x40,0x00,0xC0,0x71,0x00,0x80,0x3F,	// 254
	0x40,0x00,0x00,0xC0,0x01,0x02,0x18,0x0F,0x02,0x10,0xF8,0x01,0x00,0x78,0x00,0x18,0x0F,0x00,0xD8,0x01,0x00,0x40	// 255
};
