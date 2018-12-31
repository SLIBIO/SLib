/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_CORE_LOCALE
#define CHECKHEADER_SLIB_CORE_LOCALE

#include "definition.h"

#include "macro.h"
#include "string.h"
#include "parse.h"
#include "function.h"

namespace slib
{

/*
	Language Code by ISO 639-1
*/

#define SLIB_LANGUAGE(CODE) (slib::Language)(SLIB_MAKE_WORD((#CODE)[1], (#CODE)[0]))

#define SLIB_DEFINE_LANGUAGE_CODE(CODE, NAME) \
	CODE = (int)(SLIB_LANGUAGE(CODE)), \
	NAME = CODE

	enum class Language
	{
		Unknown = 0,
		SLIB_DEFINE_LANGUAGE_CODE(ab, Abkhaz),
		SLIB_DEFINE_LANGUAGE_CODE(aa, Afar),
		SLIB_DEFINE_LANGUAGE_CODE(af, Afrikaans),
		SLIB_DEFINE_LANGUAGE_CODE(ak, Akan),
		SLIB_DEFINE_LANGUAGE_CODE(sq, Albanian),
		SLIB_DEFINE_LANGUAGE_CODE(am, Amharic),
		SLIB_DEFINE_LANGUAGE_CODE(ar, Arabic),
		SLIB_DEFINE_LANGUAGE_CODE(an, Aragonese),
		SLIB_DEFINE_LANGUAGE_CODE(hy, Armenian),
		SLIB_DEFINE_LANGUAGE_CODE(as, Assamese),
		SLIB_DEFINE_LANGUAGE_CODE(av, Avaric),
		SLIB_DEFINE_LANGUAGE_CODE(ae, Avestan),
		SLIB_DEFINE_LANGUAGE_CODE(ay, Aymara),
		SLIB_DEFINE_LANGUAGE_CODE(az, Azerbaijani),
		SLIB_DEFINE_LANGUAGE_CODE(bm, Bambara),
		SLIB_DEFINE_LANGUAGE_CODE(ba, Bashkir),
		SLIB_DEFINE_LANGUAGE_CODE(eu, Basque),
		SLIB_DEFINE_LANGUAGE_CODE(be, Belarusian),
		SLIB_DEFINE_LANGUAGE_CODE(bn, Bengali),
		Bangla = Bengali,
		SLIB_DEFINE_LANGUAGE_CODE(bh, Bihari),
		SLIB_DEFINE_LANGUAGE_CODE(bi, Bislama),
		SLIB_DEFINE_LANGUAGE_CODE(bs, Bosnian),
		SLIB_DEFINE_LANGUAGE_CODE(br, Breton),
		SLIB_DEFINE_LANGUAGE_CODE(bg, Bulgarian),
		SLIB_DEFINE_LANGUAGE_CODE(my, Burmese),
		SLIB_DEFINE_LANGUAGE_CODE(ca, Catalan),
		SLIB_DEFINE_LANGUAGE_CODE(ch, Chamorro),
		SLIB_DEFINE_LANGUAGE_CODE(ce, Chechen),
		SLIB_DEFINE_LANGUAGE_CODE(ny, Chichewa),
		Chewa = Chichewa,
		Nyanja = Chichewa,
		SLIB_DEFINE_LANGUAGE_CODE(zh, Chinese),
		SLIB_DEFINE_LANGUAGE_CODE(cv, Chuvash),
		SLIB_DEFINE_LANGUAGE_CODE(kw, Cornish),
		SLIB_DEFINE_LANGUAGE_CODE(co, Corsican),
		SLIB_DEFINE_LANGUAGE_CODE(cr, Cree),
		SLIB_DEFINE_LANGUAGE_CODE(hr, Croatian),
		SLIB_DEFINE_LANGUAGE_CODE(cs, Czech),
		SLIB_DEFINE_LANGUAGE_CODE(da, Danish),
		SLIB_DEFINE_LANGUAGE_CODE(dv, Divehi),
		Dhivehi = Divehi,
		Maldivian = Divehi,
		SLIB_DEFINE_LANGUAGE_CODE(nl, Dutch),
		SLIB_DEFINE_LANGUAGE_CODE(dz, Dzongkha),
		SLIB_DEFINE_LANGUAGE_CODE(en, English),
		SLIB_DEFINE_LANGUAGE_CODE(eo, Esperanto),
		SLIB_DEFINE_LANGUAGE_CODE(et, Estonian),
		SLIB_DEFINE_LANGUAGE_CODE(ee, Ewe),
		SLIB_DEFINE_LANGUAGE_CODE(fo, Faroese),
		SLIB_DEFINE_LANGUAGE_CODE(fj, Fijian),
		SLIB_DEFINE_LANGUAGE_CODE(fi, Finnish),
		SLIB_DEFINE_LANGUAGE_CODE(fr, French),
		SLIB_DEFINE_LANGUAGE_CODE(ff, Fula),
		Fulah = Fula,
		Pulaar = Fula,
		Pular = Fula,
		SLIB_DEFINE_LANGUAGE_CODE(gl, Galician),
		SLIB_DEFINE_LANGUAGE_CODE(ka, Georgian),
		SLIB_DEFINE_LANGUAGE_CODE(de, German),
		SLIB_DEFINE_LANGUAGE_CODE(el, Greek),
		SLIB_DEFINE_LANGUAGE_CODE(gn, Guarani),
		SLIB_DEFINE_LANGUAGE_CODE(gu, Gujarati),
		SLIB_DEFINE_LANGUAGE_CODE(ht, Haitian),
		SLIB_DEFINE_LANGUAGE_CODE(ha, Hausa),
		SLIB_DEFINE_LANGUAGE_CODE(he, Hebrew),
		SLIB_DEFINE_LANGUAGE_CODE(hz, Herero),
		SLIB_DEFINE_LANGUAGE_CODE(hi, Hindi),
		SLIB_DEFINE_LANGUAGE_CODE(ho, HiriMotu),
		SLIB_DEFINE_LANGUAGE_CODE(hu, Hungarian),
		SLIB_DEFINE_LANGUAGE_CODE(ia, Interlingua),
		SLIB_DEFINE_LANGUAGE_CODE(id_, Indonesian),
		SLIB_DEFINE_LANGUAGE_CODE(ie, Interlingue),
		SLIB_DEFINE_LANGUAGE_CODE(ga, Irish),
		SLIB_DEFINE_LANGUAGE_CODE(ig, Igbo),
		SLIB_DEFINE_LANGUAGE_CODE(ik, Inupiaq),
		SLIB_DEFINE_LANGUAGE_CODE(io, Ido),
		SLIB_DEFINE_LANGUAGE_CODE(is, Icelandic),
		SLIB_DEFINE_LANGUAGE_CODE(it, Italian),
		SLIB_DEFINE_LANGUAGE_CODE(iu, Inuktitut),
		SLIB_DEFINE_LANGUAGE_CODE(ja, Japanese),
		SLIB_DEFINE_LANGUAGE_CODE(jv, Javanese),
		SLIB_DEFINE_LANGUAGE_CODE(kl, Kalaallisut),
		Greenlandic = Kalaallisut,
		SLIB_DEFINE_LANGUAGE_CODE(kn, Kannada),
		SLIB_DEFINE_LANGUAGE_CODE(kr, Kanuri),
		SLIB_DEFINE_LANGUAGE_CODE(ks, Kashmiri),
		SLIB_DEFINE_LANGUAGE_CODE(kk, Kazakh),
		SLIB_DEFINE_LANGUAGE_CODE(km, Khmer),
		SLIB_DEFINE_LANGUAGE_CODE(ki, Kikuyu),
		Gikuyu = Kikuyu,
		SLIB_DEFINE_LANGUAGE_CODE(rw, Kinyarwanda),
		SLIB_DEFINE_LANGUAGE_CODE(ky, Kyrgyz),
		SLIB_DEFINE_LANGUAGE_CODE(kv, Komi),
		SLIB_DEFINE_LANGUAGE_CODE(kg, Kongo),
		SLIB_DEFINE_LANGUAGE_CODE(ko, Korean),
		SLIB_DEFINE_LANGUAGE_CODE(ku, Kurdish),
		SLIB_DEFINE_LANGUAGE_CODE(kj, Kwanyama),
		Kuanyama = Kwanyama,
		SLIB_DEFINE_LANGUAGE_CODE(la, Latin),
		SLIB_DEFINE_LANGUAGE_CODE(lb, Luxembourgish),
		Letzeburgesch = Luxembourgish,
		SLIB_DEFINE_LANGUAGE_CODE(lg, Ganda),
		SLIB_DEFINE_LANGUAGE_CODE(li, Limburgish),
		Limburgan = Limburgish,
		Limburger = Limburgish,
		SLIB_DEFINE_LANGUAGE_CODE(ln, Lingala),
		SLIB_DEFINE_LANGUAGE_CODE(lo, Lao),
		SLIB_DEFINE_LANGUAGE_CODE(lt, Lithuanian),
		SLIB_DEFINE_LANGUAGE_CODE(lu, LubaKatanga),
		SLIB_DEFINE_LANGUAGE_CODE(lv, Latvian),
		SLIB_DEFINE_LANGUAGE_CODE(gv, Manx),
		SLIB_DEFINE_LANGUAGE_CODE(mk, Macedonian),
		SLIB_DEFINE_LANGUAGE_CODE(mg, Malagasy),
		SLIB_DEFINE_LANGUAGE_CODE(ms, Malay),
		SLIB_DEFINE_LANGUAGE_CODE(ml, Malayalam),
		SLIB_DEFINE_LANGUAGE_CODE(mt, Maltese),
		SLIB_DEFINE_LANGUAGE_CODE(mi, Maori),
		SLIB_DEFINE_LANGUAGE_CODE(mr, Marathi),
		SLIB_DEFINE_LANGUAGE_CODE(mh, Marshallese),
		SLIB_DEFINE_LANGUAGE_CODE(mn, Mongolian),
		SLIB_DEFINE_LANGUAGE_CODE(na, Nauruan),
		SLIB_DEFINE_LANGUAGE_CODE(nv, Navajo),
		Navaho = Navajo,
		SLIB_DEFINE_LANGUAGE_CODE(nd, NorthernNdebele),
		SLIB_DEFINE_LANGUAGE_CODE(ne, Nepali),
		SLIB_DEFINE_LANGUAGE_CODE(ng, Ndonga),
		SLIB_DEFINE_LANGUAGE_CODE(nb, NorwegianBokmal),
		SLIB_DEFINE_LANGUAGE_CODE(nn, NorwegianNynorsk),
		SLIB_DEFINE_LANGUAGE_CODE(no, Norwegian),
		SLIB_DEFINE_LANGUAGE_CODE(ii, Nuosu),
		SLIB_DEFINE_LANGUAGE_CODE(nr, SouthernNdebele),
		SLIB_DEFINE_LANGUAGE_CODE(oc, Occitan),
		SLIB_DEFINE_LANGUAGE_CODE(oj, Ojibwe),
		Ojibwa = Ojibwe,
		SLIB_DEFINE_LANGUAGE_CODE(cu, OldChurchSlavonic),
		ChurchSlavonic = OldChurchSlavonic,
		OldBulgarian = OldChurchSlavonic,
		SLIB_DEFINE_LANGUAGE_CODE(om, Oromo),
		SLIB_DEFINE_LANGUAGE_CODE(or_, Oriya),
		SLIB_DEFINE_LANGUAGE_CODE(os, Ossetian),
		Ossetic = Ossetian,
		SLIB_DEFINE_LANGUAGE_CODE(pa, Panjabi),
		Punjabi = Panjabi,
		SLIB_DEFINE_LANGUAGE_CODE(pi, Pali),
		SLIB_DEFINE_LANGUAGE_CODE(fa, Persian),
		SLIB_DEFINE_LANGUAGE_CODE(pl, Polish),
		SLIB_DEFINE_LANGUAGE_CODE(ps, Pashto),
		Pushto = Pashto,
		SLIB_DEFINE_LANGUAGE_CODE(pt, Portuguese),
		SLIB_DEFINE_LANGUAGE_CODE(qu, Quechua),
		SLIB_DEFINE_LANGUAGE_CODE(rm, Romansh),
		SLIB_DEFINE_LANGUAGE_CODE(rn, Kirundi),
		SLIB_DEFINE_LANGUAGE_CODE(ro, Romanian),
		SLIB_DEFINE_LANGUAGE_CODE(ru, Russian),
		SLIB_DEFINE_LANGUAGE_CODE(sa, Sanskrit),
		SLIB_DEFINE_LANGUAGE_CODE(sc, Sardinian),
		SLIB_DEFINE_LANGUAGE_CODE(sd, Sindhi),
		SLIB_DEFINE_LANGUAGE_CODE(se, NorthernSami),
		SLIB_DEFINE_LANGUAGE_CODE(sm, Samoan),
		SLIB_DEFINE_LANGUAGE_CODE(sg, Sango),
		SLIB_DEFINE_LANGUAGE_CODE(sr, Serbian),
		SLIB_DEFINE_LANGUAGE_CODE(gd, Gaelic),
		ScottishGaelic = Gaelic,
		SLIB_DEFINE_LANGUAGE_CODE(sn, Shona),
		SLIB_DEFINE_LANGUAGE_CODE(si, Sinhala),
		Sinhalese = Sinhala,
		SLIB_DEFINE_LANGUAGE_CODE(sk, Slovak),
		SLIB_DEFINE_LANGUAGE_CODE(sl, Slovene),
		SLIB_DEFINE_LANGUAGE_CODE(so, Somali),
		SLIB_DEFINE_LANGUAGE_CODE(st, SouthernSotho),
		SLIB_DEFINE_LANGUAGE_CODE(es, Spanish),
		SLIB_DEFINE_LANGUAGE_CODE(su, Sundanese),
		SLIB_DEFINE_LANGUAGE_CODE(sw, Swahili),
		SLIB_DEFINE_LANGUAGE_CODE(ss, Swati),
		SLIB_DEFINE_LANGUAGE_CODE(sv, Swedish),
		SLIB_DEFINE_LANGUAGE_CODE(ta, Tamil),
		SLIB_DEFINE_LANGUAGE_CODE(te, Telugu),
		SLIB_DEFINE_LANGUAGE_CODE(tg, Tajik),
		SLIB_DEFINE_LANGUAGE_CODE(th, Thai),
		SLIB_DEFINE_LANGUAGE_CODE(ti, Tigrinya),
		SLIB_DEFINE_LANGUAGE_CODE(bo, Tibetan),
		SLIB_DEFINE_LANGUAGE_CODE(tk, Turkmen),
		SLIB_DEFINE_LANGUAGE_CODE(tl, Tagalog),
		SLIB_DEFINE_LANGUAGE_CODE(tn, Tswana),
		SLIB_DEFINE_LANGUAGE_CODE(to, Tonga),
		SLIB_DEFINE_LANGUAGE_CODE(tr, Turkish),
		SLIB_DEFINE_LANGUAGE_CODE(ts, Tsonga),
		SLIB_DEFINE_LANGUAGE_CODE(tt, Tatar),
		SLIB_DEFINE_LANGUAGE_CODE(tw, Twi),
		SLIB_DEFINE_LANGUAGE_CODE(ty, Tahitian),
		SLIB_DEFINE_LANGUAGE_CODE(ug, Uyghur),
		SLIB_DEFINE_LANGUAGE_CODE(uk, Ukrainian),
		SLIB_DEFINE_LANGUAGE_CODE(ur, Urdu),
		SLIB_DEFINE_LANGUAGE_CODE(uz, Uzbek),
		SLIB_DEFINE_LANGUAGE_CODE(ve, Venda),
		SLIB_DEFINE_LANGUAGE_CODE(vi, Vietnamese),
		SLIB_DEFINE_LANGUAGE_CODE(vo, Volapuk),
		SLIB_DEFINE_LANGUAGE_CODE(wa, Walloon),
		SLIB_DEFINE_LANGUAGE_CODE(cy, Welsh),
		SLIB_DEFINE_LANGUAGE_CODE(wo, Wolof),
		SLIB_DEFINE_LANGUAGE_CODE(fy, WesternFrisian),
		SLIB_DEFINE_LANGUAGE_CODE(xh, Xhosa),
		SLIB_DEFINE_LANGUAGE_CODE(yi, Yiddish),
		SLIB_DEFINE_LANGUAGE_CODE(yo, Yoruba),
		SLIB_DEFINE_LANGUAGE_CODE(za, Zhuang),
		Chuang = Zhuang,
		SLIB_DEFINE_LANGUAGE_CODE(zu, Zulu)

	};


/*
	Country Code by ISO 3166-1 alpha-2
*/

#define SLIB_COUNTRY(CODE) (slib::Country)(SLIB_MAKE_WORD((#CODE)[1], (#CODE)[0]))

#define SLIB_DEFINE_COUNTRY_CODE(CODE, NAME) \
	CODE = (int)(SLIB_COUNTRY(CODE)), \
	NAME = CODE

#ifdef IN
#undef IN
#endif
	
