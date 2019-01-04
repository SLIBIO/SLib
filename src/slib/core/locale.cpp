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

#include "slib/core/locale.h"

#include "slib/core/list.h"
#include "slib/core/safe_static.h"

namespace slib
{

#define DEFINE_LANGUAGE_CASE(LANG, NAME) \
	case Language::LANG: SLIB_RETURN_STRING(NAME);

	String Locale::getLanguageName(Language lang)
	{
		switch (lang) {
			DEFINE_LANGUAGE_CASE(ab, "Abkhaz")
			DEFINE_LANGUAGE_CASE(aa, "Afar")
			DEFINE_LANGUAGE_CASE(af, "Afrikaans")
			DEFINE_LANGUAGE_CASE(ak, "Akan")
			DEFINE_LANGUAGE_CASE(sq, "Albanian")
			DEFINE_LANGUAGE_CASE(am, "Amharic")
			DEFINE_LANGUAGE_CASE(ar, "Arabic")
			DEFINE_LANGUAGE_CASE(an, "Aragonese")
			DEFINE_LANGUAGE_CASE(hy, "Armenian")
			DEFINE_LANGUAGE_CASE(as, "Assamese")
			DEFINE_LANGUAGE_CASE(av, "Avaric")
			DEFINE_LANGUAGE_CASE(ae, "Avestan")
			DEFINE_LANGUAGE_CASE(ay, "Aymara")
			DEFINE_LANGUAGE_CASE(az, "Azerbaijani")
			DEFINE_LANGUAGE_CASE(bm, "Bambara")
			DEFINE_LANGUAGE_CASE(ba, "Bashkir")
			DEFINE_LANGUAGE_CASE(eu, "Basque")
			DEFINE_LANGUAGE_CASE(be, "Belarusian")
			DEFINE_LANGUAGE_CASE(bn, "Bengali")
			DEFINE_LANGUAGE_CASE(bh, "Bihari")
			DEFINE_LANGUAGE_CASE(bi, "Bislama")
			DEFINE_LANGUAGE_CASE(bs, "Bosnian")
			DEFINE_LANGUAGE_CASE(br, "Breton")
			DEFINE_LANGUAGE_CASE(bg, "Bulgarian")
			DEFINE_LANGUAGE_CASE(my, "Burmese")
			DEFINE_LANGUAGE_CASE(ca, "Catalan")
			DEFINE_LANGUAGE_CASE(ch, "Chamorro")
			DEFINE_LANGUAGE_CASE(ce, "Chechen")
			DEFINE_LANGUAGE_CASE(ny, "Chichewa")
			DEFINE_LANGUAGE_CASE(zh, "Chinese")
			DEFINE_LANGUAGE_CASE(cv, "Chuvash")
			DEFINE_LANGUAGE_CASE(kw, "Cornish")
			DEFINE_LANGUAGE_CASE(co, "Corsican")
			DEFINE_LANGUAGE_CASE(cr, "Cree")
			DEFINE_LANGUAGE_CASE(hr, "Croatian")
			DEFINE_LANGUAGE_CASE(cs, "Czech")
			DEFINE_LANGUAGE_CASE(da, "Danish")
			DEFINE_LANGUAGE_CASE(dv, "Divehi")
			DEFINE_LANGUAGE_CASE(nl, "Dutch")
			DEFINE_LANGUAGE_CASE(dz, "Dzongkha")
			DEFINE_LANGUAGE_CASE(en, "English")
			DEFINE_LANGUAGE_CASE(eo, "Esperanto")
			DEFINE_LANGUAGE_CASE(et, "Estonian")
			DEFINE_LANGUAGE_CASE(ee, "Ewe")
			DEFINE_LANGUAGE_CASE(fo, "Faroese")
			DEFINE_LANGUAGE_CASE(fj, "Fijian")
			DEFINE_LANGUAGE_CASE(fi, "Finnish")
			DEFINE_LANGUAGE_CASE(fr, "French")
			DEFINE_LANGUAGE_CASE(ff, "Fula")
			DEFINE_LANGUAGE_CASE(gl, "Galician")
			DEFINE_LANGUAGE_CASE(ka, "Georgian")
			DEFINE_LANGUAGE_CASE(de, "German")
			DEFINE_LANGUAGE_CASE(el, "Greek (modern)")
			DEFINE_LANGUAGE_CASE(gn, "Guaraní")
			DEFINE_LANGUAGE_CASE(gu, "Gujarati")
			DEFINE_LANGUAGE_CASE(ht, "Haitian")
			DEFINE_LANGUAGE_CASE(ha, "Hausa")
			DEFINE_LANGUAGE_CASE(he, "Hebrew (modern)")
			DEFINE_LANGUAGE_CASE(hz, "Herero")
			DEFINE_LANGUAGE_CASE(hi, "Hindi")
			DEFINE_LANGUAGE_CASE(ho, "Hiri Motu")
			DEFINE_LANGUAGE_CASE(hu, "Hungarian")
			DEFINE_LANGUAGE_CASE(ia, "Interlingua")
			DEFINE_LANGUAGE_CASE(id_, "Indonesian")
			DEFINE_LANGUAGE_CASE(ie, "Interlingue")
			DEFINE_LANGUAGE_CASE(ga, "Irish")
			DEFINE_LANGUAGE_CASE(ig, "Igbo")
			DEFINE_LANGUAGE_CASE(ik, "Inupiaq")
			DEFINE_LANGUAGE_CASE(io, "Ido")
			DEFINE_LANGUAGE_CASE(is, "Icelandic")
			DEFINE_LANGUAGE_CASE(it, "Italian")
			DEFINE_LANGUAGE_CASE(iu, "Inuktitut")
			DEFINE_LANGUAGE_CASE(ja, "Japanese")
			DEFINE_LANGUAGE_CASE(jv, "Javanese")
			DEFINE_LANGUAGE_CASE(kl, "Kalaallisut")
			DEFINE_LANGUAGE_CASE(kn, "Kannada")
			DEFINE_LANGUAGE_CASE(kr, "Kanuri")
			DEFINE_LANGUAGE_CASE(ks, "Kashmiri")
			DEFINE_LANGUAGE_CASE(kk, "Kazakh")
			DEFINE_LANGUAGE_CASE(km, "Khmer")
			DEFINE_LANGUAGE_CASE(ki, "Kikuyu")
			DEFINE_LANGUAGE_CASE(rw, "Kinyarwanda")
			DEFINE_LANGUAGE_CASE(ky, "Kyrgyz")
			DEFINE_LANGUAGE_CASE(kv, "Komi")
			DEFINE_LANGUAGE_CASE(kg, "Kongo")
			DEFINE_LANGUAGE_CASE(ko, "Korean")
			DEFINE_LANGUAGE_CASE(ku, "Kurdish")
			DEFINE_LANGUAGE_CASE(kj, "Kwanyama")
			DEFINE_LANGUAGE_CASE(la, "Latin")
			DEFINE_LANGUAGE_CASE(lb, "Luxembourgish")
			DEFINE_LANGUAGE_CASE(lg, "Ganda")
			DEFINE_LANGUAGE_CASE(li, "Limburgish")
			DEFINE_LANGUAGE_CASE(ln, "Lingala")
			DEFINE_LANGUAGE_CASE(lo, "Lao")
			DEFINE_LANGUAGE_CASE(lt, "Lithuanian")
			DEFINE_LANGUAGE_CASE(lu, "Luba-Katanga")
			DEFINE_LANGUAGE_CASE(lv, "Latvian")
			DEFINE_LANGUAGE_CASE(gv, "Manx")
			DEFINE_LANGUAGE_CASE(mk, "Macedonian")
			DEFINE_LANGUAGE_CASE(mg, "Malagasy")
			DEFINE_LANGUAGE_CASE(ms, "Malay")
			DEFINE_LANGUAGE_CASE(ml, "Malayalam")
			DEFINE_LANGUAGE_CASE(mt, "Maltese")
			DEFINE_LANGUAGE_CASE(mi, "Māori")
			DEFINE_LANGUAGE_CASE(mr, "Marathi")
			DEFINE_LANGUAGE_CASE(mh, "Marshallese")
			DEFINE_LANGUAGE_CASE(mn, "Mongolian")
			DEFINE_LANGUAGE_CASE(na, "Nauruan")
			DEFINE_LANGUAGE_CASE(nv, "Navajo")
			DEFINE_LANGUAGE_CASE(nd, "Northern Ndebele")
			DEFINE_LANGUAGE_CASE(ne, "Nepali")
			DEFINE_LANGUAGE_CASE(ng, "Ndonga")
			DEFINE_LANGUAGE_CASE(nb, "Norwegian Bokmal")
			DEFINE_LANGUAGE_CASE(nn, "Norwegian Nynorsk")
			DEFINE_LANGUAGE_CASE(no, "Norwegian")
			DEFINE_LANGUAGE_CASE(ii, "Nuosu")
			DEFINE_LANGUAGE_CASE(nr, "Southern Ndebele")
			DEFINE_LANGUAGE_CASE(oc, "Occitan")
			DEFINE_LANGUAGE_CASE(oj, "Ojibwe")
			DEFINE_LANGUAGE_CASE(cu, "Old Church Slavonic")
			DEFINE_LANGUAGE_CASE(om, "Oromo")
			DEFINE_LANGUAGE_CASE(or_, "Oriya")
			DEFINE_LANGUAGE_CASE(os, "Ossetian")
			DEFINE_LANGUAGE_CASE(pa, "Panjabi")
			DEFINE_LANGUAGE_CASE(pi, "Pāli")
			DEFINE_LANGUAGE_CASE(fa, "Persian")
			DEFINE_LANGUAGE_CASE(pl, "Polish")
			DEFINE_LANGUAGE_CASE(ps, "Pashto")
			DEFINE_LANGUAGE_CASE(pt, "Portuguese")
			DEFINE_LANGUAGE_CASE(qu, "Quechua")
			DEFINE_LANGUAGE_CASE(rm, "Romansh")
			DEFINE_LANGUAGE_CASE(rn, "Kirundi")
			DEFINE_LANGUAGE_CASE(ro, "Romanian")
			DEFINE_LANGUAGE_CASE(ru, "Russian")
			DEFINE_LANGUAGE_CASE(sa, "Sanskrit")
			DEFINE_LANGUAGE_CASE(sc, "Sardinian")
			DEFINE_LANGUAGE_CASE(sd, "Sindhi")
			DEFINE_LANGUAGE_CASE(se, "Northern Sami")
			DEFINE_LANGUAGE_CASE(sm, "Samoan")
			DEFINE_LANGUAGE_CASE(sg, "Sango")
			DEFINE_LANGUAGE_CASE(sr, "Serbian")
			DEFINE_LANGUAGE_CASE(gd, "Scottish Gaelic")
			DEFINE_LANGUAGE_CASE(sn, "Shona")
			DEFINE_LANGUAGE_CASE(si, "Sinhala")
			DEFINE_LANGUAGE_CASE(sk, "Slovak")
			DEFINE_LANGUAGE_CASE(sl, "Slovene")
			DEFINE_LANGUAGE_CASE(so, "Somali")
			DEFINE_LANGUAGE_CASE(st, "Southern Sotho")
			DEFINE_LANGUAGE_CASE(es, "Spanish")
			DEFINE_LANGUAGE_CASE(su, "Sundanese")
			DEFINE_LANGUAGE_CASE(sw, "Swahili")
			DEFINE_LANGUAGE_CASE(ss, "Swati")
			DEFINE_LANGUAGE_CASE(sv, "Swedish")
			DEFINE_LANGUAGE_CASE(ta, "Tamil")
			DEFINE_LANGUAGE_CASE(te, "Telugu")
			DEFINE_LANGUAGE_CASE(tg, "Tajik")
			DEFINE_LANGUAGE_CASE(th, "Thai")
			DEFINE_LANGUAGE_CASE(ti, "Tigrinya")
			DEFINE_LANGUAGE_CASE(bo, "Tibetan")
			DEFINE_LANGUAGE_CASE(tk, "Turkmen")
			DEFINE_LANGUAGE_CASE(tl, "Tagalog")
			DEFINE_LANGUAGE_CASE(tn, "Tswana")
			DEFINE_LANGUAGE_CASE(to, "Tonga")
			DEFINE_LANGUAGE_CASE(tr, "Turkish")
			DEFINE_LANGUAGE_CASE(ts, "Tsonga")
			DEFINE_LANGUAGE_CASE(tt, "Tatar")
			DEFINE_LANGUAGE_CASE(tw, "Twi")
			DEFINE_LANGUAGE_CASE(ty, "Tahitian")
			DEFINE_LANGUAGE_CASE(ug, "Uyghur")
			DEFINE_LANGUAGE_CASE(uk, "Ukrainian")
			DEFINE_LANGUAGE_CASE(ur, "Urdu")
			DEFINE_LANGUAGE_CASE(uz, "Uzbek")
			DEFINE_LANGUAGE_CASE(ve, "Venda")
			DEFINE_LANGUAGE_CASE(vi, "Vietnamese")
			DEFINE_LANGUAGE_CASE(vo, "Volapuk")
			DEFINE_LANGUAGE_CASE(wa, "Walloon")
			DEFINE_LANGUAGE_CASE(cy, "Welsh")
			DEFINE_LANGUAGE_CASE(wo, "Wolof")
			DEFINE_LANGUAGE_CASE(fy, "Western Frisian")
			DEFINE_LANGUAGE_CASE(xh, "Xhosa")
			DEFINE_LANGUAGE_CASE(yi, "Yiddish")
			DEFINE_LANGUAGE_CASE(yo, "Yoruba")
			DEFINE_LANGUAGE_CASE(za, "Zhuang")
			DEFINE_LANGUAGE_CASE(zu, "Zulu")
			case Language::Unknown:
				break;
		}
		return sl_null;
	}

