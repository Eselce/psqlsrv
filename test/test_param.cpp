
#include <ctime>

#include "pg_type.h"

#include "pgconn.hpp"

#include "pgstmt.hpp"

#include "pgparam.hpp"

#include "test_param.hpp"

test_param::test_param(void)
{
}

test_param::~test_param(void)
{
}

// A1:AW1 (49 cols)
[[maybe_unused]] static const char *header = "ID	VID	Date	OwnerID	Artist	Format	Title	Len	Description	Views	Likes	Comments	Evaluation	Remarks	Tags	OK	Public	SongID	Last	Kind	Etag	OwnerName	LiveBroadcast	PublishTime	CategoryID	AudioLanguage	Definition	Caption	Projection	Licensed	Dimension	Duration	TopicCategories	License	Embeddable	PublicStatusViewable	MadeForKids	PublicStatus	UploadStatus	ScheduledTime	ActualStartTime	ActualEndTime	ActiveLiveChatID	DiscordFull	Skip	RsongID	DescTags	PerformanceID	RPerformanceID";

// A2:AW11 (10 rows)
static const char *data[] = {
"0	_66Ug5c3aJo	14.04.23 05:34	-_a855MsBk5Kz12_zBLb3w	LOVEBITES	Song	Lovebites - Bravehearted (1st Session With New Bassist Fami) 4K + Sub: Eng & Esp	00:06:52	On October 21, 2022, Fami was announced as the new bassist for Lovebites after the departure of the previous bassist Miho who was also the co-founder and leader of Lovebites. A bassist virtuoso, Fami has been very well known before ever joining Lovebites as she has gotten in 4 years more than 670,000 subscribers on her own personal youtube channel. That means that as of 2023, Fami actually has over 6 times the amount of subscribers that Lovebites themselves have on their own official youtube channel. Fami seems to already have brought good luck to the band as the first album with her on bass, \"Judgement Day\", released on February 22, 2023, peaked at number 5 in total album sales in Japan on the Oricon charts. That is the highest spot that Lovebites has reached up to this point in Japanese album sales. After Fami formally joined, the band recorded with her on bass a live in-studio performance of \"Bravehearted\", a track that was originally released by Lovebites on their first EP and later on their first studio album. This performance of \"Bravehearted\" with Fami is marked by Lovebites as the first official performance with her as a member of the band. A short version of this was used to announce Fami as the new bassist on youtube. This is the full length version with on screen English subtitles for the song lyrics.   Remember to support Lovebites at their official links and websites.  Lovebites´ Official YouTube Channel: https://www.youtube.com/channel/UC90wxogt_sQrP0Os0HT-xuw  Fami´s Official Youtube Channel: https://www.youtube.com/@fami4906   #Lovebites #Bravehearted #Fami #Metal	414277	5082	245	NA	CC: ES	Lovebites, Bravehearted, Fami, Power Metal, Rock, Metal, Live Performance, Live	WAHR	WAHR	601	08.02.25 05:21	youtube#video	nI7U2zL2tkd5RL4bphUT6wdOyfI	DaRunningManSubs	none	07.02.25 21:37	10	en-US	hd	WAHR	rectangular	FALSCH	2d	PT6M53S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Music_of_Asia https://en.wikipedia.org/wiki/Pop_music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1681443248:R>] https://youtu.be/_66Ug5c3aJo [06:52 CC en-US hd(2d) Music] DaRunningManSubs Lovebites - Bravehearted (1st Session With New Bassist Fami) 4K + Sub: Eng & Esp	FALSCH		#Lovebites #Fami #Metal",
"1	08b7xb7a11I	10.08.24 06:28	IuQa-ayzCqgb6kCvmtSVsQ	LOVEBITES	Concert	LOVEBITES & Fans (´Veats Shibuya´, Tokyo, May 11, 2023)	00:38:57	LOVEBITES - ´The Shining Vol.2´ (´Veats Shibuya´, ...	47438	886	58				FALSCH	WAHR	602	12.07.25 19:50	youtube#video	3M0UK0hhuQC6sJoMoJxhEHAJDRc	Серый★Лис	none	12.07.25 18:18	22	en-GB	hd	FALSCH	rectangular	FALSCH	2d	PT38M58S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Music_of_Asia	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1723264080:R>] https://youtu.be/08b7xb7a11I [38:57 en-GB hd(2d) People & Blogs] Серый★Лис LOVEBITES & Fans (´Veats Shibuya´, Tokyo, May 11, 2023)	FALSCH		",
"2	1DcY6IKh7RU	15.04.22 13:00	90wxogt_sQrP0Os0HT-xuw	LOVEBITES	OMV	LOVEBITES / The Crusade [MUSIC VIDEO]	00:04:57	Here is the video clip to \"The Crusade\" taken from...	325235	6271	316	Official			WAHR	WAHR	50	12.07.25 19:50	youtube#video	rdLvAamPO1yhil9ynBuULVxIvgw	LOVEBITES	none	12.07.25 18:18	10	ja	hd	FALSCH	rectangular	WAHR	2d	PT4M58S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1650020430:R>] https://youtu.be/1DcY6IKh7RU [04:57 ja hd(2d) Music] LOVEBITES LOVEBITES / The Crusade [MUSIC VIDEO]	FALSCH		#lovebites #heavymetal #allfemalemetal",
"3	1r37MDTRBWk	19.06.19 13:03	90wxogt_sQrP0Os0HT-xuw	LOVEBITES	Trailer	LOVEBITES / M.D.O. [Live in Tokyo 2019]	00:01:36	2019年7月10日発売となる、ヘヴィ・メタル・バンドLOVEBITESの初ライヴBlu-ray/D...	144519	1624	59	Official		ラヴバイツ, ラブバイツ, babymetal, band-maid, Unlucky, Morpheus, loudness, aldious, mary´s, blood, v	WAHR	WAHR	40	12.07.25 19:50	youtube#video	9O4agjc3gLk4_x4wNP_ZjKoOGho	LOVEBITES	none	12.07.25 18:18	10		hd	FALSCH	rectangular	WAHR	2d	PT1M37S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1560942220:R>] https://youtu.be/1r37MDTRBWk [01:36 hd(2d) Music] LOVEBITES LOVEBITES / M.D.O. [Live in Tokyo 2019]	FALSCH		",
"4	1S19rJMjCgo	01.07.24 22:13	IxiBqoElKZntsPLUP_GUhA	LOVEBITES	Clip	LOVEBITES  - Shadowmaker [Solo Section] (New Cross Inn, 14/06/2024)	00:01:26		152	9	0	Discord			WAHR	WAHR	18	12.07.25 19:50	youtube#video	IxKrqfrQDaS1zCZSixoueeH4nw4	Hadesillo	none	12.07.25 18:18	10	ja	hd	FALSCH	rectangular	FALSCH	2d	PT1M27S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1719864813:R>] https://youtu.be/1S19rJMjCgo [01:26 ja hd(2d) Music] Hadesillo LOVEBITES  - Shadowmaker [Solo Section] (New Cross Inn, 14/06/2024)	FALSCH		",
"5	1ynnXDfEnnY	19.10.20 17:39	SLX6nA3LRuguwhHZOQO1ZQ	LOVEBITES	Song	LOVEBITES〖EPILOGUE〗〖DON´T BITE THE DUST〗Daughters of the Dawn ~ Live in Tokyo 2019 【lyrics】	00:12:12	〖EPILOGUE〗 EPILOGUE is to be confident in yourself and to live your own way! There is a message. ASAMI´s persuasive singing ability! MIYAKO / MIDORI´s crying guitar solo and twin guitars! A melody like the Eagles Hotel California! MIHO / HARUNA´s accurate rhythm section! Ballad songs are songs that test playing techniques. LOVEBITES showed a great live performance. I think LOVEBITES is the one and only heavy metal band in the world.  〖EPILOGUE〗 Lyrics－ASAMI Music－ASAMI    MAO  〖DON´T BITE THE DUST〗 Lyricist－MIHO Music－MIHO    MAO  【Daughters of the Dawn ~ Live in Tokyo 2019】 CD＆DVD＆Blu-ray（Now on sale) 〖Track list〗 01. Clockwork Immortality (Intro) 02. Addicted 03. Bravehearted 04. The Crusade 05. Pledge Of The Saviour 06. Rising 07. Scream For Me 08. Break The Wall 09. Shadowmaker 10. Above The Black Sea 11. Empty Daydream 13. Journey To The Otherside 14. Edge Of The World 15. We The United 16. Epilogue 17. Do n’t Bite The Dust 18. Under The Red Sky 19. The Everlasting (Outro) (Commentary sub-audio recording by members) ＋ Bonus video \"Introduction of equipment\"  DAUGHTERS OF THE DAWN - LIVE IN TOKYO 2019 (BLU-RAY / DVD / 2CD) 〖SHOPPING FROM OVERSEAS〗-> https://lovebites.booth.pm/items/2366256  〖Purchased from Amazon Japan〗-> https://www.amazon.co.jp/-/en/LOVEBITES/dp/B07R326TWL/ref=sr_1_2?keywords=Lovebites+Daughters+of+the+Dawn+%7E+Live+in+Tokyo+2019&qid=1637017813&sr=8-2  The best-of work \"IN THE BEGINNING - THE BEST OF 2017-2021\" will be released on December 22!　The best album tracing the footsteps of the original lineup from 2017 to 2021, selected by fan vote! LOVEBITES \"IN THE BEGINNING - THE BEST OF 2017-2021  Heavy metal band LOVEBITES has released a best album that traces the footsteps of the original lineup from 2017 to 2021! The heavy metal band LOVEBITES has released a best album tracing the footsteps of their original lineup from 2017 to 2021!  The album includes a completely new song \"Nameless Warrior\". The song was written after miho´s departure and recorded while she was still with the band. The song was written after miho´s departure and recorded while she was still with the band, making it the last song with the original lineup.  The previously released songs were selected by fan vote! Newly mastered by Mika Jussiila at Finland´s prestigious Finnvox Studios. Newly mastered by Mika Jussiila at Finnvox Studios in Finland, the album is now available on 2 CDs.  In addition to the main 2CDs and the Blu-ray containing all the music videos, there are also 2 instrumental CDs of the main CD. In addition to the 2 main CDs and the Blu-ray with all the music videos, there are also 2 instrumental CDs of the main CD.  This deluxe edition includes the main 2 CDs, all music videos on Blu-ray, and 2 instrumental versions of the main CDs.  Recorded tracks CD 1 Nameless Warrior Glory To The World Winds Of Transylvania Golden Destination Thunder Vengeance Holy War Raise Some Hell When Destinies Align Swan Song Rising M.D.O./M.D.O.  CD 2 We The United Empty Daydream The Crusade Break The Wall Under The Red Sky The Hammer Of Wrath The Hammer Of Wrath 07 Shadowmaker Scream For Me Don´t Bite The Dust Edge Of The World Bravehearted  CD 3 Instrumental version of CD 1  CD 4 Instrumental version of CD 2  Blu-ray (MUSIC VIDEOS) Don´t Bite The Dust Shadowmaker The Crusade The Crusade When Destinies Align Golden Destination Winds Of Transylvania Winds Of Transylvania (VLADLOVE Version) Winds Of Transylvania (VLADLOVE Version) Glory To The World 10. Nameless Warrior  Official LOVEBITES webstore for overseas only.-> https://lovebites.booth.pm/  Purchase from Amazon Japan-> https://www.amazon.co.jp/s?k=lovebites&language=en_US&crid=GENYEZND9SJP&sprefix=LOVE%2Caps%2C292&ref=nb_sb_ss_ts-doa-p_1_4  Official LOVEBITES webstore for overseas only.-> https://lovebites.booth.pm/  Purchase from Amazon Japan-> https://www.amazon.co.jp/s?k=lovebites&language=en_US&crid=GENYEZND9SJP&sprefix=LOVE%2Caps%2C292&ref=nb_sb_ss_ts-doa-p_1_4  LOVEBITES SHOPPING FROM OVERSEAS: https://lovebites.booth.pm/  asami…Vocal midori…Guitar miyako…Guitar / Keyboard (Piano) miho…Base, Leader haruna…Drums  Official site-> https://lovebites.jp/  YouTube Channel-> https://www.youtube.com/channel/UC90wxogt_sQrP0Os0HT-xuw  Twitter-> https://twitter.com/lovebites_jp   ＃LOVEBITES ＃EPILOGUE ＃DontBiteTheDust	211686	2960	290	NA			WAHR	WAHR	512	08.02.25 05:21	youtube#video	EADiaU3jSVR0oyvpzV13AvDKOM4	okkuオック	none	07.02.25 21:37	22	en-US	hd	WAHR	rectangular	FALSCH	2d	PT12M13S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Pop_music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1603121987:R>] https://youtu.be/1ynnXDfEnnY [12:12 CC en-US hd(2d) People & Blogs] okkuオック LOVEBITES〖EPILOGUE〗〖DON´T BITE THE DUST〗Daughters of the Dawn ~ Live in Tokyo 2019 【lyrics】	FALSCH		",
"6	30yWisiRBuM	17.08.24 17:00	90wxogt_sQrP0Os0HT-xuw	LOVEBITES	OLV	LOVEBITES / Soldier Stands Solitarily [Official Live Video from \"Memorial For The Warrior Souls\"]	00:06:12	Here is the live video to \"Soldier Stands Solitari...	643003	11510	1265	Official			WAHR	WAHR	71	12.07.25 19:50	youtube#video	E5S8lrRnpnJ1n0cUA5BI9CvPJBE	LOVEBITES	none	12.07.25 18:18	10	ja	hd	FALSCH	rectangular	WAHR	2d	PT6M13S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed	17.08.24 17:00	17.08.24 17:00	17.08.24 17:08		[Premiere <t:1723906822:R>] https://youtu.be/30yWisiRBuM DONE (<t:1723907308:R>) [06:12 ja hd(2d) Music] LOVEBITES LOVEBITES / Soldier Stands Solitarily [Official Live Video from \"Memorial For The Warrior Souls\"]	FALSCH		#lovebites #japanesemetal #metallive",
"7	3XRq1YR_A0w	07.05.21 06:34	SLX6nA3LRuguwhHZOQO1ZQ	LOVEBITES	Song	LOVEBITES〖BRAVEHEARTED〗 Daughters of the Dawn~Live in Tokyo 2019	00:06:25	BRAVEHEARTED, written and composed by HARUNA, is a wonderful song, and it is the coolest HARUNA sings and smiles while playing the drums so as not to get tired until the end of the live performance.  【BRAVEHEARTED】 Lyricist－HARUNA 　　　　 ASAMI：English translation Music－HARUNA Publisher－Victor Music Arts Co., Ltd.  Daughters of the Dawn ~ Live in Tokyo 2019 〖CD, DVD, Blu-ray〗 Now on sale 〖Track list〗 01. Clockwork Immortality (Intro) 02. Addicted 03. Bravehearted 04. The Crusade 05. Pledge Of The Saviour 06. Rising 07. Scream For Me 08. Break The Wall 09. Shadowmaker 10. Above The Black Sea 11. Empty Daydream 13. Journey To The Otherside 14. Edge Of The World 15. We The United 16. Epilogue 17. Do n’t Bite The Dust 18. Under The Red Sky 19. The Everlasting (Outro) (Commentary sub-audio recording by members) ＋ Bonus video \"Introduction of equipment\"  DAUGHTERS OF THE DAWN - LIVE IN TOKYO 2019 (BLU-RAY / DVD / 2CD) SHOPPING FROM OVERSEAS:-> https://lovebites.booth.pm/items/2366256  〖Purchased from Amazon Japan〗-> https://www.amazon.co.jp/-/en/LOVEBIT...  LOVEBITES SHOPPING FROM OVERSEAS: https://lovebites.booth.pm/  asami…Vocal midori…Guitar miyako…Guitar / Keyboard (Piano) miho…Base, Leader haruna…Drums  Official site-> https://lovebites.jp/  YouTube Channel-> https://www.youtube.com/channel/UC90wxogt_sQrP0Os0HT-xuw  Twitter-> https://twitter.com/lovebites_jp  ＃LOVEBITES    ＃BRAVEHEARTED    ＃HARUNA	141830	2197	175	NA			WAHR	WAHR	511	08.02.25 05:21	youtube#video	ZQcjWc16gA46rJnhledF16DiUJk	okkuオック	none	07.02.25 21:37	22	ja	hd	FALSCH	rectangular	FALSCH	2d	PT6M26S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Music_of_Asia https://en.wikipedia.org/wiki/Pop_music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1620362040:R>] https://youtu.be/3XRq1YR_A0w [06:25 ja hd(2d) People & Blogs] okkuオック LOVEBITES〖BRAVEHEARTED〗 Daughters of the Dawn~Live in Tokyo 2019	FALSCH		＃LOVEBITES ＃HARUNA ＃BRAVEHEARTED",
"8	49g7JfnHY-o	19.06.21 11:33	SLX6nA3LRuguwhHZOQO1ZQ	LOVEBITES	Song	LOVEBITES【PLEDGE OF THE SAVIOUR】LIVE AT MYNAVI BLITZ AKASAKA,TOKYO,JAPAN－JANUARY/27/2019	00:04:48	【PLEDGE OF THE SAVIOUR】 Lyrics－ASAMI music－MIYAKO Publisher－Victor Music Arts Co., Ltd.  Now on sale Daughters of the Dawn ~ Live in Tokyo 2019 [CD, DVD, Blu-ray] 〖Track list〗 01. Clockwork Immortality (Intro) 02. Addicted 03. Bravehearted 04. The Crusade 05. Pledge Of The Saviour 06. Rising 07. Scream For Me 08. Break The Wall 09. Shadowmaker 10. Above The Black Sea 11. Empty Daydream 12. M.D.O. 13. Journey To The Otherside 14. Edge Of The World 15. We The United 16. Epilogue 17. Do n’t Bite The Dust 18. Under The Red Sky 19. The Everlasting (Outro) (Commentary sub-audio recording by members) ＋ Bonus video 〖Introduction of equipment〗  DAUGHTERS OF THE DAWN - LIVE IN TOKYO 2019 (BLU-RAY / DVD / 2CD) SHOPPING FROM OVERSEAS:-> https://lovebites.booth.pm/items/2366256  〖Purchased from Amazon Japan〗-> https://www.amazon.co.jp/-/en/LOVEBIT...  LOVEBITES SHOPPING FROM OVERSEAS: https://lovebites.booth.pm/  asami…Vocal midori…Guitar miyako…Guitar / Keyboard (Piano) miho…Base, Leader haruna…Drums  Official site-> https://lovebites.jp/  YouTube Channel-> https://www.youtube.com/channel/UC90wxogt_sQrP0Os0HT-xuw  Twitter-> https://twitter.com/lovebites_jp  ＃LOVEBITES ＃PledgeOfTheSavio ＃Live	9865	250	18	NA			WAHR	WAHR	505	08.02.25 05:21	youtube#video	KmyBLFUmRKKtu3bGAf7OOYIoT4o	okkuオック	none	07.02.25 21:37	22	ja	hd	FALSCH	rectangular	FALSCH	2d	PT4M49S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1624095213:R>] https://youtu.be/49g7JfnHY-o [04:48 ja hd(2d) People & Blogs] okkuオック LOVEBITES【PLEDGE OF THE SAVIOUR】LIVE AT MYNAVI BLITZ AKASAKA,TOKYO,JAPAN－JANUARY/27/2019	FALSCH		＃LOVEBITES ＃Live",
"9	4ImGA7cPyrs	21.02.23 16:00	90wxogt_sQrP0Os0HT-xuw	LOVEBITES	OMV	LOVEBITES / Stand And Deliver (Shoot ´em Down) [OFFICIAL MUSIC VIDEO]	00:05:19	Here is the video clip to \"Stand And Deliver (Shoo...	1543412	24471	2349	Official			WAHR	WAHR	46	12.07.25 19:50	youtube#video	rmEC73rbnbHIpe3ivivh7fc1gm8	LOVEBITES	none	12.07.25 18:18	10	ja	hd	WAHR	rectangular	WAHR	2d	PT5M20S	https://en.wikipedia.org/wiki/Music https://en.wikipedia.org/wiki/Rock_music	youtube	WAHR	WAHR	FALSCH	public	processed					[<t:1676991602:R>] https://youtu.be/4ImGA7cPyrs [05:19 CC ja hd(2d) Music] LOVEBITES LOVEBITES / Stand And Deliver (Shoot ´em Down) [OFFICIAL MUSIC VIDEO]	FALSCH		#lovebites"
	};