	enum class Country
	{
		Unknown = 0,
		SLIB_DEFINE_COUNTRY_CODE(AD, Andorra),
		SLIB_DEFINE_COUNTRY_CODE(AE, UnitedArabEmirates),
		UAE = UnitedArabEmirates,
		SLIB_DEFINE_COUNTRY_CODE(AF, Afghanistan),
		SLIB_DEFINE_COUNTRY_CODE(AG, AntiguaAndBarbuda),
		SLIB_DEFINE_COUNTRY_CODE(AI, Anguilla),
		SLIB_DEFINE_COUNTRY_CODE(AL, Albania),
		SLIB_DEFINE_COUNTRY_CODE(AM, Armenia),
		SLIB_DEFINE_COUNTRY_CODE(AO, Angola),
		SLIB_DEFINE_COUNTRY_CODE(AQ, Antarctica),
		SLIB_DEFINE_COUNTRY_CODE(AR, Argentina),
		SLIB_DEFINE_COUNTRY_CODE(AS, AmericanSamoa),
		SLIB_DEFINE_COUNTRY_CODE(AT, Austria),
		SLIB_DEFINE_COUNTRY_CODE(AU, Australia),
		SLIB_DEFINE_COUNTRY_CODE(AW, Aruba),
		SLIB_DEFINE_COUNTRY_CODE(AX, AlandIslands),
		SLIB_DEFINE_COUNTRY_CODE(AZ, Azerbaijan),
		SLIB_DEFINE_COUNTRY_CODE(BA, BosniaAndHerzegovina),
		SLIB_DEFINE_COUNTRY_CODE(BB, Barbados),
		SLIB_DEFINE_COUNTRY_CODE(BD, Bangladesh),
		SLIB_DEFINE_COUNTRY_CODE(BE, Belgium),
		SLIB_DEFINE_COUNTRY_CODE(BF, BurkinaFaso),
		SLIB_DEFINE_COUNTRY_CODE(BG, Bulgaria),
		SLIB_DEFINE_COUNTRY_CODE(BH, Bahrain),
		SLIB_DEFINE_COUNTRY_CODE(BI, Burundi),
		SLIB_DEFINE_COUNTRY_CODE(BJ, Benin),
		SLIB_DEFINE_COUNTRY_CODE(BL, SaintBarthelemy),
		SLIB_DEFINE_COUNTRY_CODE(BM, Bermuda),
		SLIB_DEFINE_COUNTRY_CODE(BN, BruneiDarussalam),
		SLIB_DEFINE_COUNTRY_CODE(BO, Bolivia),
		SLIB_DEFINE_COUNTRY_CODE(BQ, CaribbeanNetherlands), // Bonaire, Sint Eustatius and Saba
		SLIB_DEFINE_COUNTRY_CODE(BR, Brazil),
		SLIB_DEFINE_COUNTRY_CODE(BS, Bahamas),
		SLIB_DEFINE_COUNTRY_CODE(BT, Bhutan),
		SLIB_DEFINE_COUNTRY_CODE(BV, BouvetIsland),
		SLIB_DEFINE_COUNTRY_CODE(BW, Botswana),
		SLIB_DEFINE_COUNTRY_CODE(BY, Belarus),
		SLIB_DEFINE_COUNTRY_CODE(BZ, Belize),
		SLIB_DEFINE_COUNTRY_CODE(CA, Canada),
		SLIB_DEFINE_COUNTRY_CODE(CC, CocosIslands), // Cocos (Keeling) Islands
		SLIB_DEFINE_COUNTRY_CODE(CD, DRCongo), // the Democratic Republic of the Congo
		SLIB_DEFINE_COUNTRY_CODE(CF, CentralAfricanRepublic),
		SLIB_DEFINE_COUNTRY_CODE(CG, Congo),
		SLIB_DEFINE_COUNTRY_CODE(CH, Switzerland),
		SLIB_DEFINE_COUNTRY_CODE(CI, IvoryCoast), // Cote d'Ivoire
		SLIB_DEFINE_COUNTRY_CODE(CK, CookIslands),
		SLIB_DEFINE_COUNTRY_CODE(CL, Chile),
		SLIB_DEFINE_COUNTRY_CODE(CM, Cameroon),
		SLIB_DEFINE_COUNTRY_CODE(CN, China),
		SLIB_DEFINE_COUNTRY_CODE(CO, Colombia),
		SLIB_DEFINE_COUNTRY_CODE(CR, CostaRica),
		SLIB_DEFINE_COUNTRY_CODE(CU, Cuba),
		SLIB_DEFINE_COUNTRY_CODE(CV, CaboVerde),
		SLIB_DEFINE_COUNTRY_CODE(CW, Curacao),
		SLIB_DEFINE_COUNTRY_CODE(CX, ChristmasIsland),
		SLIB_DEFINE_COUNTRY_CODE(CY, Cyprus),
		SLIB_DEFINE_COUNTRY_CODE(CZ, Czech),
		SLIB_DEFINE_COUNTRY_CODE(DE, Germany),
		SLIB_DEFINE_COUNTRY_CODE(DJ, Djibouti),
		SLIB_DEFINE_COUNTRY_CODE(DK, Denmark),
		SLIB_DEFINE_COUNTRY_CODE(DM, Dominica),
		SLIB_DEFINE_COUNTRY_CODE(DO, DominicanRepublic),
		SLIB_DEFINE_COUNTRY_CODE(DZ, Algeria),
		SLIB_DEFINE_COUNTRY_CODE(EC, Ecuador),
		SLIB_DEFINE_COUNTRY_CODE(EE, Estonia),
		SLIB_DEFINE_COUNTRY_CODE(EG, Egypt),
		SLIB_DEFINE_COUNTRY_CODE(EH, WesternSahara),
		SLIB_DEFINE_COUNTRY_CODE(ER, Eritrea),
		SLIB_DEFINE_COUNTRY_CODE(ES, Spain),
		SLIB_DEFINE_COUNTRY_CODE(ET, Ethiopia),
		SLIB_DEFINE_COUNTRY_CODE(FI, Finland),
		SLIB_DEFINE_COUNTRY_CODE(FJ, Fiji),
		SLIB_DEFINE_COUNTRY_CODE(FK, FalklandIslands),
		SLIB_DEFINE_COUNTRY_CODE(FM, Micronesia),
		SLIB_DEFINE_COUNTRY_CODE(FO, FaroeIslands),
		SLIB_DEFINE_COUNTRY_CODE(FR, France),
		SLIB_DEFINE_COUNTRY_CODE(GA, Gabon),
		SLIB_DEFINE_COUNTRY_CODE(GB, UnitedKingdom),
		UK = UnitedKingdom,
		SLIB_DEFINE_COUNTRY_CODE(GD, Grenada),
		SLIB_DEFINE_COUNTRY_CODE(GE, Georgia),
		SLIB_DEFINE_COUNTRY_CODE(GF, FrenchGuiana),
		SLIB_DEFINE_COUNTRY_CODE(GG, Guernsey),
		SLIB_DEFINE_COUNTRY_CODE(GH, Ghana),
		SLIB_DEFINE_COUNTRY_CODE(GI, Gibraltar),
		SLIB_DEFINE_COUNTRY_CODE(GL, Greenland),
		SLIB_DEFINE_COUNTRY_CODE(GM, Gambia),
		SLIB_DEFINE_COUNTRY_CODE(GN, Guinea),
		SLIB_DEFINE_COUNTRY_CODE(GP, Guadeloupe),
		SLIB_DEFINE_COUNTRY_CODE(GQ, EquatorialGuinea),
		SLIB_DEFINE_COUNTRY_CODE(GR, Greece),
		SLIB_DEFINE_COUNTRY_CODE(GS, SGSSI), // South Georgia and the South Sandwich Islands
		SLIB_DEFINE_COUNTRY_CODE(GT, Guatemala),
		SLIB_DEFINE_COUNTRY_CODE(GU, Guam),
		SLIB_DEFINE_COUNTRY_CODE(GW, GuineaBissau),
		SLIB_DEFINE_COUNTRY_CODE(GY, Guyana),
		SLIB_DEFINE_COUNTRY_CODE(HK, HongKong),
		SLIB_DEFINE_COUNTRY_CODE(HM, HIMI), // Heard Island and McDonald Islands
		SLIB_DEFINE_COUNTRY_CODE(HN, Honduras),
		SLIB_DEFINE_COUNTRY_CODE(HR, Croatia),
		SLIB_DEFINE_COUNTRY_CODE(HT, Haiti),
		SLIB_DEFINE_COUNTRY_CODE(HU, Hungary),
		SLIB_DEFINE_COUNTRY_CODE(ID, Indonesia),
		SLIB_DEFINE_COUNTRY_CODE(IE, Ireland),
		SLIB_DEFINE_COUNTRY_CODE(IL, Israel),
		SLIB_DEFINE_COUNTRY_CODE(IM, IsleOfMan),
		SLIB_DEFINE_COUNTRY_CODE(IN, India),
		SLIB_DEFINE_COUNTRY_CODE(IO, BritishIndianOceanTerritory),
		SLIB_DEFINE_COUNTRY_CODE(IQ, Iraq),
		SLIB_DEFINE_COUNTRY_CODE(IR, Iran),
		SLIB_DEFINE_COUNTRY_CODE(IS, Iceland),
		SLIB_DEFINE_COUNTRY_CODE(IT, Italy),
		SLIB_DEFINE_COUNTRY_CODE(JE, Jersey),
		SLIB_DEFINE_COUNTRY_CODE(JM, Jamaica),
		SLIB_DEFINE_COUNTRY_CODE(JO, Jordan),
		SLIB_DEFINE_COUNTRY_CODE(JP, Japan),
		SLIB_DEFINE_COUNTRY_CODE(KE, Kenya),
		SLIB_DEFINE_COUNTRY_CODE(KG, Kyrgyzstan),
		SLIB_DEFINE_COUNTRY_CODE(KH, Cambodia),
		SLIB_DEFINE_COUNTRY_CODE(KI, Kiribati),
		SLIB_DEFINE_COUNTRY_CODE(KM, Comoros),
		SLIB_DEFINE_COUNTRY_CODE(KN, SaintKittsAndNevis),
		SLIB_DEFINE_COUNTRY_CODE(KP, NorthKorea),
		DPRK = NorthKorea,
		SLIB_DEFINE_COUNTRY_CODE(KR, SouthKorea),
		SLIB_DEFINE_COUNTRY_CODE(KW, Kuwait),
		SLIB_DEFINE_COUNTRY_CODE(KY, CaymanIslands),
		SLIB_DEFINE_COUNTRY_CODE(KZ, Kazakhstan),
		SLIB_DEFINE_COUNTRY_CODE(LA, Laos),
		SLIB_DEFINE_COUNTRY_CODE(LB, Lebanon),
		SLIB_DEFINE_COUNTRY_CODE(LC, SaintLucia),
		SLIB_DEFINE_COUNTRY_CODE(LI, Liechtenstein),
		SLIB_DEFINE_COUNTRY_CODE(LK, SriLanka),
		SLIB_DEFINE_COUNTRY_CODE(LR, Liberia),
		SLIB_DEFINE_COUNTRY_CODE(LS, Lesotho),
		SLIB_DEFINE_COUNTRY_CODE(LT, Lithuania),
		SLIB_DEFINE_COUNTRY_CODE(LU, Luxembourg),
		SLIB_DEFINE_COUNTRY_CODE(LV, Latvia),
		SLIB_DEFINE_COUNTRY_CODE(LY, Libya),
		SLIB_DEFINE_COUNTRY_CODE(MA, Morocco),
		SLIB_DEFINE_COUNTRY_CODE(MC, Monaco),
		SLIB_DEFINE_COUNTRY_CODE(MD, Moldova),
		SLIB_DEFINE_COUNTRY_CODE(ME, Montenegro),
		SLIB_DEFINE_COUNTRY_CODE(MF, SaintMartin),
		SLIB_DEFINE_COUNTRY_CODE(MG, Madagascar),
		SLIB_DEFINE_COUNTRY_CODE(MH, MarshallIslands),
		SLIB_DEFINE_COUNTRY_CODE(MK, Macedonia),
		SLIB_DEFINE_COUNTRY_CODE(ML, Mali),
		SLIB_DEFINE_COUNTRY_CODE(MM, Myanmar),
		SLIB_DEFINE_COUNTRY_CODE(MN, Mongolia),
		SLIB_DEFINE_COUNTRY_CODE(MO, Macao),
		SLIB_DEFINE_COUNTRY_CODE(MP, NorthernMarianaIslands),
		SLIB_DEFINE_COUNTRY_CODE(MQ, Martinique),
		SLIB_DEFINE_COUNTRY_CODE(MR, Mauritania),
		SLIB_DEFINE_COUNTRY_CODE(MS, Montserrat),
		SLIB_DEFINE_COUNTRY_CODE(MT, Malta),
		SLIB_DEFINE_COUNTRY_CODE(MU, Mauritius),
		SLIB_DEFINE_COUNTRY_CODE(MV, Maldives),
		SLIB_DEFINE_COUNTRY_CODE(MW, Malawi),
		SLIB_DEFINE_COUNTRY_CODE(MX, Mexico),
		SLIB_DEFINE_COUNTRY_CODE(MY, Malaysia),
		SLIB_DEFINE_COUNTRY_CODE(MZ, Mozambique),
		SLIB_DEFINE_COUNTRY_CODE(NA, Namibia),
		SLIB_DEFINE_COUNTRY_CODE(NC, NewCaledonia),
		SLIB_DEFINE_COUNTRY_CODE(NE, Niger),
		SLIB_DEFINE_COUNTRY_CODE(NF, NorfolkIsland),
		SLIB_DEFINE_COUNTRY_CODE(NG, Nigeria),
		SLIB_DEFINE_COUNTRY_CODE(NI, Nicaragua),
		SLIB_DEFINE_COUNTRY_CODE(NL, Netherlands),
		SLIB_DEFINE_COUNTRY_CODE(NO_, Norway),
		SLIB_DEFINE_COUNTRY_CODE(NP, Nepal),
		SLIB_DEFINE_COUNTRY_CODE(NR, Nauru),
		SLIB_DEFINE_COUNTRY_CODE(NU, Niue),
		SLIB_DEFINE_COUNTRY_CODE(NZ, NewZealand),
		SLIB_DEFINE_COUNTRY_CODE(OM, Oman),
		SLIB_DEFINE_COUNTRY_CODE(PA, Panama),
		SLIB_DEFINE_COUNTRY_CODE(PE, Peru),
		SLIB_DEFINE_COUNTRY_CODE(PF, FrenchPolynesia),
		SLIB_DEFINE_COUNTRY_CODE(PG, PapuaNewGuinea),
		SLIB_DEFINE_COUNTRY_CODE(PH, Philippines),
		SLIB_DEFINE_COUNTRY_CODE(PK, Pakistan),
		SLIB_DEFINE_COUNTRY_CODE(PL, Poland),
		SLIB_DEFINE_COUNTRY_CODE(PM, SaintPierreAndMiquelon),
		SLIB_DEFINE_COUNTRY_CODE(PN, Pitcairn),
		SLIB_DEFINE_COUNTRY_CODE(PR, PuertoRico),
		SLIB_DEFINE_COUNTRY_CODE(PS, Palestine),
		SLIB_DEFINE_COUNTRY_CODE(PT, Portugal),
		SLIB_DEFINE_COUNTRY_CODE(PW, Palau),
		SLIB_DEFINE_COUNTRY_CODE(PY, Paraguay),
		SLIB_DEFINE_COUNTRY_CODE(QA, Qatar),
		SLIB_DEFINE_COUNTRY_CODE(RE, Reunion),
		SLIB_DEFINE_COUNTRY_CODE(RO, Romania),
		SLIB_DEFINE_COUNTRY_CODE(RS, Serbia),
		SLIB_DEFINE_COUNTRY_CODE(RU, Russia),
		SLIB_DEFINE_COUNTRY_CODE(RW, Rwanda),
		SLIB_DEFINE_COUNTRY_CODE(SA, SaudiArabia),
		SLIB_DEFINE_COUNTRY_CODE(SB, SolomonIslands),
		SLIB_DEFINE_COUNTRY_CODE(SC, Seychelles),
		SLIB_DEFINE_COUNTRY_CODE(SD, Sudan),
		SLIB_DEFINE_COUNTRY_CODE(SE, Sweden),
		SLIB_DEFINE_COUNTRY_CODE(SG, Singapore),
		SLIB_DEFINE_COUNTRY_CODE(SH, SaintHelena), // Saint Helena, Ascension and Tristan da Cunha
		SLIB_DEFINE_COUNTRY_CODE(SI, Slovenia),
		SLIB_DEFINE_COUNTRY_CODE(SJ, SvalbardAndJanMayen),
		SLIB_DEFINE_COUNTRY_CODE(SK, Slovakia),
		SLIB_DEFINE_COUNTRY_CODE(SL, SierraLeone),
		SLIB_DEFINE_COUNTRY_CODE(SM, SanMarino),
		SLIB_DEFINE_COUNTRY_CODE(SN, Senegal),
		SLIB_DEFINE_COUNTRY_CODE(SO, Somalia),
		SLIB_DEFINE_COUNTRY_CODE(SR, Suriname),
		SLIB_DEFINE_COUNTRY_CODE(SS, SouthSudan),
		SLIB_DEFINE_COUNTRY_CODE(ST, SaoTomeAndPrincipe),
		SLIB_DEFINE_COUNTRY_CODE(SV, ElSalvador),
		SLIB_DEFINE_COUNTRY_CODE(SX, SintMaarten),
		SLIB_DEFINE_COUNTRY_CODE(SY, Syria),
		SLIB_DEFINE_COUNTRY_CODE(SZ, Swaziland),
		SLIB_DEFINE_COUNTRY_CODE(TC, TurksAndCaicosIslands),
		SLIB_DEFINE_COUNTRY_CODE(TD, Chad),
		SLIB_DEFINE_COUNTRY_CODE(TF, FrenchSouthernTerritories),
		SLIB_DEFINE_COUNTRY_CODE(TG, Togo),
		SLIB_DEFINE_COUNTRY_CODE(TH, Thailand),
		SLIB_DEFINE_COUNTRY_CODE(TJ, Tajikistan),
		SLIB_DEFINE_COUNTRY_CODE(TK, Tokelau),
		SLIB_DEFINE_COUNTRY_CODE(TL, TimorLeste),
		SLIB_DEFINE_COUNTRY_CODE(TM, Turkmenistan),
		SLIB_DEFINE_COUNTRY_CODE(TN, Tunisia),
		SLIB_DEFINE_COUNTRY_CODE(TO, Tonga),
		SLIB_DEFINE_COUNTRY_CODE(TR, Turkey),
		SLIB_DEFINE_COUNTRY_CODE(TT, TrinidadAndTobago),
		SLIB_DEFINE_COUNTRY_CODE(TV, Tuvalu),
		SLIB_DEFINE_COUNTRY_CODE(TW, Taiwan),
		SLIB_DEFINE_COUNTRY_CODE(TZ, Tanzania),
		SLIB_DEFINE_COUNTRY_CODE(UA, Ukraine),
		SLIB_DEFINE_COUNTRY_CODE(UG, Uganda),
		SLIB_DEFINE_COUNTRY_CODE(UM, UnitedStatesMinorOutlyingIslands),
		SLIB_DEFINE_COUNTRY_CODE(US, UnitedStates),
		SLIB_DEFINE_COUNTRY_CODE(UY, Uruguay),
		SLIB_DEFINE_COUNTRY_CODE(UZ, Uzbekistan),
		SLIB_DEFINE_COUNTRY_CODE(VA, HolySee),
		SLIB_DEFINE_COUNTRY_CODE(VC, SaintVincentAndGrenadines),
		SLIB_DEFINE_COUNTRY_CODE(VE, Venezuela),
		SLIB_DEFINE_COUNTRY_CODE(VG, VirginIslands_British),
		SLIB_DEFINE_COUNTRY_CODE(VI, VirginIslands_US),
		SLIB_DEFINE_COUNTRY_CODE(VN, Vietnam),
		SLIB_DEFINE_COUNTRY_CODE(VU, Vanuatu),
		SLIB_DEFINE_COUNTRY_CODE(WF, WallisAndFutuna),
		SLIB_DEFINE_COUNTRY_CODE(WS, Samoa),
		SLIB_DEFINE_COUNTRY_CODE(YE, Yemen),
		SLIB_DEFINE_COUNTRY_CODE(YT, Mayotte),
		SLIB_DEFINE_COUNTRY_CODE(ZA, SouthAfrica),
		SLIB_DEFINE_COUNTRY_CODE(ZM, Zambia),
		SLIB_DEFINE_COUNTRY_CODE(ZW, Zimbabwe)

	};


/*
	 Script Code by ISO 15924 (4 letter)
*/
	
#define SLIB_LANGUAGE_SCRIPT(CODE) (slib::LanguageScript)(SLIB_MAKE_DWORD((#CODE)[3], (#CODE)[2], (#CODE)[1], (#CODE)[0]))
	
#define SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(CODE, NAME) \
	CODE = (int)(SLIB_LANGUAGE_SCRIPT(CODE)), \
	NAME = CODE
	
#define SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(CODE) \
	CODE = (int)(SLIB_LANGUAGE_SCRIPT(CODE))