	Language Locale::getLanguageFromCode(const sl_char8* code)
	{
		if (code && code[0] && code[1]) {
			return (Language)(SLIB_MAKE_WORD(code[1], code[0]));
		}
		return Language::Unknown;
	}

	Language Locale::getLanguageFromCode(const sl_char16* code)
	{
		if (code && code[0] && code[1]) {
			return (Language)(SLIB_MAKE_WORD(code[1], code[0]));
		}
		return Language::Unknown;
	}

	Language Locale::getLanguageFromCode(const String& code)
	{
		if (code.getLength() == 2) {
			return getLanguageFromCode(code.getData());
		}
		return Language::Unknown;
	}

	void Locale::getLanguageCode(Language language, sl_char8* sz)
	{
		sz[0] = SLIB_GET_BYTE0((int)language);
		sz[1] = SLIB_GET_BYTE1((int)language);
	}
	
	String Locale::getLanguageCode(Language language)
	{
		char sz[3];
		getLanguageCode(language, sz);
		sz[2] = 0;
		return sz;
	}

	sl_bool Locale::isValidLanguageCode(Language language)
	{
		return getLanguageName(language).isNotNull();
	}

	
	LanguageScript Locale::getScriptFromCode(const sl_char8* code)
	{
		if (code && code[0] && code[1] && code[2] && code[3]) {
			return (LanguageScript)(SLIB_MAKE_DWORD(code[3], code[2], code[1], code[0]));
		}
		return LanguageScript::Unknown;
	}
	
