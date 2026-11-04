# Lygiagretus kasimas
* **Gijų valdymas:** 5 blokų kandidatai kasami lygiagrečiai, pasitelkiant C++ standartinės bibliotekos gijas (`std::thread`), kas leidžia vienu metu vykdyti nepriklausomas paieškas.
* **Saugus sustabdymas:** Naudojant `std::atomic<bool>` ir `std::mutex`, pirmajai gijai radus tinkamą darbo įrodymą (PoW), visos kitos gijos nutraukia darbą.

## Palyginimas su v0.2 versija


### Rezultatas netaikant threads:

1 - merkle testas, 2 - blockchain sim
2
Sugeneruota vartotoju: 1000
Bendra pradine suma tinkle: 506867023
Sugeneruota transakciju: 10000

Pirmos transakcijos pavyzdys:
Siuntejas: user_229
Gavejas: user_631
Suma: 1861
Seka: 0
Tx ID (maisa): e05c9d354bae7fad90789ec98d21571e4084ca10acb7fe0b4c4dee54264aa355

Genesis maisa: 56efd83362c25b48781929daf1afe4ac47b8987ad5c6ab14d89ad81cd4b9e127

Matavimas bloko kasimo:
Iškastas Blokas #1 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 250 | Hash: 00046ee07ab7fac6df42d658563313d307a440ad8316d8e70704a30a86aa271b
Iškastas Blokas #2 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 1257 | Hash: 00064ab78afe91f8a7835733b80d1b2250ec29b8e50531a6462d2a0832b0aa30
Iškastas Blokas #3 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 1653 | Hash: 0008f1db5c6d6e9795345cbde93ff6e15a8885304444b818d3bab849b1e4f8d4
Iškastas Blokas #4 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 140 | Hash: 000fde28ae69765162baed394b3db9b08904224fad1137ce977c1fb806ddb4d7
Iškastas Blokas #5 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 1226 | Hash: 00042747ffea6cb7205fc366201d4b0bcc4d3cc510dab5036b79f3d8711eb628
Iškastas Blokas #6 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 686 | Hash: 0007c750549a358fa82410400c48b05c129b8b2688999fc7104f0fd982afa182
Iškastas Blokas #7 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 1370 | Hash: 000dfd993281e5dff4e7db87bb9854da732acf6fdfd0516dd21757eb5912392a
Iškastas Blokas #8 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 1408 | Hash: 000cecde881b188e26b763e10b14dfdaee465c8c50d6560c2369d893c42f30ea
Iškastas Blokas #9 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 563 | Hash: 0002f24705e61bfba8c0e611d9e4a0cdcef77ae0bd4dca2738d97702893cbf97
Iškastas Blokas #10 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 501 | Hash: 00097d08437912b7dcbc993bf166310fc7b940ecb67f28546d78f4f3ccf909e5
Iškastas Blokas #11 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 500 | Hash: 000ced6f07bcae6261d95f70b7c265e3fa917bcb667ac07318b6174b9b3fc4af
Iškastas Blokas #12 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 1260 | Hash: 000d9fce9240fd2742619cd354ab1cb6af88b93a783f4682a80e26128f85cf28
Iškastas Blokas #13 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 596 | Hash: 000c3eb02eac4b0e85a27cddc32f02ad5226bd7cf2fb4797879b3fe715935584
Iškastas Blokas #14 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 3001 | Hash: 0008c2f8ed13c19e77986b221a6f9f835debc35c4b20af9a7d7acf2f203773f8
Iškastas Blokas #15 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 2566 | Hash: 000ace616f75049906b8ddfc1d996867bfd8aea2ceff16decd08029323496373
Iškastas Blokas #16 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 357 | Hash: 0001113126d6c3ce3a23651e5eb35801938298029f6dd6d47b13e6ba1d8c411c
Iškastas Blokas #17 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 365 | Hash: 0007bd1122a7414c74b4a370d292d4a5817ecc8013f7e7fb36ec5ad342a168b3
Iškastas Blokas #18 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 403 | Hash: 000ce9d6056e5e380407e32fd0b0a2885d40515ee3b0fa89f0954e5f4603576d
Iškastas Blokas #19 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 318 | Hash: 00018ca42593a60b00d17471eacd2f510c21ee5178f122afd8f2056b48888e49
Iškastas Blokas #20 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1507 | Hash: 000be5d6668f9b3b0360c178f28e94f050f4788dd485f9045e95810b07eef2f4
Iškastas Blokas #21 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 501 | Hash: 0005ea50b3b671452172d1453557379bacd95032784e131f8765c81348c11fcb
Iškastas Blokas #22 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 2741 | Hash: 000f2fadd6240c84ab9965ecf482080086b86c8131db7f77d61ca46c4a621beb
Iškastas Blokas #23 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 956 | Hash: 000908406ade80b67c930694f761c2452c4f289dba12725124102f195ac8020e
Iškastas Blokas #24 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 723 | Hash: 00009d6ad926f601dd9481d195d810015c67fe96a88f8d43c65f0511a6e4b912
Iškastas Blokas #25 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 1406 | Hash: 000b950ac1e3eea6e678927e2e85a7b4146c4bbf690be7b54336ca26e0ffcc0d
Iškastas Blokas #26 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 1654 | Hash: 000c64073b9ec7bc5d0d8f9163e6598e6855b310e8951ca26c33dfe7ebefba45
Iškastas Blokas #27 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 917 | Hash: 00091d5a8b06c89db84aba8e1345ebfe517628ab7b621d71327baee62bb1ebcc
Iškastas Blokas #28 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 160 | Hash: 000e4ea534563847cc4bccf0c0ce10dc97092889319ea7ddd2cb878f1bc5201d
Iškastas Blokas #29 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 1629 | Hash: 00083c6549ca83ad11a36a7f4efc2cd2d6c621d964f04cdb81bee0cccb4d0f8d
Iškastas Blokas #30 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 4802 | Hash: 000098ac705a1b303b3e44af6c32546780176415d0ef41aaa6cf90eceb508198
Iškastas Blokas #31 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 630 | Hash: 000e2ffd170c32635131f024481bebe9cabe23093bae97cd5779529823a85f86
Iškastas Blokas #32 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 399 | Hash: 0001f384a07f9cc8528617378465ae1deca5aae47aff0b496fb6143a722104a9
Iškastas Blokas #33 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 698 | Hash: 0006bcddb437dde780d88fc8e17b6977d0e93d8be55d8a619bfce1d06b274615
Iškastas Blokas #34 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 627 | Hash: 0003dc1309a3341bfe938a9630286b50e763c305055177c9fa584c97efdd7335
Iškastas Blokas #35 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 930 | Hash: 000a101eeec526d5ea6dc95fc4881b867f9b86ba95962cd3cce1a09c2b895db7
Iškastas Blokas #36 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 147 | Hash: 0001278f03f01883958ca163348d4b28cb214ab80b6bdf3bddb8238a4bc9bb02
Iškastas Blokas #37 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 1892 | Hash: 00094472129b5ddb3039ff6963b7844114f3796996aa0bd42e4d5438b5ea5874
Iškastas Blokas #38 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 688 | Hash: 000e77cad1dc726cdf6fa7c38403ba76ae352d1d5941bc5993b3d8201cdc5270
Iškastas Blokas #39 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 657 | Hash: 0002729803638614a02616a2c30898ab925403e4fca91aa6f7a372b25daf3de6
Iškastas Blokas #40 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 53 | Hash: 0009890abb8fe82b02a858cc3094e0d06284d4b8455d6abf052adc54699cfc79
Iškastas Blokas #41 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 882 | Hash: 000a17f8ce839aa9bc2151d3d07eb37fcb8771a3a28d06353593308f4d6bb723
Iškastas Blokas #42 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 418 | Hash: 0009222b63eb5068d7e9856ef315c976ec9a22bb4ef1bcd51befec6917bfb0ff
Iškastas Blokas #43 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 938 | Hash: 0008ae4d9900d1181790d7aa0f636430ca9efca88e7f0421f75e84bd66f6da45
Iškastas Blokas #44 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 582 | Hash: 0000df492cc1b18300ae2d4ef7f62839bdbc70cdfcdfa0d29054dad395af8e2d
Iškastas Blokas #45 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 844 | Hash: 000012113d9e9c437bfe03d3be590fc7a26f779f11ded210d61a995db5ca939f
Iškastas Blokas #46 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 1281 | Hash: 000ec364ec0c14573c62b5acba89bddb635c762de3a41ab49206581779d03695
Iškastas Blokas #47 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 326 | Hash: 00035bd32f378eed544a8e613493f8f238c9e729be34f7f058188900bbcadc26
Iškastas Blokas #48 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 673 | Hash: 000258ec53f70a68b1abf8f519381fa3c5d5c512de81ae9df9b0c620fbfbcc79
Iškastas Blokas #49 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 460 | Hash: 0009b5e23ea1aafe7a9784237f8eb1a099f8ae723df6eda108892a6c96189238
Iškastas Blokas #50 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 2142 | Hash: 00062da036051a5828de368c1707d7621b5b8799750bad27fa02f39ae8a66c9c
Iškastas Blokas #51 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 1213 | Hash: 000286aab0c2310ad9258f01257264e05b866e57494cba01ba8a1ff4a263b69f
Iškastas Blokas #52 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 2376 | Hash: 0006038e52d68fb189259ccba4b1625a88c1b0da55d2b9cfae4c9bfe4a84c93f
Iškastas Blokas #53 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 17 | Hash: 0000738d6b9fd87083caff98d3bd35fe6dfa9d2fd54daa56912f10704d16a2f7
Iškastas Blokas #54 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 204 | Hash: 00020389b4ea35f555c922b55070269cd2d48341849b2b846dbb029ec88e41ae
Iškastas Blokas #55 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1047 | Hash: 000885123df720cf7bc1f413d3a8311c345a78d4d9217a903ddcc11d326c863c
Iškastas Blokas #56 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 647 | Hash: 000256832f8fa15da13e3d33d4db87e3195df603f49992aa09d4534f4aee333a
Iškastas Blokas #57 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 1910 | Hash: 000d87cae5ee94c2306be7c327bae3d613af2aef4b83a1f549ecd6da869914c6
Iškastas Blokas #58 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 607 | Hash: 0009243924f34b1c3c1f57fc02dc260e58f496a5e49442125f3f486a740edfe3
Iškastas Blokas #59 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 34 | Hash: 00038cfcda80f0621a09db06b729d6871dba29b6c00cae632f2b4bd1385438d0
Iškastas Blokas #60 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 2176 | Hash: 0009f2f5a7aa9d4360b8bd95acbb8582dd6803c46fbd1afec26e6d0d23e45f08
Iškastas Blokas #61 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 437 | Hash: 000d244db007a7e4da40a0f05a7d90082a2ecf779c4370d51a8e7fcdfdf76b9a
Iškastas Blokas #62 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 327 | Hash: 000eab25b1d157d32d3701675781278e993aa2c7856d3b5b8df147ab14949c6a
Iškastas Blokas #63 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 289 | Hash: 00029242e7749223702c2db6b1e58d050c798b733bdf88ca1cf42d0160846cae
Iškastas Blokas #64 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 1691 | Hash: 000de883e373320fb9c530ef07127c160487487d3c5abf0bb864a44cc017da84
Iškastas Blokas #65 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 68 | Hash: 00027fd6fd60ae745b8bf348e8494c1e3fe584c6427057876317e155e241fb05
Iškastas Blokas #66 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 110 | Hash: 0009a2d467c001f66c681be4e7fe3b2985ced7a46a24564353fb106c51de577f
Iškastas Blokas #67 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 698 | Hash: 00012b3dab3f5cbb40df2ddc0f42c067360b1d0fb530e1a68a8db1e41766c069
Iškastas Blokas #68 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 348 | Hash: 00035d20198a7e465a9e34175def23c8e38a605a098bcaa3c1d44717cf96a0eb
Iškastas Blokas #69 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 2055 | Hash: 00074a2a4ef5dc2f9fe1077e084013ccace6d8e5391569b195e35612791eca2e
Iškastas Blokas #70 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 141 | Hash: 000d35acee7668c6c538f35e0c0e45ce3e518292659c220c568adf12d17b8b1f
Iškastas Blokas #71 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 874 | Hash: 0001037586c4451d6e5488d96c437868ccebda54aeac239b99057558810f0559
Iškastas Blokas #72 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 1707 | Hash: 00064ebc6a5c4c24818143909205e7f023e7ece5bea67887f2afd7c22e8d7a2e
Iškastas Blokas #73 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 624 | Hash: 000556fbfa3599037ce8c9bde3179499c3f449b4b6abe26cf16fc6fab369be70
Iškastas Blokas #74 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 182 | Hash: 0007402e661d8d9bafc8b3c447a88b4457fbdde83f86bfd9e3cc4a1bd03ca7e9
Iškastas Blokas #75 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 3695 | Hash: 00071b921b41c8aa1a6871c47b840906eff9550189192dd68210ded3aa328457
Iškastas Blokas #76 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 554 | Hash: 000f8d45a85b44e47e1391062ecffb2e6071b266027271abcfef56c07fec122c
Iškastas Blokas #77 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 518 | Hash: 0002aea91e6f5cc6c4427039eccc645e6baef6daf8a2c8ed7ffa767ebdea38fc
Iškastas Blokas #78 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 197 | Hash: 000dff2107442957a48adf42e9a363d3d9d039287e473d1a5ca0eedeb9c908a7
Iškastas Blokas #79 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 22 | Hash: 0002e3d994de9deb57a62edb72ec3327f314b570580fe47e31e2780bdb83b50b
Iškastas Blokas #80 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 903 | Hash: 000b75e721138a08e3a7021c727e3f1aaef774e95a2e888507b7cce98e4e9c7e
Iškastas Blokas #81 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 64 | Hash: 000441fa7451c9931f397040bfdbf9d96033e88fe03648c1ac9888623596ca68
Iškastas Blokas #82 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 9 | Hash: 000a1651bfe434e12feea38c7c7d60508c7765054781c0799b38e9f64f6b1bc0
Iškastas Blokas #83 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 108 | Hash: 000d4f0c9718ad50cbbcecf444ee3f381a23d08c5d89f6cfad626b89c5a28e78
Iškastas Blokas #84 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 2311 | Hash: 000f0fdaf2f4d55871e7d68e0d9446a2f0d872775079dc2dee49bd708bc38f06
Iškastas Blokas #85 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 891 | Hash: 000d88ec95c9ff81f583a0cb597aa1589314a92d868bdda2d79b25cdce5af0ed
Iškastas Blokas #86 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 3856 | Hash: 000715413e57064e7b9a2fb8b49f6d81ff0339a9862074d8919fc273206479e6
Iškastas Blokas #87 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 203 | Hash: 000990fe8a90a7c8997b50f9d043d8db14526c2b1664feee863588295487d629
Iškastas Blokas #88 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 161 | Hash: 000c795f935aa41f038f970c406d006fb87771d7d896d689848bb34dbbd3e12d
Iškastas Blokas #89 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 275 | Hash: 000aec0501e8ef30b6c662decb4429f84898e3753dbe10f4dc40330fcb72fede
Iškastas Blokas #90 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 198 | Hash: 000b2b8cfc5042b9b123c9c3313bfde06240a55fc000f63a157a656343a00577
Iškastas Blokas #91 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 614 | Hash: 0006b4ee3f1c3ed2acfffb84684faeeb61f10a260b383eccba1cb911a0e68c29
Iškastas Blokas #92 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 386 | Hash: 000e7768bdf70ec95f1d88d7ab1a9692b10cf5f707d50961d2cf084d028cba0c
Iškastas Blokas #93 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1669 | Hash: 000b1fe88a3ed12a0ab4c50f2d22bf50e3fea635a3ce32ac2b939b9d60e58419
Iškastas Blokas #94 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 2197 | Hash: 000fb8e19df91afbfba02bd0b7eacc6e4db75598193501e94b1371699f3f8339
Iškastas Blokas #95 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 44 | Hash: 000d9d2a9f84974f46b6fd98f74d8b18ec1af7ff7f199ced497e46c6d374c256
Iškastas Blokas #96 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 57 | Hash: 00076e767da1a94ef5fcb1ca66a75b97d30c08af457ee2a64f9ea7ea3912537b
Iškastas Blokas #97 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 1510 | Hash: 000b768be5ddd01b83d77176aed50d1579f9f58a663c56389cd8581687f4ec10
Iškastas Blokas #98 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 224 | Hash: 00097e12827e12adacfde86cc5e70a17db776183eb5f18d0ee573a18c899b2c7
Iškastas Blokas #99 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 950 | Hash: 000cc5c8d5562e9f310e154a125a6ff901d58105653d81160f3af923e9bc6048
Iškastas Blokas #100 | Laimėjo Kand: 2 | Tx: 73 | Nonce: 244 | Hash: 000bc43321eb0c1a8d448d74973d584ec00d1f878af7e168839c56caddff5b16
Iškastas Blokas #101 | Laimėjo Kand: 3 | Tx: 2 | Nonce: 505 | Hash: 00067ace3ba5659682ef89f271e3da164111000ecbae9b9cf4e343b0a6f42388
Priimtu bloku (su Genesis): 102
Atmestos transakcijos: 25
Pradine suma: 506867023
Galutine suma: 506867023
Pinigai nedingo
Visas kasimo laikas: 4.93865 s