	enum class LanguageScript
	{
		Unknown = 0,
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Adlm, Adlam),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Afak, Afaka),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Aghb, CaucasianAlbanian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Ahom),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Arab, Arabic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Aran, ArabicNastaliq),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Armi, ImperialAramaic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Armn, ArmenianArmenia),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Avst, Avestan),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bali, Balinese),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bamu, Bamum),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bass, BassaVah),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Batk, Batak),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Beng, Bengali),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bhks, Bhaiksuki),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Blis, Blissymbols),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bopo, Bopomofo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Brah, Brahmi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Brai, Braille),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Bugi, Buginese),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Buhd, Buhid),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cakm, Chakma),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cans, UnifiedCanadianAboriginalSyllabics),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cari, Carian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Cham),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cher, Cherokee),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cirt, Cirth),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Copt, Coptic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cpmn, CyproMinoan),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cprt, CypriotSyllabary),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cyrl, Cyrillic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Cyrs, Cyrillic_OldChurchSlavonic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Deva, Devanagari),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Dogr, Dogra),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Dsrt, Deseret),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Dupl, DuployanShorthand),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Egyd, EgyptianDemotic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Egyh, EgyptianHieratic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Egyp, EgyptianHieroglyphs),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Elba, Elbasan),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Elym, Elymaic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Ethi, Ethiopic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Geok, Khutsuri),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Geor, Georgian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Glag, Glagolitic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Gong, GunjalaGondi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Gonm, MasaramGondi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Goth, Gothic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Gran, Grantha),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Grek, Greek),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Gujr, Gujarati),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Guru, Gurmukhi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hanb, HanBopomofo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hang, Hangul),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hani, Han),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hano, Hanunoo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hans, ChineseSimplified),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hant, ChineseTraditional),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hatr, Hatran),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hebr, Hebrew),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hira, Hiragana),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hluw, AnatolianHieroglyphs),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hmng, PahawhHmong),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hmnp, NyiakengPuachueHmong),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hrkt, JapaneseSyllabaries),
		KatakanaHiragana = Hrkt,
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Hung, OldHungarian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Inds, Indus),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Ital, OldItalic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Jamo), // Subset of Korean
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Java, Javanese),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Jpan, Japanese),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Jurc, Jurchen),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kali, KayahLi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kana, Katakana),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Khar, Kharoshthi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Khmr, Khmer),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Khoj, Khojki),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kitl, KhitanLarge),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kits, KhitanSmall),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Knda, Kannada),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kore, Korean),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kpel, Kpelle),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Kthi, Kaithi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Lana, TaiTham),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Laoo, Lao),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Latf, LatinFraktur),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Latg, LatinGaelic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Latn, Latin),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Leke),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Lepc, Lepcha),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Limb, Limbu),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Lina, LinearA),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Linb, LinearB),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Lisu),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Loma),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Lyci, Lycian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Lydi, Lydian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mahj, Mahajani),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Maka, Makasar),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mand, Mandaic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mani, Manichaean),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Marc, Marchen),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Maya, MayanHieroglyphs),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Medf, Medefaidrin),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mend, MendeKikakui),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Merc, MeroiticCursive),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mero, MeroiticHieroglyphs),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mlym, Malayalam),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Modi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mong, Mongolian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Moon),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mroo, Mro),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mtei, MeiteiMayek),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mult, Multani),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Mymr, Myanmar),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nand, Nandinagari),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Narb, OldNorthArabian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nbat, Nabataean),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Newa, Newar),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nkdb, NaxiDongba),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nkgb, NakhiGeba),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nkoo, NKo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Nshu, Nushu),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Ogam, Ogham),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Olck, OlChiki),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Orkh, OldTurkic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Orya, Oriya),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Osge, Osage),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Osma, Osmanya),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Palm, Palmyrene),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Pauc, PauCinHau),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Perm, OldPermic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Phag, Phags_pa),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Phli, InscriptionalPahlavi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Phlp, PsalterPahlavi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Phlv, BookPahlavi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Phnx, Phoenician),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Piqd, Klingon),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Plrd, Miao),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Prti, InscriptionalParthian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Rjng, Rejang),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Rohg, HanifiRohingya),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Roro, Rongorongo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Runr, Runic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Samr, Samaritan),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sara, Sarati),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sarb, OldSouthArabian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Saur, Saurashtra),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sgnw, SignWriting),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Shaw, Shavian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Shrd, Sharada),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Shui, Shuishu),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sidd, Siddham),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sind, Khudawadi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sinh, Sinhala),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sogd, Sogdian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sogo, OldSogdian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sora, SoraSompeng),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Soyo, Soyombo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sund, Sundanese),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Sylo, SylotiNagri),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Syrc, Syriac),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Syre, SyriacEstrangelo),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Syrj, SyriacWestern),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Syrn, SyriacEastern),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tagb, Tagbanwa),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Takr, Takri),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tale, TaiLe),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Talu, NewTaiLue),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Taml, Tamil),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tang, Tangut),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tavt, TaiViet),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Telu, Telugu),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Teng, Tengwar),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tfng, Tifinagh),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tglg, Tagalog),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Thaa, Thaana),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE1(Thai),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tibt, Tibetan),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Tirh, Tirhuta),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Ugar, Ugaritic),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Vaii, Vai),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Visp, VisibleSpeech),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Wara, WarangCiti),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Wcho, Wancho),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Wole, Woleai),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Xpeo, OldPersian),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Xsux, Cuneiform),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Yiii, Yi),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zanb, ZanabazarSquare),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zinh, InheritedScript),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zmth, MathematicalNotation),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zsye, SymbolsEmoji),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zsym, Symbols),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zxxx, UnwrittenDocuments),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zyyy, UndeterminedScript),
		SLIB_DEFINE_LANGUAGE_SCRIPT_CODE(Zzzz, UncodedScript)
		
	};

	