	LanguageScript Locale::getScriptFromCode(const sl_char16* code)
	{
		if (code && code[0] && code[1] && code[2] && code[3]) {
			return (LanguageScript)(SLIB_MAKE_DWORD(code[3], code[2], code[1], code[0]));
		}
		return LanguageScript::Unknown;
	}
	
	LanguageScript Locale::getScriptFromCode(const String& code)
	{
		if (code.getLength() == 2) {
			return getScriptFromCode(code.getData());
		}
		return LanguageScript::Unknown;
	}
	
	void Locale::getScriptCode(LanguageScript script, sl_char8* sz)
	{
		sz[0] = SLIB_GET_BYTE0((sl_uint32)script);
		sz[1] = SLIB_GET_BYTE1((sl_uint32)script);
		sz[2] = SLIB_GET_BYTE2((sl_uint32)script);
		sz[3] = SLIB_GET_BYTE3((sl_uint32)script);
	}
	
	String Locale::getScriptCode(LanguageScript script)
	{
		char sz[5];
		getScriptCode(script, sz);
		sz[4] = 0;
		return sz;
	}


#define DEFINE_COUNTRY_CASE(COUNTRY, NAME) \
	case Country::COUNTRY: SLIB_RETURN_STRING(NAME);

	String Locale::getCountryName(Country country)
	{
		switch (country) {
			DEFINE_COUNTRY_CASE(AD, "Andorra")
			DEFINE_COUNTRY_CASE(AE, "United Arab Emirates")
			DEFINE_COUNTRY_CASE(AF, "Afghanistan")
			DEFINE_COUNTRY_CASE(AG, "Antigua and Barbuda")
			DEFINE_COUNTRY_CASE(AI, "Anguilla")
			DEFINE_COUNTRY_CASE(AL, "Albania")
			DEFINE_COUNTRY_CASE(AM, "Armenia")
			DEFINE_COUNTRY_CASE(AO, "Angola")
			DEFINE_COUNTRY_CASE(AQ, "Antarctica")
			DEFINE_COUNTRY_CASE(AR, "Argentina")
			DEFINE_COUNTRY_CASE(AS, "American Samoa")
			DEFINE_COUNTRY_CASE(AT, "Austria")
			DEFINE_COUNTRY_CASE(AU, "Australia")
			DEFINE_COUNTRY_CASE(AW, "Aruba")
			DEFINE_COUNTRY_CASE(AX, "Aland Islands")
			DEFINE_COUNTRY_CASE(AZ, "Azerbaijan")
			DEFINE_COUNTRY_CASE(BA, "Bosnia and Herzegovina")
			DEFINE_COUNTRY_CASE(BB, "Barbados")
			DEFINE_COUNTRY_CASE(BD, "Bangladesh")
			DEFINE_COUNTRY_CASE(BE, "Belgium")
			DEFINE_COUNTRY_CASE(BF, "Burkina Faso")
			DEFINE_COUNTRY_CASE(BG, "Bulgaria")
			DEFINE_COUNTRY_CASE(BH, "Bahrain")
			DEFINE_COUNTRY_CASE(BI, "Burundi")
			DEFINE_COUNTRY_CASE(BJ, "Benin")
			DEFINE_COUNTRY_CASE(BL, "Saint Barthelemy")
			DEFINE_COUNTRY_CASE(BM, "Bermuda")
			DEFINE_COUNTRY_CASE(BN, "Brunei Darussalam")
			DEFINE_COUNTRY_CASE(BO, "Bolivia")
			DEFINE_COUNTRY_CASE(BQ, "Bonaire, Sint Eustatius and Saba")
			DEFINE_COUNTRY_CASE(BR, "Brazil")
			DEFINE_COUNTRY_CASE(BS, "Bahamas")
			DEFINE_COUNTRY_CASE(BT, "Bhutan")
			DEFINE_COUNTRY_CASE(BV, "Bouvet Island")
			DEFINE_COUNTRY_CASE(BW, "Botswana")
			DEFINE_COUNTRY_CASE(BY, "Belarus")
			DEFINE_COUNTRY_CASE(BZ, "Belize")
			DEFINE_COUNTRY_CASE(CA, "Canada")
			DEFINE_COUNTRY_CASE(CC, "Cocos Islands")
			DEFINE_COUNTRY_CASE(CD, "DR Congo")
			DEFINE_COUNTRY_CASE(CF, "Central African Republic")
			DEFINE_COUNTRY_CASE(CG, "Congo")
			DEFINE_COUNTRY_CASE(CH, "Switzerland")
			DEFINE_COUNTRY_CASE(CI, "Ivory Coast")
			DEFINE_COUNTRY_CASE(CK, "Cook Islands")
			DEFINE_COUNTRY_CASE(CL, "Chile")
			DEFINE_COUNTRY_CASE(CM, "Cameroon")
			DEFINE_COUNTRY_CASE(CN, "China")
			DEFINE_COUNTRY_CASE(CO, "Colombia")
			DEFINE_COUNTRY_CASE(CR, "Costa Rica")
			DEFINE_COUNTRY_CASE(CU, "Cuba")
			DEFINE_COUNTRY_CASE(CV, "Cabo Verde")
			DEFINE_COUNTRY_CASE(CW, "Curacao")
			DEFINE_COUNTRY_CASE(CX, "Christmas Island")
			DEFINE_COUNTRY_CASE(CY, "Cyprus")
			DEFINE_COUNTRY_CASE(CZ, "Czech")
			DEFINE_COUNTRY_CASE(DE, "Germany")
			DEFINE_COUNTRY_CASE(DJ, "Djibouti")
			DEFINE_COUNTRY_CASE(DK, "Denmark")
			DEFINE_COUNTRY_CASE(DM, "Dominica")
			DEFINE_COUNTRY_CASE(DO, "Dominican Republic")
			DEFINE_COUNTRY_CASE(DZ, "Algeria")
			DEFINE_COUNTRY_CASE(EC, "Ecuador")
			DEFINE_COUNTRY_CASE(EE, "Estonia")
			DEFINE_COUNTRY_CASE(EG, "Egypt")
			DEFINE_COUNTRY_CASE(EH, "Western Sahara")
			DEFINE_COUNTRY_CASE(ER, "Eritrea")
			DEFINE_COUNTRY_CASE(ES, "Spain")
			DEFINE_COUNTRY_CASE(ET, "Ethiopia")
			DEFINE_COUNTRY_CASE(FI, "Finland")
			DEFINE_COUNTRY_CASE(FJ, "Fiji")
			DEFINE_COUNTRY_CASE(FK, "Falkland Islands")
			DEFINE_COUNTRY_CASE(FM, "Micronesia")
			DEFINE_COUNTRY_CASE(FO, "Faroe Islands")
			DEFINE_COUNTRY_CASE(FR, "France")
			DEFINE_COUNTRY_CASE(GA, "Gabon")
			DEFINE_COUNTRY_CASE(GB, "United Kingdom")
			DEFINE_COUNTRY_CASE(GD, "Grenada")
			DEFINE_COUNTRY_CASE(GE, "Georgia")
			DEFINE_COUNTRY_CASE(GF, "French Guiana")
			DEFINE_COUNTRY_CASE(GG, "Guernsey")
			DEFINE_COUNTRY_CASE(GH, "Ghana")
			DEFINE_COUNTRY_CASE(GI, "Gibraltar")
			DEFINE_COUNTRY_CASE(GL, "Greenland")
			DEFINE_COUNTRY_CASE(GM, "Gambia")
			DEFINE_COUNTRY_CASE(GN, "Guinea")
			DEFINE_COUNTRY_CASE(GP, "Guadeloupe")
			DEFINE_COUNTRY_CASE(GQ, "Equatorial Guinea")
			DEFINE_COUNTRY_CASE(GR, "Greece")
			DEFINE_COUNTRY_CASE(GS, "South Georgia and the South Sandwich Islands")
			DEFINE_COUNTRY_CASE(GT, "Guatemala")
			DEFINE_COUNTRY_CASE(GU, "Guam")
			DEFINE_COUNTRY_CASE(GW, "Guinea-Bissau")
			DEFINE_COUNTRY_CASE(GY, "Guyana")
			DEFINE_COUNTRY_CASE(HK, "Hong Kong")
			DEFINE_COUNTRY_CASE(HM, "Heard Island and McDonald Islands")
			DEFINE_COUNTRY_CASE(HN, "Honduras")
			DEFINE_COUNTRY_CASE(HR, "Croatia")
			DEFINE_COUNTRY_CASE(HT, "Haiti")
			DEFINE_COUNTRY_CASE(HU, "Hungary")
			DEFINE_COUNTRY_CASE(ID, "Indonesia")
			DEFINE_COUNTRY_CASE(IE, "Ireland")
			DEFINE_COUNTRY_CASE(IL, "Israel")
			DEFINE_COUNTRY_CASE(IM, "Isle of Man")
			DEFINE_COUNTRY_CASE(IN, "India")
			DEFINE_COUNTRY_CASE(IO, "British Indian Ocean Territory")
			DEFINE_COUNTRY_CASE(IQ, "Iraq")
			DEFINE_COUNTRY_CASE(IR, "Iran")
			DEFINE_COUNTRY_CASE(IS, "Iceland")
			DEFINE_COUNTRY_CASE(IT, "Italy")
			DEFINE_COUNTRY_CASE(JE, "Jersey")
			DEFINE_COUNTRY_CASE(JM, "Jamaica")
			DEFINE_COUNTRY_CASE(JO, "Jordan")
			DEFINE_COUNTRY_CASE(JP, "Japan")
			DEFINE_COUNTRY_CASE(KE, "Kenya")
			DEFINE_COUNTRY_CASE(KG, "Kyrgyzstan")
			DEFINE_COUNTRY_CASE(KH, "Cambodia")
			DEFINE_COUNTRY_CASE(KI, "Kiribati")
			DEFINE_COUNTRY_CASE(KM, "Comoros")
			DEFINE_COUNTRY_CASE(KN, "Saint Kitts and Nevis")
			DEFINE_COUNTRY_CASE(KP, "North Korea")
			DEFINE_COUNTRY_CASE(KR, "South Korea")
			DEFINE_COUNTRY_CASE(KW, "Kuwait")
			DEFINE_COUNTRY_CASE(KY, "Cayman Islands")
			DEFINE_COUNTRY_CASE(KZ, "Kazakhstan")
			DEFINE_COUNTRY_CASE(LA, "Laos")
			DEFINE_COUNTRY_CASE(LB, "Lebanon")
			DEFINE_COUNTRY_CASE(LC, "Saint Lucia")
			DEFINE_COUNTRY_CASE(LI, "Liechtenstein")
			DEFINE_COUNTRY_CASE(LK, "Sri Lanka")
			DEFINE_COUNTRY_CASE(LR, "Liberia")
			DEFINE_COUNTRY_CASE(LS, "Lesotho")
			DEFINE_COUNTRY_CASE(LT, "Lithuania")
			DEFINE_COUNTRY_CASE(LU, "Luxembourg")
			DEFINE_COUNTRY_CASE(LV, "Latvia")
			DEFINE_COUNTRY_CASE(LY, "Libya")
			DEFINE_COUNTRY_CASE(MA, "Morocco")
			DEFINE_COUNTRY_CASE(MC, "Monaco")
			DEFINE_COUNTRY_CASE(MD, "Moldova")
			DEFINE_COUNTRY_CASE(ME, "Montenegro")
			DEFINE_COUNTRY_CASE(MF, "Saint Martin")
			DEFINE_COUNTRY_CASE(MG, "Madagascar")
			DEFINE_COUNTRY_CASE(MH, "Marshall Islands")
			DEFINE_COUNTRY_CASE(MK, "Macedonia")
			DEFINE_COUNTRY_CASE(ML, "Mali")
			DEFINE_COUNTRY_CASE(MM, "Myanmar")
			DEFINE_COUNTRY_CASE(MN, "Mongolia")
			DEFINE_COUNTRY_CASE(MO, "Macao")
			DEFINE_COUNTRY_CASE(MP, "Northern Mariana Islands")
			DEFINE_COUNTRY_CASE(MQ, "Martinique")
			DEFINE_COUNTRY_CASE(MR, "Mauritania")
			DEFINE_COUNTRY_CASE(MS, "Montserrat")
			DEFINE_COUNTRY_CASE(MT, "Malta")
			DEFINE_COUNTRY_CASE(MU, "Mauritius")
			DEFINE_COUNTRY_CASE(MV, "Maldives")
			DEFINE_COUNTRY_CASE(MW, "Malawi")
			DEFINE_COUNTRY_CASE(MX, "Mexico")
			DEFINE_COUNTRY_CASE(MY, "Malaysia")
			DEFINE_COUNTRY_CASE(MZ, "Mozambique")
			DEFINE_COUNTRY_CASE(NA, "Namibia")
			DEFINE_COUNTRY_CASE(NC, "New Caledonia")
			DEFINE_COUNTRY_CASE(NE, "Niger")
			DEFINE_COUNTRY_CASE(NF, "Norfolk Island")
			DEFINE_COUNTRY_CASE(NG, "Nigeria")
			DEFINE_COUNTRY_CASE(NI, "Nicaragua")
			DEFINE_COUNTRY_CASE(NL, "Netherlands")
			DEFINE_COUNTRY_CASE(NO_, "Norway")
			DEFINE_COUNTRY_CASE(NP, "Nepal")
			DEFINE_COUNTRY_CASE(NR, "Nauru")
			DEFINE_COUNTRY_CASE(NU, "Niue")
			DEFINE_COUNTRY_CASE(NZ, "New Zealand")
			DEFINE_COUNTRY_CASE(OM, "Oman")
			DEFINE_COUNTRY_CASE(PA, "Panama")
			DEFINE_COUNTRY_CASE(PE, "Peru")
			DEFINE_COUNTRY_CASE(PF, "French Polynesia")
			DEFINE_COUNTRY_CASE(PG, "Papua New Guinea")
			DEFINE_COUNTRY_CASE(PH, "Philippines")
			DEFINE_COUNTRY_CASE(PK, "Pakistan")
			DEFINE_COUNTRY_CASE(PL, "Poland")
			DEFINE_COUNTRY_CASE(PM, "Saint Pierre and Miquelon")
			DEFINE_COUNTRY_CASE(PN, "Pitcairn")
			DEFINE_COUNTRY_CASE(PR, "Puerto Rico")
			DEFINE_COUNTRY_CASE(PS, "State of Palestine")
			DEFINE_COUNTRY_CASE(PT, "Portugal")
			DEFINE_COUNTRY_CASE(PW, "Palau")
			DEFINE_COUNTRY_CASE(PY, "Paraguay")
			DEFINE_COUNTRY_CASE(QA, "Qatar")
			DEFINE_COUNTRY_CASE(RE, "Reunion")
			DEFINE_COUNTRY_CASE(RO, "Romania")
			DEFINE_COUNTRY_CASE(RS, "Serbia")
			DEFINE_COUNTRY_CASE(RU, "Russia")
			DEFINE_COUNTRY_CASE(RW, "Rwanda")
			DEFINE_COUNTRY_CASE(SA, "Saudi Arabia")
			DEFINE_COUNTRY_CASE(SB, "Solomon Islands")
			DEFINE_COUNTRY_CASE(SC, "Seychelles")
			DEFINE_COUNTRY_CASE(SD, "Sudan")
			DEFINE_COUNTRY_CASE(SE, "Sweden")
			DEFINE_COUNTRY_CASE(SG, "Singapore")
			DEFINE_COUNTRY_CASE(SH, "Saint Helena, Ascension and Tristan da Cunha")
			DEFINE_COUNTRY_CASE(SI, "Slovenia")
			DEFINE_COUNTRY_CASE(SJ, "Svalbard and Jan Mayen")
			DEFINE_COUNTRY_CASE(SK, "Slovakia")
			DEFINE_COUNTRY_CASE(SL, "Sierra Leone")
			DEFINE_COUNTRY_CASE(SM, "San Marino")
			DEFINE_COUNTRY_CASE(SN, "Senegal")
			DEFINE_COUNTRY_CASE(SO, "Somalia")
			DEFINE_COUNTRY_CASE(SR, "Suriname")
			DEFINE_COUNTRY_CASE(SS, "South Sudan")
			DEFINE_COUNTRY_CASE(ST, "Sao Tome and Principe")
			DEFINE_COUNTRY_CASE(SV, "El Salvador")
			DEFINE_COUNTRY_CASE(SX, "Sint Maarten")
			DEFINE_COUNTRY_CASE(SY, "Syrian Arab Republic")
			DEFINE_COUNTRY_CASE(SZ, "Swaziland")
			DEFINE_COUNTRY_CASE(TC, "Turks and Caicos Islands")
			DEFINE_COUNTRY_CASE(TD, "Chad")
			DEFINE_COUNTRY_CASE(TF, "French Southern Territories")
			DEFINE_COUNTRY_CASE(TG, "Togo")
			DEFINE_COUNTRY_CASE(TH, "Thailand")
			DEFINE_COUNTRY_CASE(TJ, "Tajikistan")
			DEFINE_COUNTRY_CASE(TK, "Tokelau")
			DEFINE_COUNTRY_CASE(TL, "Timor-Leste")
			DEFINE_COUNTRY_CASE(TM, "Turkmenistan")
			DEFINE_COUNTRY_CASE(TN, "Tunisia")
			DEFINE_COUNTRY_CASE(TO, "Tonga")
			DEFINE_COUNTRY_CASE(TR, "Turkey")
			DEFINE_COUNTRY_CASE(TT, "Trinidad and Tobago")
			DEFINE_COUNTRY_CASE(TV, "Tuvalu")
			DEFINE_COUNTRY_CASE(TW, "Taiwan")
			DEFINE_COUNTRY_CASE(TZ, "Tanzania")
			DEFINE_COUNTRY_CASE(UA, "Ukraine")
			DEFINE_COUNTRY_CASE(UG, "Uganda")
			DEFINE_COUNTRY_CASE(UM, "United States Minor Outlying Islands")
			DEFINE_COUNTRY_CASE(US, "United States")
			DEFINE_COUNTRY_CASE(UY, "Uruguay")
			DEFINE_COUNTRY_CASE(UZ, "Uzbekistan")
			DEFINE_COUNTRY_CASE(VA, "Holy See")
			DEFINE_COUNTRY_CASE(VC, "Saint Vincent and the Grenadines")
			DEFINE_COUNTRY_CASE(VE, "Venezuela")
			DEFINE_COUNTRY_CASE(VG, "Virgin Islands, British")
			DEFINE_COUNTRY_CASE(VI, "Virgin Islands, U.S.")
			DEFINE_COUNTRY_CASE(VN, "Vietnam")
			DEFINE_COUNTRY_CASE(VU, "Vanuatu")
			DEFINE_COUNTRY_CASE(WF, "Wallis and Futuna")
			DEFINE_COUNTRY_CASE(WS, "Samoa")
			DEFINE_COUNTRY_CASE(YE, "Yemen")
			DEFINE_COUNTRY_CASE(YT, "Mayotte")
			DEFINE_COUNTRY_CASE(ZA, "South Africa")
			DEFINE_COUNTRY_CASE(ZM, "Zambia")
			DEFINE_COUNTRY_CASE(ZW, "Zimbabwe")
			case Country::Unknown:
				break;
		}
		return sl_null;
	}

