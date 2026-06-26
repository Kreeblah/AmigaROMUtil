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
	{"67d834ce9d4959e1716d3d83b27e5c3acba0896e9b2538ea205fc1681edf6b88", 262144, "AmigaOS 1.3 [34.005] (A3000) Hi",                          'A', 1},
	{"eb35e434c97341a8efd5e450f0efda3389a459d93268fc49886b021df292e919", 262144, "AmigaOS 1.3 [34.005] (A3000) Lo",                          'B', 1},
	{"1061cccabccd5615ae1c79de17f228c2f6d39297301601e8b87cd2e0e5156c91", 262144, "AmigaOS 1.4 [36.016] (A3000) Hi",                          'A', 1},
	{"0b17e1399b6ba9f71e13f02a7f11dd34ebfa0ca16e21a5b8afb5103ea0058c54", 262144, "AmigaOS 1.4 [36.016] (A3000) Lo",                          'B', 1},
	{"746a66dba57f4500a54e2c4637e1a8fb2c8279f156cb231b35f0d7d13e503ccc", 262144, "AmigaOS 2.02 [36.207] (A3000) Hi",                         'A', 1},
	{"252306bc00163005518af9792020e3775ce660835b20fbd726296efd4d9a6570", 262144, "AmigaOS 2.02 [36.207] (A3000) Lo",                         'B', 1},
	{"db39375da4e0c6f4d7a76cdfbaacf8bcea07076af0ad7eab545a2e18a3b63cb7", 262144, "AmigaOS 2.04 [37.175] (A3000) Hi",                         'A', 1},
	{"a8abb7f1e8e27d1f6c65500b3547a34032888e0ec5bd76b965392f17909e7cb2", 262144, "AmigaOS 2.04 [37.175] (A3000) Lo",                         'B', 1},
	{"63d993acd48fe73ea0ec7f2e90153c72a1a926a6341e677845fe82ecce8d6a18", 262144, "AmigaOS 3.0 [39.106] (A1200) Hi",                          'A', 1},
	{"89eb27cb5afd8eb9386fd00b0509276f05bf545b770c36e4172d3f2ae69decad", 262144, "AmigaOS 3.0 [39.106] (A1200) Lo",                          'B', 1},
	{"836ff02aae5fe496f0b174a1c85eff37ab728c5810026214b4b9a21bc12081c0", 262144, "AmigaOS 3.0 [39.106] (A4000) Hi",                          'A', 1},
	{"51a7bc2e775a97b04cb9e123641095f1a2d8123a5cce19d67037886bfea428df", 262144, "AmigaOS 3.0 [39.106] (A4000) Lo",                          'B', 1},
	{"933e901fc7546ac6a4e5a5e0b4ad730d536109fc9b08338b818835e2be8fe15f", 262144, "AmigaOS 3.1 [40.055] (A3000) Hi",                          'A', 1},
	{"a3f75c7958d0d0b0582fdec51f49a1315af99932185b594b032083329fbbd840", 262144, "AmigaOS 3.1 [40.055] (A3000) Lo",                          'B', 1},
	{"84e468dc6fdc04f3584008b5ac770ce97c1e5e6b57a1c61d24da3ad61acec081", 262144, "AmigaOS 3.1 [40.060] (CD32) Hi",                           'A', 1},
	{"aeff7dea66ba1dc52f1b037dd09046084baa6cffe97e83f6db000585d48d4bb8", 262144, "AmigaOS 3.1 [40.060] (CD32) Hi",                           'B', 1},
	{"b2e2fd6002069094f650cdd705b3f1ecc72dfd345518c3470f6775e1c17dffda", 262144, "AmigaOS 3.1 [40.068] (A1200) Hi",                          'A', 1},
	{"64bd1cbe9567088296d832beda5f0a46ec58e4be6ebc6e1389e4d07c5ec343bf", 262144, "AmigaOS 3.1 [40.068] (A1200) Lo",                          'B', 1},
	{"777dda55728828a24e1e84560addbe74c1f72d39c3be99e5f443e1b393740bd0", 262144, "AmigaOS 3.1 [40.068] (A3000) Hi",                          'A', 1},
	{"7cfebe139e2b74ff80fc402f1e503c9c3890eaa16d8abfeda6a49758067235c7", 262144, "AmigaOS 3.1 [40.068] (A3000) Lo",                          'B', 1},
	{"416023b251eb519682794b53733dd2de9506a59e057d5d19ac5693775cbdd1ff", 262144, "AmigaOS 3.1 [40.068] (A4000) Hi",                          'A', 1},
	{"e13fcfdbc3b6ff829aef6f491107eefe73e04d398d8fa20db735e28920d52270", 262144, "AmigaOS 3.1 [40.068] (A4000) Lo",                          'B', 1},
	{"184bde33f23bf91de34d72a0ba0741f9338729bdb2d6bea44f317af62f397d51", 262144, "AmigaOS 3.1 [40.070] (A4000T) Hi",                         'A', 1},
	{"d68779b4c9962dc80ed57cdba8bce3bd89db575fbabb8d3aa3e914b8424add7b", 262144, "AmigaOS 3.1 [40.070] (A4000T) Lo",                         'B', 1},
	{"ba5b08c5c41b0a80531095764838c850daa526f0651e9fb043255689a4abb464", 262144, "AmigaOS 3.2b [43.001b] (Walker) Hi",                       'A', 1},
	{"4109af0dc1f37ad8dc1240503c19826f8edd5923a02aae1c89066c97986fae93", 262144, "AmigaOS 3.2b [43.001b] (Walker) Lo",                       'B', 1},
	{"228cb8071a8dff68dc91123a28d6ae2db5d7dc15e66be8d64b59f4605de506e3", 262144, "AmigaOS 3.x AF3.0 [45.057] (A4000) Hi",                    'A', 1},
	{"f8a4920830f0c93570b55d562f6e261d431d28c5a4eb3775ac347088453d39d9", 262144, "AmigaOS 3.x AF3.0 [45.057] (A4000) Lo",                    'B', 1},
	{"9b683f8318f612860becccab965e41f279396b927a1d624cf6f41e9fcca837f4", 262144, "AmigaOS 3.x AF6.8 [45.061] (A4000) Hi",                    'A', 1},
	{"b69d0ea7a53dd4a75707115de448be9d66a02478e98f7136cfaad3921148d895", 262144, "AmigaOS 3.x AF6.8 [45.061] (A4000) Lo",                    'B', 1},
	{"8253e463f188197359eca9a9dd072bdceba56309f85688f5d5d5099cfe8db858", 262144, "AmigaOS 3.x AF7.0 [45.061] (A1200) Hi",                    'A', 1},
	{"0e59136a7ba059f974f0626b9b98668643d2a0e3f5451092c9f25fd73293fbbe", 262144, "AmigaOS 3.x AF7.0 [45.061] (A1200) Lo",                    'B', 1},
	{"fba6776f9a0a3918de471808d9fc2dda576952fe2a8f5d6d9b686412ea2096f8", 262144, "AmigaOS 3.x AF7.0 [45.061] (A3000) Hi",                    'A', 1},
	{"4ab8672e310c18f7572ca687424e2c7b4dcd7b003b66049a11f5e0cb20ac34e9", 262144, "AmigaOS 3.x AF7.0 [45.061] (A3000) Lo",                    'B', 1},
	{"d84140994c7a88dff7239177df40e2bfa51ea17df767ba7b483883e91d753ef9", 262144, "AmigaOS 3.x AF7.0 [45.061] (A4000T) Hi",                   'A', 1},
	{"eafabe0af25880cf21d0c800c6ab12e39d7e870ce1370ab88a22572ff568af74", 262144, "AmigaOS 3.x AF7.0 [45.061] (A4000T) Lo",                   'B', 1},
	{"0087e9b204162cdc8a72cd0a86a67eb9518810aa1b9f9574c6377ea08b207071", 262144, "AmigaOS 3.x AF7.1 [45.064] (A1200) Hi",                    'A', 1},
	{"21f9f12e5058dd0fcac4cf83fc0b927c57c57ce8eb4daba02467ea1ad2b6cf8e", 262144, "AmigaOS 3.x AF7.1 [45.064] (A1200) Lo",                    'B', 1},
	{"84dd963b7d920bf1c03e24d27d9bdb8fdaa4d9f28bf5015c39a08fecc388ad28", 262144, "AmigaOS 3.x AF7.1 [45.064] (A3000) Hi",                    'A', 1},
	{"e206bb4580ffe52d0404563209939cfd5de965baf5e8fe61f73490228497baad", 262144, "AmigaOS 3.x AF7.1 [45.064] (A3000) Lo",                    'B', 1},
	{"990461cafbbb990504b25f20dd42a5fcb07de96fdd5e51d10c36ea12b46c7cb8", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000) Hi",                    'A', 1},
	{"ae791deae4fe012c762baa90cba1fc61a95376bf169a4db4fa6c9cb65c02192d", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000) Lo",                    'B', 1},
	{"e84e756494df389e723d97b66f176eef40da205c4def7d1fb0e1149c6ee067eb", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000T) Hi",                   'A', 1},
	{"2ae9fc90fdf019e7e15006d9cad63d0268ed7b2144ebf97ed998ce06da3bdec5", 262144, "AmigaOS 3.x AF7.1 [45.064] (A4000T) Lo",                   'B', 1},
	{"bad9eae21d4b8cd477e1b4d174d10b195c8c371a2690d565d04e6056dc862399", 262144, "AmigaOS 3.x AF8.0 [45.066] (A1200) Hi",                    'A', 1},
	{"5a748b91b6273170722782f9b09789b0c6dbe46b4d60cc23ff2e15ae5972e4f3", 262144, "AmigaOS 3.x AF8.0 [45.066] (A1200) Lo",                    'B', 1},
	{"6e8ea1bd878bf1f6c588a7366d08aca00c9a25705d8685ae29983bb644403846", 262144, "AmigaOS 3.x AF8.0 [45.066] (A3000) Hi",                    'A', 1},
	{"ace7e04e1e6ea49f0314815a0a42b1a3a90a0fe0c0afe8c32c2ade8008111588", 262144, "AmigaOS 3.x AF8.0 [45.066] (A3000) Lo",                    'B', 1},
	{"0110b8566785b23b4399c944152a94506e0762b4333126ae76358028d5ff78d8", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000) Hi",                    'A', 1},
	{"437dd1615ca394b69219106b0d37b7f1cf3a40ea4c7b21df78b1e47d8a8f350d", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000) Lo",                    'B', 1},
	{"2ac6ec577eb873091ae223c131af99acaa1cb8a5fcfdd1e71077f26a44c217ad", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000T) Hi",                   'A', 1},
	{"e05c2a50edefe21655c2299550feb90c9daba2bd93daae55ad1e8921129e5187", 262144, "AmigaOS 3.x AF8.0 [45.066] (A4000T) Lo",                   'B', 1},
	{"a1d374e94732e80d9c83abc0cfa4d7d9ed050d08eb899f01a8d51f23cb3f8578", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright) Hi",        'A', 1},
	{"35acee27d5c7410ef0a57a8911cd4eaed4fdd7eef85b29ed674bcc590aa276df", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (Old Copyright) Lo",        'B', 1},
	{"4dc3623de84015bb254fb356f8429e359246eacd1885e321ca4520b0a5268f31", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright) Hi",        'A', 1},
	{"50b17f15e3fab1da82ef3e42e13cc1ade2cc4fb4e32a16c7b31bab41c2583841", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (Old Copyright) Lo",        'B', 1},
	{"4402d90c0ab5baa325c8caa7824278300e69b9ec0905d24d9fe3b759965f56c7", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright) Hi",        'A', 1},
	{"c794cc5272a0f20b76b18006454ef57093920e80a84239521ba001881e7ddb6e", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (Old Copyright) Lo",        'B', 1},
	{"e2a93d34508ab8b1f923b160a0478033816aebdfa4cd5e75af64e23238fe6d47", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright) Hi",       'A', 1},
	{"96e42803e22a5b05674e5f3ada3c393b2ed7c24747db214c227a2ec508616916", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (Old Copyright) Lo",       'B', 1},
	{"b47f7684b017c9fc8e11680f85e5e4eedf0b4adcb84273c934fae4e9cac64205", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright) Hi",        'A', 1},
	{"530c0f3e0e872b3524d84a3cda2243fd351d45094e980194af5e34736dadb333", 262144, "AmigaOS 3.1.4 [46.143] (A1200) (New Copyright) Lo",        'B', 1},
	{"2d69745a43a33233b968ba93bd3dc8c724fef5f3cd422b76b54a7dc9206afea9", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright) Hi",        'A', 1},
	{"16effa14e34855524df6d0d053db3bee14fd97de5f6125d25e5558f620baaa14", 262144, "AmigaOS 3.1.4 [46.143] (A3000) (New Copyright) Lo",        'B', 1},
	{"6dbc2567e9e69fac65b9ac8cf8cc0879704046f5ca4ab53d7b8ac48814934f38", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright) Hi",        'A', 1},
	{"d3e82728561389f5b08e372654ac930a2391e284e253227e7fc52955508708d3", 262144, "AmigaOS 3.1.4 [46.143] (A4000) (New Copyright) Lo",        'B', 1},
	{"c9075252ba668cd20e283eb4949af7bfd280c6f255106cac8c9bb8b240fa419f", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright) Hi",       'A', 1},
	{"2b6fbfbc9f43ad62517d0f37e9a5d9a6a5e04dd71aeecd556c2f603c6437be1b", 262144, "AmigaOS 3.1.4 [46.143] (A4000T) (New Copyright) Lo",       'B', 1},
	{"093afb8b5a3dd2874015cd8131afcbde4bf0396333d16c28cc7b0404674bbcc2", 262144, "AmigaOS 3.2 [47.096] (A1200) Hi",                          'A', 1},
	{"8284eb47b0079a78e1d7f777111f61e46eff233b45f0259ac1d3f1e5ee862293", 262144, "AmigaOS 3.2 [47.096] (A1200) Lo",                          'B', 1},
	{"ba8fa629b45f5946265aadc800164ced8596cffffa542ebe33166ed4e942e830", 262144, "AmigaOS 3.2 [47.096] (A3000) Hi",                          'A', 1},
	{"91d0c24d132861dd7486af437e7cae38eb1e28077330858f8a43b218d3fda74d", 262144, "AmigaOS 3.2 [47.096] (A3000) Lo",                          'B', 1},
	{"922ffb2d3ddbc0b8ef89d8ddeadb1a5a689c7236c9b5ef7993b3dd9f50e9b3ec", 262144, "AmigaOS 3.2 [47.096] (A4000) Hi",                          'A', 1},
	{"bce336123658c050c97fddfb6a1afe2a956c296c5a8407b04f0c891aba5d30e0", 262144, "AmigaOS 3.2 [47.096] (A4000) Lo",                          'B', 1},
	{"05f607b5d342b8b9585a208ccbceaacd8051c24f789bda862b2075e4df1a64a5", 262144, "AmigaOS 3.2 [47.096] (A4000T) Hi",                         'A', 1},
	{"b20b65c0aa10fe850d376e29cbad72746267eb055888c27a2d5b37f5d675a27b", 262144, "AmigaOS 3.2 [47.096] (A4000T) Lo",                         'B', 1},
	{"a6ea2f5d791951f00e0a8c9a4e0fb5034827f45cd3488ee514d2118fb89d9cd7", 262144, "AmigaOS 3.2.1 [47.102] (A1200) Hi",                        'A', 1},
	{"5d97b789e46b1df1c8b9354f363eb07f542bc618f79fb4ce4bfbfb93e448dd85", 262144, "AmigaOS 3.2.1 [47.102] (A1200) Lo",                        'B', 1},
	{"673a123cd2efecc944bf242b669378186a260e0fcf7b06ec7eb840d7b0362d69", 262144, "AmigaOS 3.2.1 [47.102] (A3000) Hi",                        'A', 1},
	{"e8a2fc00ec2ba3b274112dd9b32f6df8d285829c3254e083cf448474e46a5465", 262144, "AmigaOS 3.2.1 [47.102] (A3000) Lo",                        'B', 1},
	{"e5b65082e607172bb630b7ec6ad33a69bdc675953a602fdab7609ebb60861fae", 262144, "AmigaOS 3.2.1 [47.102] (A4000) Hi",                        'A', 1},
	{"011959b14b57c618dd499018b541c3982291ddcae618ae749aa55c867e068bd8", 262144, "AmigaOS 3.2.1 [47.102] (A4000) Lo",                        'B', 1},
	{"5c8cb50b7e44ca759db0168dc25cea4a90bd7c2fb2dc14b1ecb67b7c6d8767ce", 262144, "AmigaOS 3.2.1 [47.102] (A4000T) Hi",                       'A', 1},
	{"49814b571185f25e3a80d442dd72fb268cba387068132d63d0259ee81a311db8", 262144, "AmigaOS 3.2.1 [47.102] (A4000T) Lo",                       'B', 1},
	{"21c2e0e407a684bda34038bef855d9d3e87dc66c7fa6add3957c7d3af751f2d1", 262144, "AmigaOS 3.2.2 [47.111] (A1200) Hi",                        'A', 1},
	{"13d9aadd2b97dc7dcd66d20baf51a8338288566e30b42fdcde16bffc307724d0", 262144, "AmigaOS 3.2.2 [47.111] (A1200) Lo",                        'B', 1},
	{"c1a79a2f467ae599df8941362004a2580a0c6f0dace28c1e2c74b472162ab1c8", 262144, "AmigaOS 3.2.2 [47.111] (A3000) Hi",                        'A', 1},
	{"58df010f585f059fd6c888544b42b5e0753cf9ca97c058aecddb6f6dd47ef14f", 262144, "AmigaOS 3.2.2 [47.111] (A3000) Lo",                        'B', 1},
	{"6fe5e7feba9b0a17d2dbede4a2159b9b8eb2268d6af43454e3204dec3bdd006c", 262144, "AmigaOS 3.2.2 [47.111] (A4000) Hi",                        'A', 1},
	{"8b7067de2e5f68c59783d6ec732d7abdc63cd4b06ef2f5f781bbf03fac64c078", 262144, "AmigaOS 3.2.2 [47.111] (A4000) Lo",                        'B', 1},
	{"983e8ddaa9ab6a4bac17d376894feabeaf059575264b4a45b982722e9612e947", 262144, "AmigaOS 3.2.2 [47.111] (A4000T) Hi",                       'A', 1},
	{"0a188b59b657496ea5fcc9d5e84a2152f5c3aaeaf270a313116ce80e77941ef7", 262144, "AmigaOS 3.2.2 [47.111] (A4000T) Lo",                       'B', 1},
	{"f0a160b5a15e854f9a8eeea1df278383cba38ee0386463b9446bde78e420ad71", 262144, "AmigaOS 3.2.3 [47.115] (A1200) Hi",                        'A', 1},
	{"a3af3510873e4e5bcf9c9df871a7899a6ef6335bb10fd397adacd5f84c50ec04", 262144, "AmigaOS 3.2.3 [47.115] (A1200) Lo",                        'B', 1},
	{"0346ea674f205b64248663ec5d58a8ec50ae8a8bc23abb1cc652b9f3e367fbd5", 262144, "AmigaOS 3.2.3 [47.115] (A3000) Hi",                        'A', 1},
	{"02ee8f146c84343a330959385b05bfe4a0afb98672b3b1872439435ca6fabf75", 262144, "AmigaOS 3.2.3 [47.115] (A3000) Lo",                        'B', 1},
	{"5b72192a22d10f3eae7cf8b6146d289165fa72dd0fabb032e115f010fd6ea3d5", 262144, "AmigaOS 3.2.3 [47.115] (A4000) Hi",                        'A', 1},
	{"ca46d91c45a9a4f1b329b1de26d1fa6a3b3d1ffc0d417aa2d47376c404f05da6", 262144, "AmigaOS 3.2.3 [47.115] (A4000) Lo",                        'B', 1},
	{"36678e75201ee966172d02372cc31540e0dad64f86ae328f4721046a33ac92cd", 262144, "AmigaOS 3.2.3 [47.115] (A4000T) Hi",                       'A', 1},
	{"9377b52bae070af87ccde491a48b3c11a78a7f2c082eba3a4c101103f609dada", 262144, "AmigaOS 3.2.3 [47.115] (A4000T) Lo",                       'B', 1},

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