bool test_param::insertrun(PGconnection &pg, const char *command, const int arg) const
{
	DBrecordset *rs = pg.query(command);

	if (rs != nullptr) {
		PGstatement deletestmt(&pg, "DELETE FROM \"Video\" WHERE \"ID\"=$1", arg);
		PGparameter param(0, "_66Ug5c3aJo");  // direct insert, defines types!

		pg.getanswer(&deletestmt, param);

		// Insert the first line...
		rs->getanswer(param);

		// Insert the rest...
		for (int i = 1; i < 10; ++i) {
			const char *parsestring = data[i];

			param.parse(parsestring, '	');  // from string, uses types!

			pg.getanswer(&deletestmt, param);

			rs->getanswer(param);
		}

		return true;
	} else {
		return false;
	}
}

bool test_param::selectemptyrun(PGconnection &pg, const char *command, [[maybe_unused]] const int arg) const
{
	DBrecordset *rs = pg.query(command);

	if (rs != nullptr) {
		const int nFields = rs->getNFields();
		const DBparameterType *types = rs->getFieldTypes();

		std::cout << nFields << " columns..." << std::endl;

		for (int i = 0; i < nFields; ++i) {
			int pos = (i + 1);
			std::string name = rs->getFieldName(pos);
			std::cout << pos << '\t' << types[i] << '\t' << name << std::endl;
		}

		return true;
	} else {
		return false;
	}
}