	String Locale::getCountryLongName(Country country)
	{
		switch (country) {
			DEFINE_COUNTRY_CASE(BO, "Plurinational State of Bolivia")
			DEFINE_COUNTRY_CASE(CC, "Cocos (Keeling) Islands")
			DEFINE_COUNTRY_CASE(CD, "Democratic Republic of the Congo")
			DEFINE_COUNTRY_CASE(CG, "Republic of the Congo")
			DEFINE_COUNTRY_CASE(CI, "Cote d'Ivoire")
			DEFINE_COUNTRY_CASE(CZ, "Czech Republic")
			DEFINE_COUNTRY_CASE(DM, "Commonwealth of Dominica")
			DEFINE_COUNTRY_CASE(FK, "Falkland Islands (Malvinas)")
			DEFINE_COUNTRY_CASE(FM, "Federated States of Micronesia")
			DEFINE_COUNTRY_CASE(GB, "United Kingdom of Great Britain and Northern")
			DEFINE_COUNTRY_CASE(IR, "Islamic Republic of Iran")
			DEFINE_COUNTRY_CASE(KP, "Democratic People's Republic of Korea")
			DEFINE_COUNTRY_CASE(KR, "Republic of Korea")
			DEFINE_COUNTRY_CASE(LA, "Lao People's Democratic Republic")
			DEFINE_COUNTRY_CASE(MD, "Republic of Moldova")
			DEFINE_COUNTRY_CASE(MF, "Saint Martin (French part)")
			DEFINE_COUNTRY_CASE(RU, "Russian Federation")
			DEFINE_COUNTRY_CASE(SX, "Sint Maarten (Dutch part)")
			DEFINE_COUNTRY_CASE(TZ, "United Republic of Tanzania")
			DEFINE_COUNTRY_CASE(US, "United States of America")
			DEFINE_COUNTRY_CASE(VE, "Bolivarian Republic of Venezuela")
			DEFINE_COUNTRY_CASE(VN, "Viet Nam")
			default:
				break;
		}
		return getCountryName(country);
	}

