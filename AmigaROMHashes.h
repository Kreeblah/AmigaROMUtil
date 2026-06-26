/*
MIT License

Copyright (c) 2026 Christopher Gelatt

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

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct {
	const char sha256hash[65];
	const size_t file_size;
	const char version[128];
	const char type;
	const int byte_swap;
} AmigaROMInfo;

static const AmigaROMInfo AMIGA_ROM_INFO[] = {
	// Merged, unswapped Kickstart ROMs
	{"e7f848aaea613cffe57b768ae806e6bbba42f259eb5bfd8032bee99dd6e19ae0", 262144, "AmigaOS 0.7b [27.003b] (A1000)",                           'M', 0},
	{"f0ecb1b000bc0317c886e6d4d4b3665aac0a26c9388bb1874eff51c6470dc03f", 262144, "AmigaOS 1.0 (A1000)",                                      'M', 0},
	{"879a7b2dc9735fd628dc0c52e8d97fb30713fa52a6d90856113610c2ea936e14", 262144, "AmigaOS 1.1 [31.034] (NTSC, A1000)",                       'M', 0},
	{"6a892043597fa2e22e57f3777d3fad46b6662c9206bcb715579971da09f920f2", 262144, "AmigaOS 1.1 [31.034] (PAL, A1000)",                        'M', 0},
	{"86a0de3a6e390fe0804e8d8e3d39091552f3f5020d8bd4989a77bb996bbc50b7", 262144, "AmigaOS 1.2 [33.166] (A500/A1000/A2000)",                  'M', 0},
	{"87cddb1f499e32758de20145e73031a84bab299e3f6e5c8487e76d02b2ee9d16", 262144, "AmigaOS 1.2 [33.180] (A500/A1000/A2000)",                  'M', 0},
	{"ee05862d8102a08436ac4056da7d549db31625c7d47b24dfb7b3c9a5c113ca53", 262144, "AmigaOS 1.3 [34.005] (A500/A1000/A2000/CDTV)",             'M', 0},
	{"fafb349f6137eaac97aa54cef2251247d1770b8da90b41266682cf6a82fbe3ec", 262144, "AmigaOS 1.3 [34.005] (A3000)",                             'M', 0},
	{"08d564e782972b59e16db013049117f9c73a650d946a52d2abe736f27e4713d7", 524288, "AmigaOS 1.4 [36.016] (A3000)",                             'M', 0},
	{"d6cb3e07759f48f5b1375fa2c81950acd3662984b7fcf9264117bb41deb1c60b", 524288, "AmigaOS 2.02 [36.207] (A3000)",                            'M', 0},
	{"d0b70e8a1772614b897f92c33cb299bed3fc8e3de488fc12f67f97fc2486eb79", 524288, "AmigaOS 2.04 [37.175] (A500+)",                            'M', 0},
	{"563f948af19c09daed1f06b8760221ff4b2789c1690cdcfbce2c941157af23d1", 524288, "AmigaOS 2.04 [37.175] (A3000)",                            'M', 0},
	{"59e7df327a7a680e4f2f185e17161e3257bf41247911f34ddf6564399645d703", 524288, "AmigaOS 2.05 [37.299] (A600)",                             'M', 0},
	{"339deebab62f772775d7afb27bd50dbb56391480c911114c72b0a06709181ca4", 524288, "AmigaOS 2.05 [37.300] (A600HD)",                           'M', 0},
	{"8d57d6e9d976df42d91ff9d10a5eedaede1a00e3b4b34cad750ca5ff7229f28a", 524288, "AmigaOS 2.05 [37.350] (A600HD)",                           'M', 0},
	{"293beea59d7de4caab3e07223d66e844ad5268eb06a04f405b523a0a1543d64c", 524288, "AmigaOS 3.0 [39.106] (A1200)",                             'M', 0},
	{"b859d9fb3d868a8328a4a363d44934eb1ee1c73955aa98576c7ffe3feed3d3ff", 524288, "AmigaOS 3.0 [39.106] (A4000)",                             'M', 0},
	{"1b838091ebba356ad3067c4f1d69dad10a2ee6984667a3690ae5a106df275ded", 524288, "AmigaOS 3.1 [40.055] (A3000)",                             'M', 0},
	{"3586355514947b5ab48c8755dc004fab94f188b19e7bce491c8f9693499e830b", 524288, "AmigaOS 3.1 [40.060] (CD32)",                              'M', 0},
	{"8c8a0cf04f91b88eaf0c4f1126041987067e2286a8ee590bdbae447a8000c5ee", 524288, "AmigaOS 3.1 [40.063] (A500/A600/A2000)",                   'M', 0},
	{"6d43840d4099a74170ea0f0425b6257c3891ebcaa39c4d1840075a9ab22b5707", 524288, "AmigaOS 3.1 [40.068] (A1200)",                             'M', 0},
	{"f2c9307aacb92b203a3135a35185deffad6c66cfa8d79920b235d7cf9a5222c4", 524288, "AmigaOS 3.1 [40.068] (A3000)",                             'M', 0},
	{"71236ed62c85394a3ad7d0d8d65405bac7a10083e06ba6b82120dacc7a00fe9a", 524288, "AmigaOS 3.1 [40.068] (A4000)",                             'M', 0},
	{"e5f87ea8953d840db0e05d097362ad1be98a1bf3292cf7f359190af61556d036", 524288, "AmigaOS 3.1 [40.070] (A4000T)",                            'M', 0},
	{"3913ea4124cc1d0b24bca3e9d7b6f8fd5935f41d8dcfb17781c1ca09424cf120", 524288, "AmigaOS 3.2b [43.001b] (Walker)",                          'M', 0},
	{"9ecb8c8070e35c4ce0f3b6567b5994bd082db1e0d0f0e17c259d1b36c47a6b62", 524288, "AmigaOS 3.x AF3.0 [45.057] (A4000)",                       'M', 0},
	{"b665a6d1c0a281dc1f1ce5867e4c4d14b1b5571ab3b392bd36eb0f7d7ec80175", 524288, "AmigaOS 3.x AF6.8 [45.061] (A4000)",                       'M', 0},
	{"db82fdafdebf501f694283d493f055cbc411b7586bd114e96192c6687446a352", 524288, "AmigaOS 3.x AF7.0 [45.061] (A500/A600/A2000)",             'M', 0},
	{"9bb8d250033dcdd6cb09107650e45b62047a66f32663354d3ce64ea9c7364e6e", 524288, "AmigaOS 3.x AF7.0 [45.061] (A1200)",                       'M', 0},
	{"38ca27796ba9c3edb4601fd4b3c58965504c09286975b4593da69ad1d423c344", 524288, "AmigaOS 3.x AF7.0 [45.061] (A3000)",                       'M', 0},
	{"b178e670030d5e9b270d43fca724d51869ff09fb211859c87395d0f6d7398560", 524288, "AmigaOS 3.x AF7.0 [45.061] (A4000T)",                      'M', 0},
	{"4acc216efa6dbee7625c6075b40446227d31f4f78efa3daf45420eb02ef779cf", 524288, "AmigaOS 3.x AF7.1 [45.064] (A500/A600/A2000)",             'M', 0},
	{"a4ac486388588995d8f3a69a0d8300d8faca1026212c182fb8b75bdf7f344c83", 524288, "AmigaOS 3.x AF7.1 [45.064] (A1200)",                       'M', 0},
	{"c206f14c20be3ceefc689c38f9245cdda8fdd4458cd8034a4189092f9198be0d", 524288, "AmigaOS 3.x AF7.1 [45.064] (A3000)",                       'M', 0},
	{"e2f0add8b10b7ac98c40683b5e54ae97642e8c7e4c38bb92d505dac356e69dac", 524288, "AmigaOS 3.x AF7.1 [45.064] (A4000)",                       'M', 0},
	{"d9aa68b441b66d38971154a932ce78a08e90bfaec6d956855b443df0d312a3bf", 524288, "AmigaOS 3.x AF7.1 [45.064] (A4000T)",                      'M', 0},
	{"0a19cc57bbffd52374da97a42c3870e03d5529aa712347d447f53fa688ed5f9f", 524288, "AmigaOS 3.x AF8.0 [45.066] (A500/A600/A2000)",             'M', 0},
	{"00aade66620d0b1885aa054ebedf6b414220e0219e98eee77c2b0d805dec042c", 524288, "AmigaOS 3.x AF8.0 [45.066] (A1200)",                       'M', 0},
	{"d40c5906c8bf050e121b3d4f53e7a696c23642523032cc5e27e6ddcb18da4300", 524288, "AmigaOS 3.x AF8.0 [45.066] (A3000)",                       'M', 0},
	{"d5c6ce79f096a24086d2d73bfc76f11daa8286a66f75c020f6aacb14c9008436", 524288, "AmigaOS 3.x AF8.0 [45.066] (A4000)",                       'M', 0},
	{"6ff0ece4068c26ee7d4346c249143f959b570d2b3a4ecdf96dbb011284fbd694", 524288, "AmigaOS 3.x AF8.0 [45.066] (A4000T)",                      'M', 0},
	{"886dc0bc6089d88ed76597dc07dd238f6bde9d09d65e9423d3795ae2989f1dee", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (Old Copyright)", 'M', 0},
	{"f797db0b99856d9c8219ee1f11e16285fd7dbdc0cbca2e149befd3a3986eb007", 524288, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright)",           'M', 0},
	{"c5e083dbaee0973a61f81aa03e354752fdc8d5e34da8de96517638bacc604e05", 524288, "AmigaOS 3.1.4 [46.143] (A2000) (Old Copyright)",           'M', 0},
	{"c5a201aced1b3a4b2cb50a83ade804acebf48e2ae86062f22c535f763db24d36", 524288, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright)",           'M', 0},
	{"990272a0e53accc5cde6989210af0f0bd40e428946c2c769e11f0fef875ce759", 524288, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright)",           'M', 0},
	{"dec00465238236cd954bf85fc83e26a3f56b1b1095ce124bfc3087a6682a3acd", 524288, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright)",          'M', 0},
	{"ca52081be6809a9ec344908c738109d79dbf341e20a7169a29b5a34f150c08b3", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (New Copyright)", 'M', 0},
	{"938bc1602cbe5038f069fddddeee9d6f8f652d9163685d620bf6651d000cc65e", 524288, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright)",           'M', 0},
	{"af254071f122bfabf8af9d8be13b2ae60d44cd333cfe8b580d570832a7e3a897", 524288, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright)",           'M', 0},
	{"6d4ce748d669ab4b981955815d467620347258b9ec0ee2bccb428c3a474cefc8", 524288, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright)",           'M', 0},
	{"cb1a8e9a236578cb12afb6a0f84559406e7db6585675830d5173762aacb2df8e", 524288, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright)",          'M', 0},
	{"0ea0943d93a6fdb56ab5da4298beee8463f1bb11d3721ac06e621a2ded20edb1", 524288, "AmigaOS 3.2 [47.096] (A500/A600/A1000/A2000/CDTV)",        'M', 0},
	{"ffcf6993c1a2df032fee1125036e2192cf29f70add547a50fc2cfff8f4fb9f7a", 524288, "AmigaOS 3.2 [47.096] (A1200)",                             'M', 0},
	{"f08f9c905f23a0a53a1cd69f72ab04e0cc48ed83bee2191ad639616e8776967f", 524288, "AmigaOS 3.2 [47.096] (A3000)",                             'M', 0},
	{"151f1984fa567a183d761378af8050e35cd8694b169cff2bcb49dd539eb0f86d", 524288, "AmigaOS 3.2 [47.096] (A4000)",                             'M', 0},
	{"3c6d26cf5c6201caf8e1fd31091b966a8916a28e4dea2e2f27b8be9661b9b809", 524288, "AmigaOS 3.2 [47.096] (A4000T)",                            'M', 0},
	{"2e2360558892868ec5d336f5dc27a814b9e3bb7f7f01c8f0e5785a4cbaac7dc9", 524288, "AmigaOS 3.2.1 [47.102] (A500/A600/A1000/A2000/CDTV)",      'M', 0},
	{"68f9d3a36c2866d054f41c29b600618642a4807dfb6bca7354d4879a36c9aa7c", 524288, "AmigaOS 3.2.1 [47.102] (A1200)",                           'M', 0},
	{"b5393c00874da98ed60c33244e06065aca05a76f8fb64ab8c1155fa9ee6d7947", 524288, "AmigaOS 3.2.1 [47.102] (A3000)",                           'M', 0},
	{"0eb551f4475bc4302d3c9d1338bfa4a4bf5f53f7cad45bcdd99ad418e6f0ae44", 524288, "AmigaOS 3.2.1 [47.102] (A4000)",                           'M', 0},
	{"a30ec8c1697082b04e4a7ba6b13a1cc4b26c9871e332278c87ccb1b19b4c9e51", 524288, "AmigaOS 3.2.1 [47.102] (A4000T)",                          'M', 0},
	{"4d4f2aac986f0ffa8da2ba23c7da3d531a880027ecf36a63ae48352c9cdfd9b4", 524288, "AmigaOS 3.2.2 [47.111] (A500/A600/A1000/A2000/CDTV)",      'M', 0},
	{"05ff5467058622ebcb4f1eb87093979f5d7ccd6e1c4b2b384d78d52666a7ebfa", 524288, "AmigaOS 3.2.2 [47.111] (A1200)",                           'M', 0},
	{"d138342137ffbd2e007bebdec919f0582252d54aedcae254bd8cb8fb3a2fe91c", 524288, "AmigaOS 3.2.2 [47.111] (A3000)",                           'M', 0},
	{"09e81cd3f55408eb515f0b26e81213ffcc9ce7706414336fabe0f8c259009cef", 524288, "AmigaOS 3.2.2 [47.111] (A4000)",                           'M', 0},
	{"375f4a609d99aa142bccc5e521f174aa53ba7f7dcec4a7ed12c380e0b9d084dd", 524288, "AmigaOS 3.2.2 [47.111] (A4000T)",                          'M', 0},
	{"e66823f4baaf2c93ea6ac600c07014ccddadc44cc58281c564baf165e2035b47", 524288, "AmigaOS 3.2.3 [47.115] (A500/A600/A1000/A2000/CDTV)",      'M', 0},
	{"27ddaf0a460e2ee85935139cbac1df234c283b3979c50526d118e9a55c61100a", 524288, "AmigaOS 3.2.3 [47.115] (A1200)",                           'M', 0},
	{"21b63158aed2f1aa65c2f4f6950a5e9d5c260de22496b77184669ca809499770", 524288, "AmigaOS 3.2.3 [47.115] (A3000)",                           'M', 0},
	{"b35a9073b01f58e1bbc6b421578c01b0023bbf01eda50f01847d6fe5aa9cc65a", 524288, "AmigaOS 3.2.3 [47.115] (A4000)",                           'M', 0},
	{"36d9d48fb2bc73b0aa1a0a6758e7eb8a9a593e3ddb8b21fc6044f681f80dfbb0", 524288, "AmigaOS 3.2.3 [47.115] (A4000T)",                          'M', 0},

	// Swapped Kickstart ROMs
	{"c4e82cd0a5b65c2b666838d12e66efb905d385c13c03f68d16c6e9d995687a34", 262144, "AmigaOS 0.7b [27.003b] (A1000)",                           'M', 1},
	{"5ea774c6926e2aefd200ff032817c5c798794798371ef8ad4b8d6b9874b5aa0d", 262144, "AmigaOS 1.0 (A1000)",                                      'M', 1},
	{"f9ecde327f55a2412e8167a3d263a00004ab12c462605f29270444b4aa556265", 262144, "AmigaOS 1.1 [31.034] (NTSC, A1000)",                       'M', 1},
	{"ee679fd413fe662dbe96741b9d31235612e6b3704bffe232a1c0ac1306f5d081", 262144, "AmigaOS 1.1 [31.034] (PAL, A1000)",                        'M', 1},
	{"6f4d7b56871e0f94353a4cad44d1a70750e4ab1f1bf5de827fccd5b131e2cd85", 262144, "AmigaOS 1.2 [33.166] (A500/A1000/A2000)",                  'M', 1},
	{"71331769041b3d9eb946f3c00de5b239f69b64195ae31e226078cda5f037ec73", 262144, "AmigaOS 1.2 [33.180] (A500/A1000/A2000)",                  'M', 1},
	{"14e84ee1b15c073c21bf2261a7c4523edaa16270968b45398d5404504c90c06b", 262144, "AmigaOS 1.3 [34.005] (A500/A1000/A2000/CDTV)",             'M', 1},
	{"71d0dc2ee218719ed241946a943d59a63919522b7ceabccd7ce5541fec9d6a23", 524288, "AmigaOS 2.04 [37.175] (A500+)",                            'M', 1},
	{"24900b6ba54813276f72ddab5f25a0e521ed0b126cdbfe058f41f2db2301462e", 524288, "AmigaOS 2.05 [37.299] (A600)",                             'M', 1},
	{"b8cf8eb20ba1faf844e8ceae2ac6318601f1c9a6c9b9b683816550b292fb7bf1", 524288, "AmigaOS 2.05 [37.300] (A600HD)",                           'M', 1},
	{"cc740e4e22a9eca716d86bdbd50f32e30f61400872cd8ec3cd0d0ca009b17b39", 524288, "AmigaOS 2.05 [37.350] (A600HD)",                           'M', 1},
	{"afb3bdf762e46182540c809906ad08df5b5c105d031ff82987af6b8a16cecda7", 524288, "AmigaOS 3.1 [40.063] (A500/A600/A2000)",                   'M', 1},
	{"2a1355276d04757c58484296b602874bfa949ab4c68658e9f2983dfdbc98a7fc", 524288, "AmigaOS 3.x AF7.0 [45.061] (A500/A600/A2000)",             'M', 1},
	{"9b2a1cc049bbef20fd365256ab69aa7e525adc3ae38ee38309b003669f3f4b11", 524288, "AmigaOS 3.x AF7.1 [45.064] (A500/A600/A2000)",             'M', 1},
	{"a3fa4727d5ee3e1c1d658c41bb4d452dec79985630da52bc526484c129703ac2", 524288, "AmigaOS 3.x AF8.0 [45.066] (A500/A600/A2000)",             'M', 1},
	{"f7a827e941dceef4ef6aeeee755e35cf6df0b58fb987846b9dea809bfcc396d0", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (Old Copyright)", 'M', 1},
	{"bee14df8805076099ec84e1a781d55137f7f993d36b5fa4a0129b640ee50b9f1", 524288, "AmigaOS 3.1.4 [46.143] (A2000) (Old Copyright)",           'M', 1},
	{"7118dcb98623001451456a5f97f439c8dd0498d7f321bd473e83baf0344cf17a", 524288, "AmigaOS 3.1.4 [46.143] (A500/A600/A2000) (New Copyright)", 'M', 1},
	{"41187732f40856f197bdcdbbe3da9395c6a4dc1567d425b52f07577bb71385d4", 524288, "AmigaOS 3.2 [47.096] (A500/A600/A1000/A2000/CDTV)",        'M', 1},
	{"1b442c776e454c7670d37cb5ff22930d17ffc4a7159ffffc748d4325ebb99a60", 524288, "AmigaOS 3.2.1 [47.102] (A500/A600/A1000/A2000/CDTV)",      'M', 1},
	{"992b5e043b46ed81bf72536a5ee7f28d918b05219b07e7fd92e2df0fac703efb", 524288, "AmigaOS 3.2.2 [47.111] (A500/A600/A1000/A2000/CDTV)",      'M', 1},
	{"2f48c3991748f4d812f1c0d13a99581cc17a058261d73856aee9bafda2d07fee", 524288, "AmigaOS 3.2.3 [47.115] (A500/A600/A1000/A2000/CDTV)",      'M', 1},

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
	{"ef74df7d0103d4a1f06248455b30b85db24ffd5a", 262144, "AmigaOS 3.2.3 [47.115] (A1200) Hi",                        'A', 1},
	{"85a36c01392dac8c8ac5ba73854f2fca61495668", 262144, "AmigaOS 3.2.3 [47.115] (A1200) Lo",                        'B', 1},
	{"d7a31bef244715075228465f6d3d126fd7629224", 262144, "AmigaOS 3.2.3 [47.115] (A3000) Hi",                        'A', 1},
	{"9bc15df22a2b6c2132dae4e19a73ec7caf507248", 262144, "AmigaOS 3.2.3 [47.115] (A3000) Lo",                        'B', 1},
	{"770bb69935f18389ab24b7472afbd955e6625c7a", 262144, "AmigaOS 3.2.3 [47.115] (A4000) Hi",                        'A', 1},
	{"a12a13aebc18f09c71ad5c9f6ee5e3d76550eef8", 262144, "AmigaOS 3.2.3 [47.115] (A4000) Lo",                        'B', 1},
	{"be72613b9a5b8ff1c9acad3553993d0b061ec8e3", 262144, "AmigaOS 3.2.3 [47.115] (A4000T) Hi",                       'A', 1},
	{"350157c6668c58dacd73b3bce37570c8af17a2e3", 262144, "AmigaOS 3.2.3 [47.115] (A4000T) Lo",                       'B', 1},

	// Extended Amiga ROMs
	{"ec20b624c2625dbd61be245724570ac64b5d106ed0b97998b0355e1b4dc1188d", 262144, "Ext. ROM 1.3 (A570)",                                      'E', 0},
	{"6e48bb768539e8dae352553e5fdfa860682edaf9e26b105fadd8e26103171765", 262144, "Ext. ROM 1.3 (CDTV)",                                      'E', 0},
	{"fb5f3232813dc132d5befb94e21c35dcb6406a88350e6f25e55e54a8e4636daa", 262144, "Ext. ROM 2.3 (CDTV)",                                      'E', 0},
	{"13fe2e538aa8a3af6cd66410f6000499c64d3753f759598439a0e1a760045604", 262144, "Ext. ROM 2.35 (A570) (Unofficial)",                        'E', 0},
	{"2a7d0fa93ebb4084038c82486da3ad72467a592a4b65f0675a38e7ccd84c7128", 262144, "Ext. ROM 2.35 (A690) (Unofficial)",                        'E', 0},
	{"ee56af802e8284fa18bdf8e5009a9deab4924091f4cad5c0a6886c45b665c422", 262144, "Ext. ROM 2.35 (CDTV) (Unofficial)",                        'E', 0},
	{"d964826432559994ffe329f643fd4e42dbbff313362d9434945725d7119cae87", 524288, "Ext. ROM [40.060] (CD32)",                                 'E', 0},
	{"86bf2470e679cb4c7d9e0c4b93bd4dfebb6d286e49577e20838eb817a26b2988", 262144, "Ext. ROM 3.2 [47.096] (CDTV)",                             'E', 0},

	// Other unswapped ROMs
	{"680e654880c891a3b03e6107ed30b10903efca16c1dac1b4c8e7835f4e37acb6", 262144, "Amiga CRT 3.1 FMV (CD32)",                                 'O', 0},
	{"7b40fe2e48a24a4d3a6f3f897740c786f706fd4d2cb85589957930ced830d08b",   8192, "Amiga SCSI Boot ROM (A1000)",                              'O', 0},
	{"39e88e7acc9e19806ba61817d750a22679dc10ab9beadaded08287247d2ae721",  16384, "Amiga SCSI Boot ROM (A590)",                               'O', 0},
	{"e635cc7a737d672e67c03f8b58527e59704a5cfe87be1fbca6aef01ad552fccd",  32768, "Amiga SCSI Boot ROM (A4091)",                              'O', 0},
	{"14f8f8e69e30c07c0ad780124f2d6404e423bf2759b93c4a8eaae7e799ab8e8c", 524288, "Logica Dialoga 2.0",                                       'O', 0},
	{"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", 131072, "Ralph Schmidt Cyberstorm PPC [44.071]",                    'O', 0},
	{"e72e2fe5aa4887269b3605066d1ce74a915ccf6cb07a3d8a5066af87b08261c4", 131072, "Village Tronic Picasso IV 7.4",                            'O', 0}
};

#ifdef __cplusplus
}
#endif

#endif