### Rezultatas taikant threads:

1 - merkle testas, 2 - blockchain sim
2
Sugeneruota vartotoju: 1000
Bendra pradine suma tinkle: 500062293
Sugeneruota transakciju: 10000

Pirmos transakcijos pavyzdys:
Siuntejas: user_99
Gavejas: user_700
Suma: 2372
Seka: 0
Tx ID (maisa): 606ce06dcdf42f98411f307e89c1b844a361306bfba64a0f4ea575f4c6368eb7

Genesis maisa: 754dc6d21ade2a5d7198671c63ba3f2e719a585f894885f772cbd13e345b5ee3

Matavimas bloko kasimo:
Iškastas Blokas #1 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001991 | Hash: 0002a09b409c9508d7814a9754d8cbb4a6dbf23749a88aba42aaa14611250d78
Iškastas Blokas #2 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000032 | Hash: 0005b02d4a1c3a16e4192d8f532a621342bb4ec56686ca02f546f9ff88846503
Iškastas Blokas #3 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 744 | Hash: 000cf8633afc908ba1d2e1aecce2d60f094bd9fcd0a0879ce3baa3579027d605
Iškastas Blokas #4 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000319 | Hash: 000394e820635dd41966390a54cbd95cc6015a1a8dc603fecdd24fdc566f7f5b
Iškastas Blokas #5 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 954 | Hash: 0009c8555da7148275fc3a9108d47d960d2b8e0b586568524bfd8d70937091b5
Iškastas Blokas #6 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20001091 | Hash: 0009db9b608f240b3bbbc5d9695b747b35fa601fd514b9651b68e47d34edc6a7
Iškastas Blokas #7 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001304 | Hash: 0007fa512caf9efd82f7e56c2b567c89e41949fedab2c8733a68eabd43f6927a
Iškastas Blokas #8 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000018 | Hash: 000f2001d0c9ac4e1693206a208377d4f6747e08585324b8d8d2d7b19f2fa946
Iškastas Blokas #9 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000393 | Hash: 0009a46f0dccf6ef40787d91539e6a087773a52b12bc1014f9e8de456cc732e9
Iškastas Blokas #10 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000448 | Hash: 0001130e49a73ef50b5652e633c322821b90f8ab005ddbe07fa509ad7614da3a
Iškastas Blokas #11 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000041 | Hash: 000d425af3669fe29642831a75403f688e2bf93b1808445ae92e0d66166ac814
Iškastas Blokas #12 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 525 | Hash: 00011ea759a7534f1736e93a2026a830d89dd997b25fcaf1471fdb7e11cf891a
Iškastas Blokas #13 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000363 | Hash: 000052233f4fbb3448af952f900460590186496b8d52e36a10dbc6db412edd28
Iškastas Blokas #14 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000726 | Hash: 0004bc2205a8147148785030307b3effceef36845ecaea3e8656101cebce8656
Iškastas Blokas #15 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000707 | Hash: 000745630693d686485e84dc2fbf36dbfaa792fbc319496adfe2bc9c5e5bbaf1
Iškastas Blokas #16 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000176 | Hash: 000de9b7cf09fe1653e2727d14b61015f4e275e8e4afb5d7dacb466782cb6bac
Iškastas Blokas #17 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 355 | Hash: 0007d8a3d4aa31733aab1a239207139ed78fe8e6e9fa3e76592c06c7650f0c60
Iškastas Blokas #18 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000322 | Hash: 000c590fed83ac9a2fdc9dac8991ab7758144fc7bd05aaf0062cca8618afae54
Iškastas Blokas #19 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001820 | Hash: 00040c33402b585d714dac4034642405c58c5388e272090bc11058a8c1827694
Iškastas Blokas #20 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000800 | Hash: 0008e666431295e90c152f4da57572b4d7e7ca87ed6c863a0af50b75899481a5
Iškastas Blokas #21 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000519 | Hash: 0009cb7ffa7e2347fd2f51556c245fd22502d2ba7b3b2007009e7e8f3259d574
Iškastas Blokas #22 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000178 | Hash: 0008624140724da5e985f3fdfcd9a68adae84d281aa1a41389b64336b778eb51
Iškastas Blokas #23 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000422 | Hash: 00070aca6cbc98ad607cab22094da423d1233194197cd431276e4c7fe1a0444f
Iškastas Blokas #24 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000796 | Hash: 000c34f124446f4fab62a8cc90185d71e70bf2c6387a8c16dfdbc24ec9921476
Iškastas Blokas #25 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40002208 | Hash: 000952fa433f595bad41e2f5aaff6b53529fda7a4c6445654c4bbf7e3667d5e7
Iškastas Blokas #26 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 186 | Hash: 000aed295b6f9ecfbb647c7789180312129562ff5a2690bae8d42481a412779f
Iškastas Blokas #27 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000273 | Hash: 000426f721742fe4d212db1571d8da6f7e66db80f5cef177ad8196db1425e8b8
Iškastas Blokas #28 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20001973 | Hash: 000a8902b3490ab003b46ac039b00bb35bc3af1f2c425daf3650c655507596ce
Iškastas Blokas #29 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000583 | Hash: 0003e85c7ba292293dd26291ffedae1ad93afe7b22da420f8c2496e9b609e346
Iškastas Blokas #30 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000310 | Hash: 00071e41d787a5a7e707155f88e775f075a528e71815f65e7bfb88db6559e64a
Iškastas Blokas #31 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1086 | Hash: 000135fc4645865b259f96a6c25ef8d6aa4f68181ffa11b490699c90b218c469
Iškastas Blokas #32 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000651 | Hash: 000f31525423282fa2548e45090bd836bf3aea25ac8017cf0a79eba1afbc33a5
Iškastas Blokas #33 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20002528 | Hash: 000e961a9ca9380b8914d2c9af22a0f2e37dd119af3ea7ae48d80c140e4f0a49
Iškastas Blokas #34 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000003 | Hash: 0006b0bb26a9b971c64ef0110fb2fde34b6aeb4603f25ceeab7f57a5056bb17f
Iškastas Blokas #35 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000087 | Hash: 0008db8fbde7db53d4bd79654112c384de2de274aacaeeed1938e32dd887d6c1
Iškastas Blokas #36 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 109 | Hash: 00035283d0f1a475257be6adf672b8dfb39c04922729c333143265afa5b8214e
Iškastas Blokas #37 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001101 | Hash: 0009603045c528d9b4171bf501b5556e14d5bc137f53dcc9df85022d634d71ac
Iškastas Blokas #38 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000363 | Hash: 00001785c8deb18898cf38d1ee225b7a82c0ffbdf202b132f3da817a35be09df
Iškastas Blokas #39 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1195 | Hash: 00047a3c972739da7bdd3605bcee938c000e8dd3f01a94189c48b716fe1d4ba7
Iškastas Blokas #40 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000433 | Hash: 000a7c3b0eb39c160ae9364843a52d3cc06f3da9d4907f1dd58124739ec66bd5
Iškastas Blokas #41 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 899 | Hash: 000f5e00ba3519a8b19b0d59ea0cdab16a6c5c3e52883f2c2b29957dd582cc44
Iškastas Blokas #42 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20003698 | Hash: 0008d5ec7228948f2fbd5b0278fe841260ad22c36b55ceaee3c9f9b346e8ac18
Iškastas Blokas #43 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000427 | Hash: 00050c98500ee1ad35a2a948f84689b9d17785a99bf43d17f6d324a2c6f3f00e
Iškastas Blokas #44 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10002038 | Hash: 000ccd4ba54c54375fa0f93a83877bfa87b34baa38f19db4fc796845b504013a
Iškastas Blokas #45 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10003131 | Hash: 000a232f91ae9bc8740a2330c291edd3932db69f76e54fd8bf87da8a1673cd41
Iškastas Blokas #46 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 3093 | Hash: 000c54a187de08449638a8fe5b314339579e6f8675abd44488b372c95f9a63a9
Iškastas Blokas #47 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000347 | Hash: 00005613ca276135c1ce081d673f1bf199af80aa91a470649cc372410015bf2c
Iškastas Blokas #48 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20004432 | Hash: 000d7c6db33aa6743764336d6b0fa31498ad324eeb72e12e0633841e84682ac5
Iškastas Blokas #49 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000752 | Hash: 000b9959fdae89854c7f27a3392af3d7a5581ba5123b96daa6d88e5a1764b861
Iškastas Blokas #50 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 80 | Hash: 0005b5a09993af4b7f41c08a2c3b2e1c7752749a37d0ca740bc229d6f1235df3
Iškastas Blokas #51 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 2562 | Hash: 0004cde583e9a1d475bbe3302de03b0596c9b6a2594149e0398051a6532d8880
Iškastas Blokas #52 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000475 | Hash: 0009802fb7aa2614cc61b417331bff75144264140ffe8780f479598ed968779e
Iškastas Blokas #53 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001702 | Hash: 0001f1e4e64d2fed6a9b0aaf0f0f41b4af027883ecb7ffd29f9ab16f0c27f1a4
Iškastas Blokas #54 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000108 | Hash: 0006e439dd4c9101a3f84a536b69e9c2a44e77cba9c95e21e0c48b47320d8b37
Iškastas Blokas #55 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001682 | Hash: 0009d67f403facd1ea6dcd79a6c0ea02ebbe3cac57e7d13867e85668370acd26
Iškastas Blokas #56 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000986 | Hash: 000e533069eee9b17a76b984e2240eb838754e5eea057a2c34c4297b5206990a
Iškastas Blokas #57 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20001026 | Hash: 0008e83be248accbe44df723ea41825b6d7481f4704fa841ba6d262cdb08e149
Iškastas Blokas #58 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001316 | Hash: 0003a5aef47589fa1d0dfb6c2d141dc4576aba1d2de12c237cf225e13b992b99
Iškastas Blokas #59 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 150 | Hash: 0003d625c2f4520b7b633ad70a5e57596729b73366ec4c60fd2ee7c4a0f795cb
Iškastas Blokas #60 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000132 | Hash: 000523e2398edeffeec7fd5df44ffb9105f20e9b53c88a9f4e2570355c474d3a
Iškastas Blokas #61 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000313 | Hash: 00043902ff510143c3eab62640dc45a3b0f67f2255bdddef5776d09f25279378
Iškastas Blokas #62 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000416 | Hash: 000e7857d9754fbe332ed0e48385f41589ada1e9c95941fe69da45300c1a7d65
Iškastas Blokas #63 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000091 | Hash: 0008a183cf92a8be21b6da2396c39c392688839de619a4f127b01e2e6b300725
Iškastas Blokas #64 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000178 | Hash: 000a44c36e47ea25df5cf4cc86de3b90659fe817f35b1fde81f298ca3097d4c3
Iškastas Blokas #65 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 160 | Hash: 0008111295ed50c0336a5fa3ab308ff7ab8489a85f13a8af7351487122f0975f
Iškastas Blokas #66 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001021 | Hash: 00091e26b728eb3b7ee81061d0e674bbe061afc576f595b099d30e3257ba19d6
Iškastas Blokas #67 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000604 | Hash: 00026766412762513dbc959ce6fb0b1dbd236a7fb0ef5673aace2613e5987301
Iškastas Blokas #68 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20002125 | Hash: 0008c549f22959f8048feda99de49cd752e7fc8840ebcfa34bf7db266181fb6e
Iškastas Blokas #69 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 893 | Hash: 00053d9a91260a9000f9bc472925d1cf2dae38685af86b53a23b1315a3b1a468
Iškastas Blokas #70 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000280 | Hash: 0003b2abdad17f3396385ed0e403c0dd87a5c3bf967713a4ac02a0a32bad3cf9
Iškastas Blokas #71 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001074 | Hash: 000742a3178076c897b3f78c6f59d788c7bae6920a5e1193afb497ea2619562b
Iškastas Blokas #72 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000190 | Hash: 0003c79a98f080b2a2c13a7c171caabd05c1cac6c97e281e8b96c6f30a9ec9c1
Iškastas Blokas #73 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000752 | Hash: 0006ed6bae15d04ad1f81064322540314eafb7e2a4fb3b5f12d56d123396e6c1
Iškastas Blokas #74 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000812 | Hash: 0009807731efb7ea623994468f49922144d855e908ee9e8219b966b14fc247fe
Iškastas Blokas #75 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20001616 | Hash: 00091361ac2bd2bee321806f887d745ca0764c9d9e10ee3860415dd046342e75
Iškastas Blokas #76 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000003 | Hash: 000ec929e2b7c2895bf5592b8954cc89132c6d0c923a03d809334272eedb85ea
Iškastas Blokas #77 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000387 | Hash: 000c34d719dd7350af18adade144cc5a1a37c3c2db865eec60e3920cbc7ade39
Iškastas Blokas #78 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10000489 | Hash: 000d8ed71bbb4b7e38b302a7e965fe72e23a2316ee9c2cb35e961d0747c76344
Iškastas Blokas #79 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1119 | Hash: 000ab5b809c01198bd85cca5c969c28d9a3f495037778b859082596ce60d7471
Iškastas Blokas #80 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1286 | Hash: 000ea135a430d45c5edf17d00d1c080342cf65cc8b917b773aecd0ef276a65a8
Iškastas Blokas #81 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000476 | Hash: 000ffb54cb209d6deb2402aa9011ae5cd764cc5f6314edf8f555d4aa3782761b
Iškastas Blokas #82 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000018 | Hash: 0005052d5cb0ac717df3ad7773ae7e24e5e4e7335e3d3a9f0af172c14d982b65
Iškastas Blokas #83 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000217 | Hash: 000fc2b1d161c98f2d04785d15d3fdbcdbe97fddba8586936a13606ac0c0f5ce
Iškastas Blokas #84 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000097 | Hash: 000943c984265ff95f9f0aa5ec746a11bad8c267637d5d99f7a4f8168db39b31
Iškastas Blokas #85 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000018 | Hash: 000f0c00e7f002101fadab4a8c829cfef71802fa2ae075a6cf9ad7562e628ed3
Iškastas Blokas #86 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 516 | Hash: 0002b5b74e25b513bb0e022ebf830ff97bd3d6e2155ba31ac73dd15f4278e921
Iškastas Blokas #87 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001956 | Hash: 00025faa604b0f370d21f635a17479e0ad1571d6d630f471854340a1f8559baa
Iškastas Blokas #88 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30003728 | Hash: 0007d247125668433ccf7e52469054b565963cb2e68172cc6c8d8cf4b4d877de
Iškastas Blokas #89 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001080 | Hash: 000febff29978e6a20593bfad17a26148223e5961f58d15825de9a1fbcba3f28
Iškastas Blokas #90 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000577 | Hash: 00052a7d1f6c249b660c22083fffddb1558b214957ece3e3c0f931f316dccac5
Iškastas Blokas #91 | Laimėjo Kand: 3 | Tx: 100 | Nonce: 20000355 | Hash: 000e615372ea9f853c791a01e677c8e62fe40cda5e80c8b95321bf8b6635f8df
Iškastas Blokas #92 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1337 | Hash: 000e6df6082155cff4aa5beb6cd178f29254dd5570a9c546bf866b6b68504c97
Iškastas Blokas #93 | Laimėjo Kand: 2 | Tx: 100 | Nonce: 10001115 | Hash: 0007740f5224fd3d9d166cf36cee52928deba362ca2f1bf871a920a204d2a95f
Iškastas Blokas #94 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000246 | Hash: 000fef68ed3ecaef3ff761d73716f4e57f5270a3fca4d0b972cbf9fbfd695ae5
Iškastas Blokas #95 | Laimėjo Kand: 1 | Tx: 100 | Nonce: 1803 | Hash: 000332cc9ef100610a2495ef1d84432bfc8163f91b57ac936ecd89a4bf488307
Iškastas Blokas #96 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30001328 | Hash: 00018a237e517fa4e00916ba6feebda936d9044c75855da843d0cad17fa5aa5d
Iškastas Blokas #97 | Laimėjo Kand: 5 | Tx: 100 | Nonce: 40000110 | Hash: 0008d9dd15e4f8847fa759c9d24fe9a3251caa5cb84cbca29272fc5eaf7a033d
Iškastas Blokas #98 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000225 | Hash: 000626d30f68c4c186fbc3af14d26b7bffc3b570456771f1645ba0fe3223bfdc
Iškastas Blokas #99 | Laimėjo Kand: 4 | Tx: 100 | Nonce: 30000412 | Hash: 0000b00327c5c5e4c151da1eb6a6d8d445048fafe082f832d414c41e1dc40bf8
Iškastas Blokas #100 | Laimėjo Kand: 4 | Tx: 86 | Nonce: 30000166 | Hash: 0008aa638cce857c953cd6982f92cf09877fbd397eb3dbb05443b1992c3bedac
Priimtu bloku (su Genesis): 101
Atmestos transakcijos: 14
Pradine suma: 500062293
Galutine suma: 500062293
Pinigai nedingo
Visas kasimo laikas: 3.33782 s