	Country Locale::getCountryFromCode(const sl_char8* code)
	{
		if (code && code[0] && code[1]) {
			return (Country)(SLIB_MAKE_WORD(code[1], code[0]));
		}
		return Country::Unknown;
	}

	Country Locale::getCountryFromCode(const sl_char16* code)
	{
		if (code && code[0] && code[1]) {
			return (Country)(SLIB_MAKE_WORD(code[1], code[0]));
		}
		return Country::Unknown;
	}

	Country Locale::getCountryFromCode(const String& code)
	{
		if (code.getLength() == 2) {
			return getCountryFromCode(code.getData());
		}
		return Country::Unknown;
	}
	
	void Locale::getCountryCode(Country country, sl_char8* sz)
	{
		sz[0] = SLIB_GET_BYTE0((int)country);
		sz[1] = SLIB_GET_BYTE1((int)country);
	}

	String Locale::getCountryCode(Country country)
	{
		char sz[3];
		getCountryCode(country, sz);
		sz[2] = 0;
		return sz;
	}

	sl_bool Locale::isValidCountryCode(Country country)
	{
		return getCountryName(country).isNotNull();
	}

	Locale::Locale(Language language)
	 : value((sl_uint32)(language))
	{
	}

	Locale::Locale(Language language, Country country)
	{
		if (language == Language::Chinese) {
			LanguageScript script = LanguageScript::Unknown;
			if (country == Country::China) {
				script = LanguageScript::ChineseSimplified;
			} else if (country == Country::Taiwan || country == Country::HongKong || country == Country::Macao) {
				script = LanguageScript::ChineseTraditional;
			}
			value = SLIB_LOCALE(language, script, country);
		} else {
			value = SLIB_LOCALE(language, LanguageScript::Unknown, country);
		}
	}