bool test_param::paramrun([[maybe_unused]] PGconnection &pg, [[maybe_unused]] const char *command, const int arg) const
{
	const float argx = (arg + 0.5f);
	const char *parsestring = "50.5	50.5	50.5	50.5	50.5	test";
	PGparameter param(static_cast<int>(argx),
						static_cast<short int>(argx),
						static_cast<long int>(argx),
						static_cast<float>(argx),
						static_cast<double>(argx),
						"arg");
	std::tm tm0{};
	std::tm tm1{ 0, 0, 0, 19, 4, 126, 0, 0, -1, 3600, "CET" };
	std::tm tm;
	std::time_t now = std::time(nullptr);
	PGparameter param2(&tm0, &tm1, &tm, &arg, &now, reinterpret_cast<const void *>(&arg));
	PGparameter param3(static_cast<__int128_t>(argx),
						static_cast<long double>(argx),
						static_cast<signed char>(argx),
						static_cast<unsigned char>(argx),
						static_cast<char16_t>(argx),
						static_cast<char32_t>(argx),
						static_cast<bool>(argx),
						true,
						false);

	param.parse(parsestring, '	');

	std::cout << "ANSWER: " << param.to_string() << std::endl << param2.to_string() << std::endl << param3.to_string() << std::endl;

	return true;
}

bool test_param::run(void)
{
	const char		*commandselect = "SELECT * FROM \"Video\" WHERE FALSE;";
	const char		*commandinsert = "INSERT INTO \"Video\" (\"ID\", \"VID\") VALUES ($1, $2);";
	const int		 arg = 50;
	const char		*conninfo = "";
	PGconnection	 pg;
	bool			 ret = false;

	pg.connect(conninfo, true, PQERRORS_VERBOSE);

   	if (pg.checkconnect()) {
		PQsetErrorVerbosity(pg.getPGconn(), PQERRORS_VERBOSE);

		std::cout << "ANSWER: " << pg.getanswer("SELECT version();") << std::endl;
		std::cout << "ANSWER: " << pg.getanswer("SET search_path TO loges;") << std::endl;

		ret = paramrun(pg, nullptr, arg);

		ret = ret && selectemptyrun(pg, commandselect, arg);

		ret = ret && insertrun(pg, commandinsert, arg);
	}

	return ret;
}