### Rezultatai

* **Vykdymo laikas:** Lygiagretus kasimas (naudojant `std::thread`) veikia pastebimai greičiau. Nuoseklus kasimas užtruko **4.94 s** (išrasta 102 blokai), o lygiagretus – **3.34 s** (išrasta 101 blokas), nes visi 5 kandidatai buvo kasami vienu metu skirtinguose procesoriaus srautuose.
* **Paieškos efektyvumas (Nonce):** Netaikant gijų, kandidatai buvo tikrinami iš eilės, todėl pirmiems kandidatams tekdavo mažiausi `nonce` skaitikliai, o kitiems dažnai nebetrūkdavo laiko. Naudojant lygiagretų kasimą, kiekvienas kandidatas gavo atskirą `nonce` rėžį (`id * 10000000`), todėl jie varžėsi sąžiningai ir laimėdavo tas, kurio transakcijų rinkinys greičiausiai rasdavo tinkamą maišą.
* **Saugumas ir teisingumas:** Abiem atvejais pradinė ir galutinė lėšų suma išliko identiška (`Pinigai nedingo`), kas patvirtina, kad lygiagretus gijų darbas veikia saugiai, nesugadindami bendrų balansų duomenų.

### Paleidimas

g++ -std=c++17 -pthread main.cpp -o programa

./programa