	Locale::Locale(Language language, LanguageScript script, Country country)
	 : value(SLIB_LOCALE(language, script, country))
	{
	}

	Locale::Locale(const String& name)
	{
		if (!(parse(name))) {
			value = Locale::Unknown;
		}
	}
	
	sl_uint64 _priv_Locale_Unknown = 0;
	const Locale& Locale::Unknown = *((Locale*)&_priv_Locale_Unknown);

	sl_bool Locale::isValid() const
	{
		Language lang = getLanguage();
		Country country = getCountry();
		if (lang != Language::Unknown && isValidLanguageCode(lang)) {
			if (country == Country::Unknown || isValidCountryCode(country)) {
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool Locale::isInvalid() const
	{
		return !(isValid());
	}

	Language Locale::getLanguage() const
	{
		return (Language)(SLIB_GET_WORD0(value));
	}

	void Locale::setLanguage(const Language& language)
	{
		value = SLIB_LOCALE(language, getScript(), getCountry());
	}
	
	String Locale::getLanguageCode() const
	{
		return getLanguageCode(getLanguage());
	}

	String Locale::getLanguageName() const
	{
		return getLanguageName(getLanguage());
	}

	LanguageScript Locale::getScript() const
	{
		return (LanguageScript)(SLIB_GET_DWORD1(value));
	}
	
	void Locale::setScript(const LanguageScript& script)
	{
		value = SLIB_LOCALE(getLanguage(), script, getCountry());
	}

	String Locale::getScriptCode() const
	{
		return getScriptCode(getScript());
	}
	
	Country Locale::getCountry() const
	{
		return (Country)(SLIB_GET_WORD1(value));
	}

	void Locale::setCountry(const Country& country)
	{
		value = SLIB_LOCALE(getLanguage(), getScript(), country);
	}
	
	String Locale::getCountryCode() const
	{
		return getCountryCode(getCountry());
	}

	String Locale::getCountryName() const
	{
		return getCountryName(getCountry());
	}

	String Locale::getCountryLongName() const
	{
		return getCountryLongName(getCountry());
	}

	String Locale::toString(sl_char8 delimiter) const
	{
		sl_char8 sz[11];
		Language lang = getLanguage();
		if (lang != Language::Unknown) {
			getLanguageCode(lang, sz);
			LanguageScript script = getScript();
			if (script != LanguageScript::Unknown) {
				sz[2] = delimiter;
				getScriptCode(script, sz + 3);
				Country country = getCountry();
				if (country != Country::Unknown) {
					sz[7] = delimiter;
					getCountryCode(country, sz + 8);
					sz[10] = 0;
					return sz;
				} else {
					sz[7] = 0;
					return sz;
				}
			} else {
				Country country = getCountry();
				if (country != Country::Unknown) {
					sz[2] = delimiter;
					getCountryCode(country, sz + 3);
					sz[5] = 0;
					return sz;
				} else {
					sz[2] = 0;
					return sz;
				}
				return sz;
			}
		} else {
			Country country = getCountry();
			if (country != Country::Unknown) {
				sz[0] = 'e';
				sz[1] = 'n';
				sz[2] = delimiter;
				getCountryCode(country, sz + 3);
				sz[5] = 0;
				return sz;
			}
		}
		return sl_null;
	}
	
	template <class T>
	SLIB_INLINE static sl_reg _priv_Locale_parse(Locale* _out, const T* sz, sl_size i, sl_size n)
	{
		if (i + 1 >= n) {
			return SLIB_PARSE_ERROR;
		}
		
		if (SLIB_CHAR_IS_ALPHA_UPPER(sz[i]) && SLIB_CHAR_IS_ALPHA_UPPER(sz[i+1])) {
			if (_out) {
				*_out = Locale(Language::Unknown, LanguageScript::Unknown, Locale::getCountryFromCode(sz));
			}
			return i + 2;
		}

		if (!(SLIB_CHAR_IS_ALPHA_LOWER(sz[i]) && SLIB_CHAR_IS_ALPHA_LOWER(sz[i+1]))) {
			return SLIB_PARSE_ERROR;
		}
		
		Language language = Locale::getLanguageFromCode(sz + i);
		LanguageScript script = LanguageScript::Unknown;
		Country country = Country::Unknown;
		
		i += 2;
		if (i < n) {
			if ((sz[i] == '-' || sz[i] == '_')) {
				i++;
				if (i + 1 >= n) {
					return SLIB_PARSE_ERROR;
				}
				if (!(SLIB_CHAR_IS_ALPHA_UPPER(sz[i]))) {
					return SLIB_PARSE_ERROR;
				}
				if (SLIB_CHAR_IS_ALPHA_UPPER(sz[i+1])) {
					country = Locale::getCountryFromCode(sz + i);
					i += 2;
				} else {
					if (i + 3 >= n) {
						return SLIB_PARSE_ERROR;
					}
					if (!(SLIB_CHAR_IS_ALPHA_LOWER(sz[i+1]) && SLIB_CHAR_IS_ALPHA_LOWER(sz[i+2]) && SLIB_CHAR_IS_ALPHA_LOWER(sz[i+3]))) {
						return SLIB_PARSE_ERROR;
					}
					script = Locale::getScriptFromCode(sz + i);
					i += 4;
					if (i < n && (sz[i] == '-' || sz[i] == '_')) {
						i++;
						if (i + 1 >= n) {
							return SLIB_PARSE_ERROR;
						}
						if (!(SLIB_CHAR_IS_ALPHA_UPPER(sz[i]) && SLIB_CHAR_IS_ALPHA_UPPER(sz[i+1]))) {
							return SLIB_PARSE_ERROR;
						}
						country = Locale::getCountryFromCode(sz + i);
						i += 2;
					}
				}
			}
		}
		if (_out) {
			if (script == LanguageScript::Unknown) {
				*_out = Locale(language, country);
			} else {
				*_out = Locale(language, script, country);
			}
		}
		return i;
	}

	
	template <>
	sl_reg Parser<Locale, sl_char8>::parse(Locale* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Locale_parse(_out, sz, posBegin, posEnd);
	}
	
	template <>
	sl_reg Parser<Locale, sl_char16>::parse(Locale* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Locale_parse(_out, sz, posBegin, posEnd);
	}
	

	static Locale _g_priv_locale_current = Locale::Unknown;
	
	Locale Locale::getCurrent()
	{
		Locale locale = _g_priv_locale_current;
		if (locale != Locale::Unknown) {
			return locale;
		}
		return _getCurrent();
	}
	
	void Locale::setCurrent(const Locale& locale)
	{
		_g_priv_locale_current = locale;
	}

	SLIB_STATIC_ZERO_INITIALIZED(AtomicList< Function<void()> >, _g_priv_locale_callback_onChangeCurrent)
	Locale _g_priv_Locale_lastCurrent;

	Function<void()> Locale::addOnChangeCurrentLocale(const Function<void()>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_locale_callback_onChangeCurrent)) {
			return callback;
		}
		_g_priv_locale_callback_onChangeCurrent.addIfNotExist(callback);
		{
			SLIB_STATIC_SPINLOCKER(lock)
			static sl_bool flagSetup = sl_false;
			if (flagSetup) {
				return callback;
			}
			flagSetup = sl_true;
		}
		_g_priv_Locale_lastCurrent = _getCurrent();
		_setupOnChangeCurrentLocale();
		return callback;
	}
	
	void Locale::removeOnChangeCurrentLocale(const Function<void()>& callback)
	{
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_locale_callback_onChangeCurrent))) {
			_g_priv_locale_callback_onChangeCurrent.remove(callback);
		}
	}
	
	void Locale::dispatchChangeCurrentLocale()
	{
		Locale locale = _getCurrent();
		if (_g_priv_Locale_lastCurrent == locale) {
			return;
		}
		_g_priv_Locale_lastCurrent = locale;
		if (!(SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_locale_callback_onChangeCurrent))) {
			ListLocker< Function<void()> > list(_g_priv_locale_callback_onChangeCurrent);
			for (sl_size i = 0; i < list.count; i++) {
				list[i]();
			}
		}
	}
	