#define SLIB_LOCALE(LANG, SCRIPT, COUNTRY) SLIB_MAKE_QWORD4(SCRIPT, SLIB_MAKE_DWORD2(COUNTRY, LANG))

	class Locale
	{
	public:
		sl_uint64 value;
		SLIB_MEMBERS_OF_PRIMITIVE_WRAPPER(Locale, sl_uint64, value)
	
		enum : sl_uint64 {
			Unknown = 0,
			ar = SLIB_LOCALE(Language::Arabic, LanguageScript::Unknown, Country::Unknown),
			Arabic = ar,
			zh = SLIB_LOCALE(Language::Chinese, LanguageScript::Unknown, Country::Unknown),
			Chinese = zh,
			zh_CN = SLIB_LOCALE(Language::Chinese, LanguageScript::ChineseSimplified, Country::China),
			China = zh_CN,
			zh_TW = SLIB_LOCALE(Language::Chinese, LanguageScript::ChineseTraditional, Country::Taiwan),
			Taiwan = zh_TW,
			en = SLIB_LOCALE(Language::English, LanguageScript::Unknown, Country::Unknown),
			English = en,
			en_GB = SLIB_LOCALE(Language::English, LanguageScript::Unknown, Country::UnitedKingdom),
			UnitedKingdom = en_GB,
			UK = en_GB,
			en_US = SLIB_LOCALE(Language::English, LanguageScript::Unknown, Country::UnitedStates),
			UnitedStates = en_US,
			US = en_US,
			fr = SLIB_LOCALE(Language::French, LanguageScript::Unknown, Country::Unknown),
			French = fr,
			fr_FR = SLIB_LOCALE(Language::French, LanguageScript::Unknown, Country::France),
			France = fr_FR,
			de = SLIB_LOCALE(Language::German, LanguageScript::Unknown, Country::Unknown),
			German = de,
			de_DE = SLIB_LOCALE(Language::German, LanguageScript::Unknown, Country::Germany),
			Germany = de_DE,
			it = SLIB_LOCALE(Language::Italian, LanguageScript::Unknown, Country::Unknown),
			Italian = it,
			it_IT = SLIB_LOCALE(Language::Italian, LanguageScript::Unknown, Country::Italy),
			Italy = it_IT,
			ja = SLIB_LOCALE(Language::Japanese, LanguageScript::Unknown, Country::Unknown),
			Japanese = ja,
			ja_JP = SLIB_LOCALE(Language::Japanese, LanguageScript::Unknown, Country::Japan),
			Japan = ja_JP,
			ko = SLIB_LOCALE(Language::Korean, LanguageScript::Unknown, Country::Unknown),
			Korean = ko,
			ko_KP = SLIB_LOCALE(Language::Korean, LanguageScript::Unknown, Country::NorthKorea),
			NorthKorea = ko_KP,
			DPRK = ko_KP,
			ko_KR = SLIB_LOCALE(Language::Korean, LanguageScript::Unknown, Country::SouthKorea),
			SouthKorea = ko_KR,
			es = SLIB_LOCALE(Language::Spanish, LanguageScript::Unknown, Country::Unknown),
			Spanish = es,
			es_ES = SLIB_LOCALE(Language::Spanish, LanguageScript::Unknown, Country::Spain),
			Spain = es_ES,
			ru = SLIB_LOCALE(Language::Russian, LanguageScript::Unknown, Country::Unknown),
			Russian = ru,
			ru_RU = SLIB_LOCALE(Language::Russian, LanguageScript::Unknown, Country::Russia),
			Russia = ru_RU
		};

	public:
		constexpr Locale() : value(Locale::Unknown) {}

		Locale(Language language);

		Locale(Language language, Country country);

		Locale(Language language, LanguageScript script, Country country);
		
		// [ISO 639-1]-[ISO 3166-1 alpha-2]  or  [ISO 639-1]-[ISO 15924]-[ISO 3166-1 alpha-2])
		Locale(const String& name);
		
	public:
		sl_bool isValid() const;
		
		sl_bool isInvalid() const;
		
		
		Language getLanguage() const;
		
		void setLanguage(const Language& language);
		
		// ISO 639-1
		String getLanguageCode() const;

		String getLanguageName() const;

		
		LanguageScript getScript() const;
		
		void setScript(const LanguageScript& script);
		
		// ISO 15924
		String getScriptCode() const;
		
		
		Country getCountry() const;
	
		void setCountry(const Country& country);

		// ISO 3166-1 alpha-2
		String getCountryCode() const;

		String getCountryName() const;

		String getCountryLongName() const;

		
		String toString(sl_char8 delimiter='-') const;
		
		template <class ST>
		static sl_bool parse(const ST& str, Locale* _out) noexcept
		{
			return Parse(str, _out);
		}
		
		template <class ST>
		sl_bool parse(const ST& str) noexcept
		{
			return Parse(str, this);
		}

	public:
		static String getLanguageName(Language language);

		// ISO 639-1
		static Language getLanguageFromCode(const sl_char8* code);
	
		// ISO 639-1
		static Language getLanguageFromCode(const sl_char16* code);

		// ISO 639-1
		static Language getLanguageFromCode(const String& code);

		// ISO 639-1
		static void getLanguageCode(Language language, sl_char8* _output);
		
		// ISO 639-1
		static String getLanguageCode(Language language);

		static sl_bool isValidLanguageCode(Language language);
		

		// ISO 15924
		static LanguageScript getScriptFromCode(const sl_char8* code);
		
		// ISO 15924
		static LanguageScript getScriptFromCode(const sl_char16* code);
		
		// ISO 15924
		static LanguageScript getScriptFromCode(const String& code);
		
		// ISO 15924
		static void getScriptCode(LanguageScript language, sl_char8* _output);
		
		// ISO 15924
		static String getScriptCode(LanguageScript language);
		
		
		static String getCountryName(Country country);

		static String getCountryLongName(Country country);

		// ISO 3166-1 alpha-2
		static Country getCountryFromCode(const sl_char8* code);

		// ISO 3166-1 alpha-2
		static Country getCountryFromCode(const sl_char16* code);

		// ISO 3166-1 alpha-2
		static Country getCountryFromCode(const String& code);

		// ISO 3166-1 alpha-2
		static void getCountryCode(Country country, sl_char8* _output);
		
		// ISO 3166-1 alpha-2
		static String getCountryCode(Country country);

		static sl_bool isValidCountryCode(Country country);
		
	public:
		static Locale getCurrent();
		
		static Function<void()> addOnChangeCurrentLocale(const Function<void()>& callback);
		
		static void removeOnChangeCurrentLocale(const Function<void()>& callback);

		static void dispatchChangeCurrentLocale();
		
	private:
		static void _setupOnChangeCurrentLocale();
		
	};
	
	template <>
	class Hash<Locale>
	{
	public:
		constexpr sl_size operator()(const Locale& locale) const
		{
			return Rehash64ToSize(locale.value);
		}

	};

	template <>
	sl_reg Parser<Locale, sl_char8>::parse(Locale* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg Parser<Locale, sl_char16>::parse(Locale* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
}

#endif
