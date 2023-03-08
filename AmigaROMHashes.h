/*
MIT License

Copyright (c) 2021 Christopher Gelatt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AMIGAROMHASHES_H
#define AMIGAROMHASHES_H

#include <stddef.h>

typedef struct {
	const char sha1hash[41];
	const size_t file_size;
	const char version[128];
	const char type;
	const int byte_swap;
} AmigaROMInfo;

static const AmigaROMInfo AMIGA_ROM_INFO[] = {
	// Merged, unswapped Kickstart ROMs
	{"fa685ebb666c3d8b09afe1329b7e41b443312f69", 262144, "AmigaOS 0.7b [27.003b] (A1000)",                           'M', 0},
	{"00c15406beb4b8ab1a16aa66c05860e1a7c1ad79", 262144, "AmigaOS 1.0 (A1000)",                                      'M', 0},
	{"4192c505d130f446b2ada6bdc91dae730acafb4c", 262144, "AmigaOS 1.1 [31.034] (NTSC, A1000)",                       'M', 0},
	{"16df8b5fd524c5a1c7584b2457ac15aff9e3ad6d", 262144, "AmigaOS 1.1 [31.034] (PAL, A1000)",                        'M', 0},
	{"6a7bfb5dbd6b8f179f03da84d8d9528267b6273b", 262144, "AmigaOS 1.2 [33.166] (A500/A1000/A2000)",                  'M', 0},
	{"11f9e62cf299f72184835b7b2a70a16333fc0d88", 262144, "AmigaOS 1.2 [33.180] (A500/A1000/A2000)",                  'M', 0},
	{"891e9a547772fe0c6c19b610baf8bc4ea7fcb785", 262144, "AmigaOS 1.3 [34.005] (A500/A1000/A2000/CDTV)",             'M', 0},
	{"c39bd9094d4e5f4e28c1411f3086950406062e87", 262144, "AmigaOS 1.3 [34.005] (A3000)",                             'M', 0},
	{"f76316bf36dff14b20fa349ed02e4b11dd932b07", 524288, "AmigaOS 1.4 [36.016] (A3000)",                             'M', 0},
	{"f2cc0cc8bf9321df63456c439884c15cda0fe752", 524288, "AmigaOS 2.02 [36.207] (A3000)",                            'M', 0},
	{"c5839f5cb98a7a8947065c3ed2f14f5f42e334a1", 524288, "AmigaOS 2.04 [37.175] (A500+)",                            'M', 0},
	{"d82ebb59afc53540ddf2d7187ecf239b7ea91590", 524288, "AmigaOS 2.04 [37.175] (A3000)",                            'M', 0},
	{"87508de834dc7eb47359cede72d2e3c8a2e5d8db", 524288, "AmigaOS 2.05 [37.299] (A600)",                             'M', 0},
	{"f72d89148dac39c696e30b10859ebc859226637b", 524288, "AmigaOS 2.05 [37.300] (A600HD)",                           'M', 0},
	{"02843c4253bbd29aba535b0aa3bd9a85034ecde4", 524288, "AmigaOS 2.05 [37.350] (A600HD)",                           'M', 0},
	{"70033828182fffc7ed106e5373a8b89dda76faa5", 524288, "AmigaOS 3.0 [39.106] (A1200)",                             'M', 0},
	{"f0b4e9e29e12218c2d5bd7020e4e785297d91fd7", 524288, "AmigaOS 3.0 [39.106] (A4000)",                             'M', 0},
	{"a268617888b2c8ffb10db85ea9b92d1129a0cd24", 524288, "AmigaOS 3.1 [40.055] (A3000)",                             'M', 0},
	{"3525be8887f79b5929e017b42380a79edfee542d", 524288, "AmigaOS 3.1 [40.060] (CD32)",                              'M', 0},
	{"3b7f1493b27e212830f989f26ca76c02049f09ca", 524288, "AmigaOS 3.1 [40.063] (A500/A600/A2000)",                   'M', 0},
	{"e21545723fe8374e91342617604f1b3d703094f1", 524288, "AmigaOS 3.1 [40.068] (A1200)",                             'M', 0},
	{"f8e210d72b4c4853e0c9b85d223ba20e3d1b36ee", 524288, "AmigaOS 3.1 [40.068] (A3000)",                             'M', 0},
	{"5fe04842d04a489720f0f4bb0e46948199406f49", 524288, "AmigaOS 3.1 [40.068] (A4000)",                             'M', 0},
	{"b0ec8b84d6768321e01209f11e6248f2f5281a21", 524288, "AmigaOS 3.1 [40.070] (A4000T)",                            'M', 0},
	{"7a9095f1107966f90267dc4cb3c1972efb4b78a8", 524288, "AmigaOS 3.2b [43.001b] (Walker)",                          'M', 0},
	{"3cbfc9e1fe396360157bd161de74fc901abee7ec", 524288, "AmigaOS 3.x AF3.0 [45.057] (A4000)",                       'M', 0},
	{"187adf4e016af8ebcdd3a63943638aa6039f37e3", 524288, "AmigaOS 3.x AF6.8 [45.061] (A4000)",                       'M', 0},
	{"e035bfee642e4dfa667db40c0553298774e538d0", 524288, "AmigaOS 3.x AF7.0 [45.061] (A500/A600/A2000)",             'M', 0},
	{"676f706fe558ba87d1b6fcc82107d030781d08e7", 524288, "AmigaOS 3.x AF7.0 [45.061] (A1200)",                       'M', 0},
	{"2f0c6efa00ef8a6943a636fd002098964a05de32", 524288, "AmigaOS 3.x AF7.0 [45.061] (A3000)",                       'M', 0},
	{"d908162580417b00d32abe5ef130c64c93706c57", 524288, "AmigaOS 3.x AF7.0 [45.061] (A4000T)",                      'M', 0},
	{"abca7c05bde8bfc77e7a22d5e83727a50106e353", 524288, "AmigaOS 3.x AF7.1 [45.064] (A500/A600/A2000)",             'M', 0},
	{"63076f74dc557cfb0b7e7b17d99f3a6f76d9276d", 524288, "AmigaOS 3.x AF7.1 [45.064] (A1200)",                       'M', 0},
	{"84d399c46b9d38d41da1bd3f99335a34feafa876", 524288, "AmigaOS 3.x AF7.1 [45.064] (A3000)",                       'M', 0},
	{"a681c63f6c075998a19e6e7707f6220f57cd307e", 524288, "AmigaOS 3.x AF7.1 [45.064] (A4000)",                       'M', 0},
	{"d553fc1416c6faf3a73fd8c056038c023df2321e", 524288, "AmigaOS 3.x AF7.1 [45.064] (A4000T)",                      'M', 0},
	{"46f15c7a50b45eed7cc1a1670b164aff71b01907", 524288, "AmigaOS 3.x AF8.0 [45.066] (A500/A600/A2000)",             'M', 0},
	{"279c6a27f8cb81c74e10b263d20f03433f98dc07", 524288, "AmigaOS 3.x AF8.0 [45.066] (A1200)",                       'M', 0},
	{"10390d4169b1009e6c81f8a572cddd83127f583d", 524288, "AmigaOS 3.x AF8.0 [45.066] (A3000)",                       'M', 0},
	{"0e1a4b5062826d8e3520cce752f47409f4adef96", 524288, "AmigaOS 3.x AF8.0 [45.066] (A4000)",                       'M', 0},
	{"d5f1be640c38e7a8585567e4ed71b7d5e1ff772c", 524288, "AmigaOS 3.x AF8.0 [45.066] (A4000T)",                      'M', 0},
	{"8a2405087ce182225656dd0b93069c45743f9e34", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (Old Copyright)", 'M', 0},
	{"6355a9ed5dc840422f9b73308a91be0d0bb506bd", 524288, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright)",           'M', 0},
	{"dd42f228616af9159856365b906c17e700e5424c", 524288, "AmigaOS 3.1.4 [46.143] (A2000) (Old Copyright)",           'M', 0},
	{"1a34b25a25e260e38e879172850143c80b461a64", 524288, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright)",           'M', 0},
	{"bad0ae388442db02eaeb6a5363ef43eb4e308ae6", 524288, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright)",           'M', 0},
	{"938a60a1d2c0d411f64bb27e5af40258b8decbf3", 524288, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright)",          'M', 0},
	{"d81cd6f131040895843d9dfa95a45bc95edd9704", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (New Copyright)", 'M', 0},
	{"ef36c4638ee45de6bb93701761216c958cd0d57b", 524288, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright)",           'M', 0},
	{"d73ae3a36f12bb49bbf6ba04a890ff7aac419015", 524288, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright)",           'M', 0},
	{"aab44cd651e6b6f81a3effd8e0ba6b37ab322f32", 524288, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright)",           'M', 0},
	{"cd73aefe9cbfc258e7966cd14a7b2f4647c9ba45", 524288, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright)",          'M', 0},
	{"b88e364daf23c9c9920e548b0d3d944e65b1031d", 524288, "AmigaOS 3.2 [47.096] (A500/A600/A1000/A2000/CDTV)",        'M', 0},
	{"5b2982876fec2166673be447643881262c84090e", 524288, "AmigaOS 3.2 [47.096] (A1200)",                             'M', 0},
	{"7bc0e75622d7254e11ea708e5d149007866c93ab", 524288, "AmigaOS 3.2 [47.096] (A3000)",                             'M', 0},
	{"37a8aa0b83782d75ce0b4a80631ff3459e91dc63", 524288, "AmigaOS 3.2 [47.096] (A4000)",                             'M', 0},
	{"ede1748eb2cbb1e86ac5bf7dc9b97246a4a54358", 524288, "AmigaOS 3.2 [47.096] (A4000T)",                            'M', 0},
	{"8f64ada68a7f128ba782e8dc9fa583344171590a", 524288, "AmigaOS 3.2.1 [47.102] (A500/A600/A1000/A2000/CDTV)",      'M', 0},
	{"0984fc0df07dc0585db0923d580629f70fca420d", 524288, "AmigaOS 3.2.1 [47.102] (A1200)",                           'M', 0},
	{"eb93508f0bb0cde81d88bf22d05045565d66ef74", 524288, "AmigaOS 3.2.1 [47.102] (A3000)",                           'M', 0},
	{"7db3c3226acc0bfe548c788cf1bc7c4c8774d66f", 524288, "AmigaOS 3.2.1 [47.102] (A4000)",                           'M', 0},
	{"a75fcd349680cedeab06e496c6338394e81788d7", 524288, "AmigaOS 3.2.1 [47.102] (A4000T)",                          'M', 0},
	{"7d5ebe686b69d59a863cc77a36b2cd60359a9ed2", 524288, "AmigaOS 3.2.2 [47.111] (A500/A600/A1000/A2000/CDTV)",      'M', 0},
	{"d9622547d84741b52c5472607f9aca75565cbba3", 524288, "AmigaOS 3.2.2 [47.111] (A1200)",                           'M', 0},
	{"443d98369aca3ba51ab2d4789d14fa9a26379c0b", 524288, "AmigaOS 3.2.2 [47.111] (A3000)",                           'M', 0},
	{"a3207bd5b5a3ca629010f3159ac793040fa813ef", 524288, "AmigaOS 3.2.2 [47.111] (A4000)",                           'M', 0},
	{"8e3fff2675cc1b97b4f072b63fd4de7093f7c627", 524288, "AmigaOS 3.2.2 [47.111] (A4000T)",                          'M', 0},

	// Swapped Kickstart ROMs
	{"4e2ee86c5675312ec4319bc694bdcabc81897e46", 262144, "AmigaOS 0.7b [27.003b] (A1000)",                           'M', 1},
	{"19c02d1a53bc3a3559699f990f6ff6a238bc854d", 262144, "AmigaOS 1.0 (A1000)",                                      'M', 1},
	{"8b47fd3feac0e138be71b49448c99d9713333728", 262144, "AmigaOS 1.1 [31.034] (NTSC, A1000)",                       'M', 1},
	{"3bf4e177195d50d6c350e403961cc9b1b8822cac", 262144, "AmigaOS 1.1 [31.034] (PAL, A1000)",                        'M', 1},
	{"a6fca3d9c4e67283dc12f2ddebb4bf08adef638d", 262144, "AmigaOS 1.2 [33.166] (A500/A1000/A2000)",                  'M', 1},
	{"feb4c7ddffd2864d2695628bce95454aaf5925c2", 262144, "AmigaOS 1.2 [33.180] (A500/A1000/A2000)",                  'M', 1},
	{"e84e6ec0d65eeae7cf3a1dd3947c6695497dd31d", 262144, "AmigaOS 1.3 [34.005] (A500/A1000/A2000/CDTV)",             'M', 1},
	{"b7f61e452d668449e2e76682d427ec17a379557a", 524288, "AmigaOS 2.04 [37.175] (A500+)",                            'M', 1},
	{"d0a8a9ad7ae0aa6418f6de63450f0bc628c87801", 524288, "AmigaOS 2.05 [37.299] (A600)",                             'M', 1},
	{"199697d832caca8e4c15a6f432fd2d08d78e230f", 524288, "AmigaOS 2.05 [37.300] (A600HD)",                           'M', 1},
	{"f9b7d78169aca89759cd362ab108e65a784a4988", 524288, "AmigaOS 2.05 [37.350] (A600HD)",                           'M', 1},
	{"5a5719ad375741e8c23f9dd288ebe74abda18911", 524288, "AmigaOS 3.1 [40.063] (A500/A600/A2000)",                   'M', 1},
	{"99fea56af9b7745dee75a9036cd37dfd41d8e8a9", 524288, "AmigaOS 3.x AF7.0 [45.061] (A500/A600/A2000)",             'M', 1},
	{"6c5c0d4d0917e4ae413ac1c1cc14395508aa158a", 524288, "AmigaOS 3.x AF7.1 [45.064] (A500/A600/A2000)",             'M', 1},
	{"65f78c13db26f6b6d1ee3e9ecec1867f1658d163", 524288, "AmigaOS 3.x AF8.0 [45.066] (A500/A600/A2000)",             'M', 1},
	{"10a5eb4ccb3b88afeaa2ebbe860ae5e05611d893", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (Old Copyright)", 'M', 1},
	{"6f55f7afe5ef3f7d2045db2399758477d9bccdbf", 524288, "AmigaOS 3.1.4 [46.143] (A2000) (Old Copyright)",           'M', 1},
	{"c50a0e32b4d59770b617c9817038bdfd1947eb46", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (New Copyright)", 'M', 1},
	{"dfae20c0a043d548d57299e64bc1d8bbfc8b011b", 524288, "AmigaOS 3.2 [47.096] (A500/A600/A1000/A2000/CDTV)",        'M', 1},
	{"6019741ce39b994405ccfc073267b4deb21e94d1", 524288, "AmigaOS 3.2.1 [47.102] (A500/A600/A1000/A2000/CDTV)",      'M', 1},
	{"7dc0de0699d01cc0a80562c296daa462a367da3b", 524288, "AmigaOS 3.2.2 [47.111] (A500/A600/A1000/A2000/CDTV)",      'M', 1},

	// Split/swapped Kickstart ROMs for burning
	{"2be27a7b8c47c2a47bd3c8298d00b3b71bba9629", 262144, "AmigaOS 1.3 [34.005] (A3000) Hi",                          'A', 1},
	{"d562a41ca084b4a9133128be636652b6b2199673", 262144, "AmigaOS 1.3 [34.005] (A3000) Lo",                          'B', 1},
	{"64d17c73dc1bf0aab035575fa4d9eaae3cb90839", 262144, "AmigaOS 1.4 [36.016] (A3000) Hi",                          'A', 1},
	{"a4353efe86bb029143ebc176a765299c93d3bd94", 262144, "AmigaOS 1.4 [36.016] (A3000) Lo",                          'B', 1},
	{"2be0a39977e35ac4b07e23d98a8ae6d66018daaa", 262144, "AmigaOS 2.02 [36.207] (A3000) Hi",                         'A', 1},
	{"1ad296045fb024e21ce7c67ab8d8ae47d33fc361", 262144, "AmigaOS 2.02 [36.207] (A3000) Lo",                         'B', 1},
	{"a7e9d4f574d34d75f1812bc5e9301e23040a10e5", 262144, "AmigaOS 2.04 [37.175] (A3000) Hi",                         'A', 1},
	{"d6bee4a6f90361e1c87c7bda0d6fa7d371b8b18a", 262144, "AmigaOS 2.04 [37.175] (A3000) Lo",                         'B', 1},
	{"ca749555bd7d53c22f182bf605698fdebe8cceb2", 262144, "AmigaOS 3.0 [39.106] (A1200) Hi",                          'A', 1},
	{"bc269462de6265ebd4c2d6bb8db2140e267dbea7", 262144, "AmigaOS 3.0 [39.106] (A1200) Lo",                          'B', 1},
	{"2097527d060b2b1a70c0e2b4273b5ad3b5278603", 262144, "AmigaOS 3.0 [39.106] (A4000) Hi",                          'A', 1},
	{"995db56375b7a5d1edc5037b8c110a7c67bb20ec", 262144, "AmigaOS 3.0 [39.106] (A4000) Lo",                          'B', 1},
	{"e2e663ca1bb5b44a27d3c97ec06d1bbacbc2fccd", 262144, "AmigaOS 3.1 [40.055] (A3000) Hi",                          'A', 1},
	{"03f57d9e0a97660749141d247a7464da3ff6eba4", 262144, "AmigaOS 3.1 [40.055] (A3000) Lo",                          'B', 1},
	{"4f27f07aa08f043802cc9910b97f2392ff3d81d5", 262144, "AmigaOS 3.1 [40.060] (CD32) Hi",                           'A', 1},
	{"2f010d95f38556a1a61cc828c29d69b8450a2891", 262144, "AmigaOS 3.1 [40.060] (CD32) Hi",                           'B', 1},
	{"cd6409ceb748566fbd89ed31f8d1c01e474b4d97", 262144, "AmigaOS 3.1 [40.068] (A1200) Hi",                          'A', 1},
	{"dcbaa8aa562d69842a05bdcc04abd9aa32da4a05", 262144, "AmigaOS 3.1 [40.068] (A1200) Lo",                          'B', 1},
	{"668a396cda15178d0fcd99c7b9bdcdc870f64221", 262144, "AmigaOS 3.1 [40.068] (A3000) Hi",                          'A', 1},
	{"a84f5ef6cfb73a45f9e49582850ff9ed619f5ed2", 262144, "AmigaOS 3.1 [40.068] (A3000) Lo",                          'B', 1},
	{"4b58a68209a43d68c802963eb0f4eaa8cb872496", 262144, "AmigaOS 3.1 [40.068] (A4000) Hi",                          'A', 1},
	{"2552398aae971a8d4721c78bb1685fa9f2c4c42e", 262144, "AmigaOS 3.1 [40.068] (A4000) Lo",                          'B', 1},
	{"959ed70d723ba1454f40039548ee901ab8dfae85", 262144, "AmigaOS 3.1 [40.070] (A4000T) Hi",                         'A', 1},
	{"adcf8d61250eedf193a4269900b35e878e55b4ac", 262144, "AmigaOS 3.1 [40.070] (A4000T) Lo",                         'B', 1},
	{"a69531064c413aeee4513151f8f234c0f9d8eac8", 262144, "AmigaOS 3.2b [43.001b] (Walker) Hi",                       'A', 1},
	{"683f256e0f6ef6a8f73f1c63713ac2aaa0e716a5", 262144, "AmigaOS 3.2b [43.001b] (Walker) Lo",                       'B', 1},
	{"d2c2e4e72975892e800c74b55a79190cd8af5b21", 262144, "AmigaOS 3.x AF3.0 [45.057] (A4000) Hi",                    'A', 1},
	{"10bc268167d9191ae6871bea2f729fe64ca063c4", 262144, "AmigaOS 3.x AF3.0 [45.057] (A4000) Lo",                    'B', 1},
	{"77ed865ee4ae74965a3a9184f9b657933546ba1d", 262144, "AmigaOS 3.x AF6.8 [45.061] (A4000) Hi",                    'A', 1},
	{"e70e36a213975fbe49a917aa7651e913e108618a", 262144, "AmigaOS 3.x AF6.8 [45.061] (A4000) Lo",                    'B', 1},
	{"0c146318a9441c3c1c9e1c683c81c4be3ae3cf4b", 262144, "AmigaOS 3.x AF7.0 [45.061] (A1200) Hi",                    'A', 1},
	{"f6b567b89d8b6a4c318c7a542de30b96d66d174d", 262144, "AmigaOS 3.x AF7.0 [45.061] (A1200) Lo",                    'B', 1},
	{"6eed8859b54e61ffbb28ef355fbc54483aa47919", 262144, "AmigaOS 3.x AF7.0 [45.061] (A3000) Hi",                    'A', 1},
	{"80f9f5e7013831d9bb250ca9082694077be4cb09", 262144, "AmigaOS 3.x AF7.0 [45.061] (A3000) Lo",                    'B', 1},
	{"27aa3afc0b01183819bb52b8ec74bfbcec9cd15b", 262144, "AmigaOS 3.x AF7.0 [45.061] (A4000T) Hi",                   'A', 1},
	{"f111d0c5f0be80018549a2526c4294e49c59d683", 262144, "AmigaOS 3.x AF7.0 [45.061] (A4000T) Lo",                   'B', 1},
	{"011c26bbffaf764cdcc686a152e38cef76c6ad1b", 262144, "AmigaOS 3.x AF7.1 [45.064] (A1200) Hi",                    'A', 1},
	{"73e3df252520785d6b18df055eb91f36e36b09f8", 262144, "AmigaOS 3.x AF7.1 [45.064] (A1200) Lo",                    'B', 1},
	{"622b5dd4b582d3b8eab22ff74c1741cccb28cf56", 262144, "AmigaOS 3.x AF7.1 [45.064] (A3000) Hi",                    'A', 1},
	{"7c7f2b3efef8231c498d2ed342c652fd54ef6f14", 262144, "AmigaOS 3.x AF7.1 [45.064] (A3000) Lo",                    'B', 1},
	{"f6ebefa5ee7012845f6b9c8940ab8980eedcab34", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000) Hi",                    'A', 1},
	{"18cb0a9d156b32e0d5197a82fbc6532cffd9bd92", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000) Lo",                    'B', 1},
	{"a6546c92f1c8487515519d393891468572dad2e4", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000T) Hi",                   'A', 1},
	{"3f5359b5e3b8e052a6fe9d56ece8ca4494310087", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000T) Lo",                   'B', 1},
	{"28f6ba44f4747def792cf525ec0660fde9c36533", 262144, "AmigaOS 3.x AF8.0 [45.066] (A1200) Hi",                    'A', 1},
	{"6f87d784fec320b59a9965fca5e25560d6004688", 262144, "AmigaOS 3.x AF8.0 [45.066] (A1200) Lo",                    'B', 1},
	{"747e8dfa105f8e06e071b71a6d289c1b731a5454", 262144, "AmigaOS 3.x AF8.0 [45.066] (A3000) Hi",                    'A', 1},
	{"5ebf30b0221de0aba473504a64f2891d5fa61a11", 262144, "AmigaOS 3.x AF8.0 [45.066] (A3000) Lo",                    'B', 1},
	{"08321a3ac619d4f8c085f3cc1fa16db44575d5fc", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000) Hi",                    'A', 1},
	{"e5450996be2ee0420cbadee8bc92f340b28a501d", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000) Lo",                    'B', 1},
	{"3327bd7731813ab9e0b9b478aa7871c4b2a479da", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000T) Hi",                   'A', 1},
	{"73b4dc7f158dc39c4d61faa2534bebcaa199a1cf", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000T) Lo",                   'B', 1},
	{"4daed46da9e75560db1dc81585dff8120ea01c25", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright) Hi",        'A', 1},
	{"5728fa8a538fabd39ed733a4b33d2da4a04277f8", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright) Lo",        'B', 1},
	{"0addca53892d65dbcd81524a361eb3498a4a02d7", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright) Hi",        'A', 1},
	{"24b7857087494f26d298776e6cd2fddfde33770e", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright) Lo",        'B', 1},
	{"646fc9c44a4619f9be64b2790911740fa9a5f281", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright) Hi",        'A', 1},
	{"7efc6e89d3141d4f974ae7423435b53c29b7a5eb", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright) Lo",        'B', 1},
	{"f4d7dc6c6f913c1465c9a939dca9a641154c2636", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright) Hi",       'A', 1},
	{"0a2e1ceef5758a44a4c066d793279e82e99af1fe", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright) Lo",       'B', 1},
	{"eada0c5da801c41e4f7a685f5e26ac42695be8f0", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright) Hi",        'A', 1},
	{"5f7f0473b6c15cbfd3950e72f2f9d4bc2fda5426", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright) Lo",        'B', 1},
	{"d5d81008234ab71b835942c9070b60d083988493", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright) Hi",        'A', 1},
	{"45218ed6052be144e1287b57c6c980e351e57dc2", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright) Lo",        'B', 1},
	{"4a42d093f1965ddc97a7df810743c594a2e8b280", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright) Hi",        'A', 1},
	{"28708dd807903eb92cbbbf47a34c0d225fb5ce81", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright) Lo",        'B', 1},
	{"af4760379f4bc677e25a07cbb5f941fae54e38f7", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright) Hi",       'A', 1},
	{"6be4cfd65c9d2a6869b57b97c3bd141fcdce77bd", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright) Lo",       'B', 1},
	{"6180165db63db311591fa581dc531aba7340207d", 262144, "AmigaOS 3.2 [47.096] (A1200) Hi",                          'A', 1},
	{"5ff86b1a41429b365b085e7709572b5957f956a4", 262144, "AmigaOS 3.2 [47.096] (A1200) Lo",                          'B', 1},
	{"89a3d438c33d195baca0742d5504315b5888ade2", 262144, "AmigaOS 3.2 [47.096] (A3000) Hi",                          'A', 1},
	{"d08a1f405283748ade32fe67bd3fb9d79f88567d", 262144, "AmigaOS 3.2 [47.096] (A3000) Lo",                          'B', 1},
	{"5fe056ae79adc23e7e65de7f3e5aad71df72be59", 262144, "AmigaOS 3.2 [47.096] (A4000) Hi",                          'A', 1},
	{"8c27ae8bdc9db1dbe49e300e11fea0df0082eac6", 262144, "AmigaOS 3.2 [47.096] (A4000) Lo",                          'B', 1},
	{"49a3d36203c0185d6c0a50bcda047e83c1192bbd", 262144, "AmigaOS 3.2 [47.096] (A4000T) Hi",                         'A', 1},
	{"9ba0bac48fd9b92e4d3d821a12d7d0bf51c885fe", 262144, "AmigaOS 3.2 [47.096] (A4000T) Lo",                         'B', 1},
	{"a7fb54d23ee26bf59c8c4ba5574f6482d6c93607", 262144, "AmigaOS 3.2.1 [47.102] (A1200) Hi",                        'A', 1},
	{"0fc5b3277c9fc107459cf15328fe642c4aaa53d0", 262144, "AmigaOS 3.2.1 [47.102] (A1200) Lo",                        'B', 1},
	{"c9668ecfcba0cdc29729aed2f656d6d53f8e7bfc", 262144, "AmigaOS 3.2.1 [47.102] (A3000) Hi",                        'A', 1},
	{"53c97fbec6052360b43e9be99456806366b7ae85", 262144, "AmigaOS 3.2.1 [47.102] (A3000) Lo",                        'B', 1},
	{"48b97a8c9854d8a0254e4bcd4a96cc35def4e3a7", 262144, "AmigaOS 3.2.1 [47.102] (A4000) Hi",                        'A', 1},
	{"8546b9baadbdc391e2fb52d9b0f2cbb85f780a4c", 262144, "AmigaOS 3.2.1 [47.102] (A4000) Lo",                        'B', 1},
	{"c59c779bd94e5e8c9620e78339ba1b9f3b64251c", 262144, "AmigaOS 3.2.1 [47.102] (A4000T) Hi",                       'A', 1},
	{"998bea2daa4051127ffe0c4701b42abc7c4c1cfc", 262144, "AmigaOS 3.2.1 [47.102] (A4000T) Lo",                       'B', 1},
	{"d08959195f260c1eddd8104ce80eee6c666fa352", 262144, "AmigaOS 3.2.2 [47.111] (A1200) Hi",                        'A', 1},
	{"f215641da4b59dc4dca448c41a6f8a2d06411c38", 262144, "AmigaOS 3.2.2 [47.111] (A1200) Lo",                        'B', 1},
	{"8b32d734eb9977bc0d2b87226fd3795c163ed587", 262144, "AmigaOS 3.2.2 [47.111] (A3000) Hi",                        'A', 1},
	{"fbe314870981a0c911a4bec5c9c581e43a649347", 262144, "AmigaOS 3.2.2 [47.111] (A3000) Lo",                        'B', 1},
	{"d9e1badba7b50ae0a8d21383da96dbba6892cfe1", 262144, "AmigaOS 3.2.2 [47.111] (A4000) Hi",                        'A', 1},
	{"92107dd42ba3b90a41b28a0e58c51d9d2fb04a61", 262144, "AmigaOS 3.2.2 [47.111] (A4000) Lo",                        'B', 1},
	{"8618ede52eee4a2e7e7a7d27120d9d13d87cf1ac", 262144, "AmigaOS 3.2.2 [47.111] (A4000T) Hi",                       'A', 1},
	{"a1261042bc11bc2c0bb44525de7d57c2b4ba39b0", 262144, "AmigaOS 3.2.2 [47.111] (A4000T) Lo",                       'B', 1},

	// Extended Amiga ROMs
	{"ed7e461d1fff3cda321631ae42b80e3cd4fa5ebb", 262144, "Ext. ROM 1.3 (A570)",                                      'E', 0},
	{"7ba40ffa17e500ed9fed041f3424bd81d9c907be", 262144, "Ext. ROM 1.3 (CDTV)",                                      'E', 0},
	{"14271106fe97e655345ba8cccac52be999c37990", 262144, "Ext. ROM 2.3 (CDTV)",                                      'E', 0},
	{"426ef4b3a229154ed320b2f84022261e7b4219a8", 262144, "Ext. ROM 2.35 (CDTV) (Unofficial)",                        'E', 0},
	{"5bef3d628ce59cc02a66e6e4ae0da48f60e78f7f", 524288, "Ext. ROM [40.060] (CD32)",                                 'E', 0},
	{"cfc16f8e930badcb9c3e7f889a57d645571fa86a", 262144, "Ext. ROM 3.2 [47.096] (CDTV)",                             'E', 0},

	// Other unswapped ROMs
	{"03ca81c7a7b259cf64bc9582863eca0f6529f435", 262144, "Amiga CRT 3.1 FMV (CD32)",                                 'O', 0},
	{"c87f9fada4ee4e69f3cca0c36193be822b9f5fe6",   8192, "Amiga SCSI Boot ROM (A1000)",                              'O', 0},
	{"e50f01baf28998928554786372a82c333c91276e",  16384, "Amiga SCSI Boot ROM (A590)",                               'O', 0},
	{"3ce66919f6fd67974923a12d91b730f1ffb4a7ba",  32768, "Amiga SCSI Boot ROM (A4091)",                              'O', 0},
	{"ba10d16166b2e2d6177c979c99edf8462b21651e", 524288, "Logica Dialoga 2.0",                                       'O', 0},
	{"9e292178d20a6773f90664b00b137d8f4e52233d", 131072, "Ralph Schmidt Cyberstorm PPC [44.071]",                    'O', 0},
	{"cafafb916f16b9f3ec9b49aa4b40eb4eeceb5b5b", 131072, "Village Tronic Picasso IV 7.4",                            'O', 0}
};

#endif