#if !defined(SLIB_PLATFORM_IS_MACOS)
	void Locale::_setupOnChangeCurrentLocale()
	{
	}
#endif

}

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "slib/core/platform_windows.h"

namespace slib
{
	Locale Locale::_getCurrent()
	{
		WINAPI_GetUserDefaultLocaleName api = Windows::getAPI_GetUserDefaultLocaleName();
		if (api) {
			WCHAR s[LOCALE_NAME_MAX_LENGTH];
			s[0] = 0;
			api(s, LOCALE_NAME_MAX_LENGTH);
			Locale locale;
			if (locale.parse((sl_char16*)s)) {
				return locale;
			}
		}
		LCID locale = GetUserDefaultLCID();
		char lang[3] = { 0, 0, 0 };
		GetLocaleInfoA(locale, LOCALE_SISO639LANGNAME, lang, 3);
		char country[3] = { 0, 0, 0 };
		GetLocaleInfoA(locale, LOCALE_SISO3166CTRYNAME, country, 3);
		return Locale(Locale::getLanguageFromCode(lang), Locale::getCountryFromCode(country));
	}
}

#elif defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/core/platform_android.h"

namespace slib
{
	SLIB_JNI_BEGIN_CLASS(JAndroid, "slib/platform/android/Android")
		SLIB_JNI_STATIC_METHOD(getCurrentLocale, "getCurrentLocale", "()Ljava/lang/String;");
	SLIB_JNI_END_CLASS

	Locale Locale::_getCurrent()
	{
		String str = JAndroid::getCurrentLocale.callString(sl_null);
		return Locale(str);
	}
}

#elif defined(SLIB_PLATFORM_IS_APPLE)

// defined at locale_apple.mm

#else

#include <stdlib.h>

namespace slib
{
	Locale Locale::_getCurrent()
	{
		String locale = getenv("LANG");
		sl_reg index = locale.indexOf('.');
		if (index > 0) {
			locale = locale.substring(0, index);
		}
		return Locale(locale);
	}
}

#endif
