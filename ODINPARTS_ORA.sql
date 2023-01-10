CREATE TABLE "ODINPARTS"."INDIRIZZI" (
    "VIA" VARCHAR(30) NOT NULL ENABLE,
    "CIVICO" INT NOT NULL ENABLE,
    "CITTÀ" VARCHAR(20) NOT NULL ENABLE,
    "CAP" INT NOT NULL ENABLE,
    "STATO" VARCHAR(20) NOT NULL ENABLE,
    "USERNAME" VARCHAR(20) NOT NULL ENABLE,
    PRIMARY KEY ("USERNAME","VIA","CIVICO","CITTÀ","CAP","STATO"),
        CONSTRAINT USRN_FK
		FOREIGN KEY(USERNAME) REFERENCES UTENTE(USERNAME)
   );

CREATE TABLE "ODINPARTS"."RICAMBIO" (
    "ID_RICAMBIO" VARCHAR(20) PRIMARY KEY NOT NULL ENABLE,
    "NOME" VARCHAR(60) NOT NULL ENABLE,
    "PRODUTTORE" VARCHAR(20) NOT NULL ENABLE,
    "CATEGORIA" VARCHAR(60) NOT NULL ENABLE,
    "N_SERIALE" VARCHAR(20) NOT NULL ENABLE,
    "GIACENZA" INT NOT NULL ENABLE,
    "DESCRIZIONE" VARCHAR(700) NOT NULL ENABLE,
    "FASCIA" VARCHAR(12) NOT NULL ENABLE,
    "DIMENSIONI" VARCHAR(20) NOT NULL ENABLE,
    "MATERIALE" VARCHAR(30) NOT NULL ENABLE,
    "PREZZO" DECIMAL(8,2) NOT NULL ENABLE,
    "URL_FOTO" VARCHAR(2083) NOT NULL ENABLE
);

CREATE TABLE "ODINPARTS"."MODELLO_AUTO"(
	"AUTO_ID" VARCHAR(30) PRIMARY KEY NOT NULL,
    "CASA_PROD" VARCHAR(15) NOT NULL,
    "ANNO" VARCHAR(10) NOT NULL,
    "VERSIONE" VARCHAR(8),
    "CARBURANTE" VARCHAR(10) NOT NULL,
    "MOTORE" VARCHAR(3) NOT NULL,
    "CILINDRATA" SMALLINT NOT NULL
 );

/***************************************/
/********SEQUENZA PER IMPLEMENTARE******/
/****AUTO_INCREMENT IN******************/
/******************CHIAVE PRIMARIA******/
/***************************************/
CREATE SEQUENCE ODINPARTS.INC_SEQ
START WITH 1
INCREMENT BY 1;
/***************************************/

CREATE TABLE "ODINPARTS"."ORDINE"(
	"ID_ORDINE" INT DEFAULT "ODINPARTS".INC_SEQ.NEXTVAL PRIMARY KEY NOT NULL,
    "ACQUIRENTE" VARCHAR(20) NOT NULL,
    "CARTA" CHAR(16) NOT NULL,
    "DATA_PAY" TIMESTAMP NOT NULL,
    "TOTALE" DECIMAL(6,2) NOT NULL,
    CONSTRAINT ORDER_FK
    	FOREIGN KEY("ACQUIRENTE") REFERENCES UTENTE("USERNAME")
 );

CREATE TABLE "ODINPARTS"."RECENSIONE"(
	"COMMENTO" VARCHAR(1000) NOT NULL,
    "DATA" TIMESTAMP NOT NULL,
    "VOTAZIONE" INT NOT NULL,
    "USERNAME" VARCHAR(20) NOT NULL,
    "ID_RICAMBIO" VARCHAR(20) NOT NULL,
    CHECK ("VOTAZIONE" BETWEEN 0 AND 10),
    PRIMARY KEY("USERNAME","ID_RICAMBIO"),
    CONSTRAINT REV_FK_0
    	FOREIGN KEY ("USERNAME") REFERENCES UTENTE("USERNAME"),		
	CONSTRAINT REV_FK_1
    	FOREIGN KEY ("ID_RICAMBIO") REFERENCES RICAMBIO("ID_RICAMBIO")
			ON DELETE CASCADE
 );
 
CREATE TABLE "ODINPARTS"."CARTA_C"(
	"NUMERO" CHAR(16) PRIMARY KEY NOT NULL,
    "NOME_TITOLARE" VARCHAR(20) NOT NULL,
    "COGNOME_TITOLARE" VARCHAR(20) NOT NULL,
    "CVV" CHAR(3) NOT NULL,
    "SCADENZA" CHAR(6) NOT NULL,
    "USERNAME" VARCHAR(20) NOT NULL,
    CONSTRAINT CARD_C
		FOREIGN KEY("USERNAME") REFERENCES UTENTE("USERNAME")
			ON DELETE CASCADE			/*SE L'UTENTE È CANCELLATO, LE INFORMAZIONI DELLA CARTA NON SONO RILEVANTI*/
 );
 
CREATE TABLE "ODINPARTS"."SPEDIZIONE"(
	"ID_SPED" VARCHAR(15) PRIMARY KEY NOT NULL,
    "CORRIERE" VARCHAR(20) NOT NULL,
    "DATA_INVIO" DATE NOT NULL,
    "DATA_CONSEGNA" DATE NOT NULL,
    "ID_ORDINE" INT NOT NULL,
    CONSTRAINT SHPMT_FK
		FOREIGN KEY("ID_ORDINE") REFERENCES ORDINE("ID_ORDINE")
			ON DELETE CASCADE			/*SE L'ORDINE È CANCELLATO, FREGA NIENTE DELLA SPEDIZIONE*/	
);

CREATE TABLE "ODINPARTS"."LISTA_COMP"(
	"AUTO_ID"	VARCHAR(30) NOT NULL,
    "ID_RICAMBIO" VARCHAR(20) NOT NULL,
    PRIMARY KEY("AUTO_ID","ID_RICAMBIO"),
	CONSTRAINT LIST_C_0
		FOREIGN KEY("AUTO_ID") REFERENCES MODELLO_AUTO("AUTO_ID")
			ON DELETE CASCADE,
	CONSTRAINT LIST_C_1
		FOREIGN KEY("ID_RICAMBIO") REFERENCES RICAMBIO("ID_RICAMBIO")
			ON DELETE CASCADE
);

CREATE TABLE "ODINPARTS"."ACQUISTATI"(
	"ACQUIRENTE" VARCHAR(20) NOT NULL,
    "ID_RICAMBIO" VARCHAR(20) NOT NULL,
    "QTA" INT NOT NULL,
    PRIMARY KEY("ACQUIRENTE","ID_RICAMBIO"),
    CONSTRAINT ACQUISTI_C_0
		FOREIGN KEY("ACQUIRENTE") REFERENCES UTENTE("USERNAME"),
	CONSTRAINT ACQUISTI_C_1
		FOREIGN KEY("ID_RICAMBIO") REFERENCES RICAMBIO("ID_RICAMBIO")
);

CREATE TABLE "ODINPARTS"."VENDUTI"(
	"VENDITORE"VARCHAR(20) NOT NULL,
    "ID_RICAMBIO" VARCHAR(20) NOT NULL,
    "QTA" INT NOT NULL,
    PRIMARY KEY("VENDITORE","ID_RICAMBIO"),
	CONSTRAINT VENDUTI_C_0
		FOREIGN KEY("VENDITORE") REFERENCES UTENTE("USERNAME"),
	CONSTRAINT VENDUTI_C_1	
		FOREIGN KEY("ID_RICAMBIO") REFERENCES RICAMBIO("ID_RICAMBIO")
);

CREATE TABLE "ODINPARTS"."VENDITE"(
	"VENDITORE"VARCHAR(20) NOT NULL,
    "ID_ORDINE" INT NOT NULL,
    PRIMARY KEY("VENDITORE","ID_ORDINE"),
    CONSTRAINT VENDITE_C_0
		FOREIGN KEY("VENDITORE") REFERENCES UTENTE("USERNAME"),
	CONSTRAINT VENDITE_C_1
		FOREIGN KEY("ID_ORDINE") REFERENCES ORDINE("ID_ORDINE")
);

CREATE TABLE "ODINPARTS"."VOTI"(
	"ACQUIRENTE" VARCHAR(20) NOT NULL,
	"VENDITORE" VARCHAR(20) NOT NULL,
	"FEEDBACK" VARCHAR(80) NOT NULL,
    PRIMARY KEY("ACQUIRENTE","VENDITORE"),
    CONSTRAINT FEEDBACK_C_0
		FOREIGN KEY("ACQUIRENTE") REFERENCES UTENTE("USERNAME"),
	CONSTRAINT FEEDBACK_C_1
		FOREIGN KEY("VENDITORE") REFERENCES UTENTE("USERNAME")
);

CREATE TABLE "ODINPARTS"."RIC_ORD"(
	"ID_ORDINE" INT NOT NULL,
    "ID_RICAMBIO" VARCHAR(20) NOT NULL,
    "QTA" INT NOT NULL,
    PRIMARY KEY("ID_ORDINE","ID_RICAMBIO"),
    CONSTRAINT RIC_ORD_0
		FOREIGN KEY("ID_ORDINE") REFERENCES ORDINE("ID_ORDINE"),
	CONSTRAINT RIC_ORD_1
		FOREIGN KEY("ID_RICAMBIO") REFERENCES RICAMBIO("ID_RICAMBIO")
);

INSERT INTO UTENTE VALUES ('abdiel.koepp','Enrico','Lindgren','llockman@example.net','a26c4bbbfbabb03ba220','0.00','0.00','0.00','0.00','0','2008-09-07');
INSERT INTO UTENTE VALUES ('abigail.macejkovic','Ressie','Hermann','chelsey.heidenreich@example.co','90c4e8e758e57342bcd1','7.45','5.25','8.00',null,'4','1984-08-17');
INSERT INTO UTENTE VALUES ('aliza57','Darius','Mills','hickle.magnus@example.net','be0f4d2519c472badefc','6.45','7.00','5.00',null,'5','1994-12-03');
INSERT INTO UTENTE VALUES ('angel.medhurst','Mack','Emmerich','mann.walter@example.net','889acefb62a51feeee43','0.00','0.00','0.00','0.00','0','1973-11-13');
INSERT INTO UTENTE VALUES ('benton34','Katrine','Mante','gheathcote@example.net','005e1f60979d490a5c7d','0.00','0.00','0.00','0.00','0','1997-07-14');
INSERT INTO UTENTE VALUES ('bernardo.mohr','Chadrick','Dietrich','maurice12@example.net','6a0a0807d4b0be90f2b7','5.82','7.00','6.16',null,'3','1971-07-06');
INSERT INTO UTENTE VALUES ('bernhard76','Amie','Goodwin','tavares62@example.net','7af5519fe3b6ea986ae1','9.00','9.55','9.00',null,'4','1987-07-12');
INSERT INTO UTENTE VALUES ('bgrady','Preston','Goyette','gino.miller@example.com','d21c913f983be0ce7a7b','0.00','0.00','0.00','0.00','0','1977-09-05');
INSERT INTO UTENTE VALUES ('bherman','Ernestina','Hermann','justice.gleason@example.net','385dad18dfa3f3344f00','8.58','7.00','7.00',null,'3','1983-06-13');
INSERT INTO UTENTE VALUES ('bmedhurst','Karson','Keeling','titus.nader@example.org','a9aaa9c6860062c5e86a','7.94','7.00','7.00',null,'1','2002-11-01');
INSERT INTO UTENTE VALUES ('braun.annamarie','Delphine','Graham','jarret.terry@example.com','531ba754f566d337bab7','0.00','0.00','0.00','0.00','0','2013-06-18');
INSERT INTO UTENTE VALUES ('carey.wehner','Keeley','Carroll','fisher.jessy@example.net','bf9211c102ac75c2ed04','10.00','10.00','9.50',null,'9','1981-10-05');
INSERT INTO UTENTE VALUES ('cgusikowski','Andrew','Ruecker','abraham29@example.com','4a04113f3186576ff221','9.33','8.00','6.22',null,'6','1972-07-30');
INSERT INTO UTENTE VALUES ('chaz69','Anjali','McLaughlin','lon.prohaska@example.org','d9eae1ac64f924a5c0dc','7.18','6.00','6.00',null,'3','2013-11-10');
INSERT INTO UTENTE VALUES ('collins.napoleon','Madilyn','Weber','rocio.grant@example.com','b982a41534e4f5d37cb8','4.00','4.00','5.58',null,'4','2015-04-02');
INSERT INTO UTENTE VALUES ('connor.baumbach','Lucius','Schuster','bartoletti.tina@example.net','78410bf96f18ff719ca3','5.51','6.00','9.93',null,'8','1999-02-24');
INSERT INTO UTENTE VALUES ('domenico21','Herminia','Bradtke','qmarks@example.com','a665ec523cbc60ad6712','6.00','7.79','6.00',null,'3','1991-05-02');
INSERT INTO UTENTE VALUES ('doyle.ariel','Astrid','Zemlak','lang.jazmin@example.com','8fae4987902c269f3623','5.70','6.00','5.89','5.86','2','1979-11-09');
INSERT INTO UTENTE VALUES ('dsauer','Arnoldo','Zieme','alexie52@example.org','d160865341239935e485','7.13','6.15','6.26','6.51','7','1986-12-11');
INSERT INTO UTENTE VALUES ('echristiansen','Ambrose','Nikolaus','laila.bogisich@example.net','93fdf3d3afc015c15a44','0.00','0.00','0.00','0.00','0','1995-05-31');
INSERT INTO UTENTE VALUES ('elisa13','Franco','Wintheiser','edyth01@example.net','8aa8f62feb66f46cd834','8.00','7.43','9.00',null,'8','2016-11-22');
INSERT INTO UTENTE VALUES ('emmie53','Laurianne','Cronin','vhartmann@example.org','bf04f693313cbcf165f0','8.00','7.05','9.26',null,'4','2007-10-15');
INSERT INTO UTENTE VALUES ('enos.feil','Jordi','Goldner','maybelle49@example.org','2cf31bdd4f86d28ba28e','9.86','8.00','7.75',null,'5','1978-12-25');
INSERT INTO UTENTE VALUES ('erick64','Danyka','Spinka','pouros.abagail@example.org','6d71a6955c5d92ea2809','0.00','0.00','0.00','0.00','0','2007-04-11');
INSERT INTO UTENTE VALUES ('feeney.ivah','Hermann','McCullough','ytoy@example.com','c058b67d78a22d50bf9e','9.38','9.00','9.00',null,'2','2008-06-19');
INSERT INTO UTENTE VALUES ('fkemmer','Dora','Muller','cassin.lawrence@example.net','854adf5303806ec8b558','8.00','9.22','8.00',null,'2','1979-09-22');
INSERT INTO UTENTE VALUES ('francesco.frami','Taryn','Hamill','fahey.melyssa@example.net','f4c59428bdf255eb10f4','5.59','7.00','6.91',null,'4','1996-04-10');
INSERT INTO UTENTE VALUES ('geovany.hirthe','Kaia','Watsica','armstrong.lori@example.org','f9742f1a9f082c87f740','6.00','5.44','6.00',null,'6','2007-03-06');
INSERT INTO UTENTE VALUES ('gerlach.kari','Camryn','Turcotte','lonzo31@example.com','9da942668dba46f36ccc','0.00','0.00','0.00','0.00','0','1984-07-04');
INSERT INTO UTENTE VALUES ('goodwin.gonzalo','Reagan','Lind','jay.abernathy@example.org','b5bcff31bce6d44787d2','7.43','8.00','5.89',null,'7','2017-08-05');
INSERT INTO UTENTE VALUES ('hankunding','Simone','Blanda','simonis.meagan@example.com','5488f5e8c3903c943be0','6.78','9.54','8.00',null,'5','1995-04-04');
INSERT INTO UTENTE VALUES ('hannah14','Angie','Crooks','amy59@example.net','c653ec7f8142e6ba862b','9.00','9.98','9.00',null,'7','1978-11-17');
INSERT INTO UTENTE VALUES ('heath.leuschke','Sister','Langworth','therzog@example.net','fe0a4fdb5067ab720920','0.00','0.00','0.00','0.00','0','2009-10-13');
INSERT INTO UTENTE VALUES ('hills.mariela','Jesus','Homenick','glemke@example.org','c5270a35a81b9532056c','0.00','0.00','0.00','0.00','0','1989-06-13');
INSERT INTO UTENTE VALUES ('hintz.jessie','Skyla','Schroeder','barrows.joanie@example.org','9e6bfe9ec57db8c759a0','7.00','6.59','8.00',null,'6','1982-12-02');
INSERT INTO UTENTE VALUES ('hipolito70','Thora','Braun','kyra97@example.org','ed676e35f295afaec360','9.12','9.52','9.00',null,'9','1975-01-11');
INSERT INTO UTENTE VALUES ('idietrich','Roger','Boyer','ulises22@example.com','7e6c52f2e97b10511afb','9.00','7.76','8.00',null,'9','2008-04-04');
INSERT INTO UTENTE VALUES ('ilynch','Cayla','Jast','houston.buckridge@example.net','52c21e99bd3df497c3c1','0.00','0.00','0.00',null,'0','1979-02-28');
INSERT INTO UTENTE VALUES ('jakubowski.johnpaul','Heidi','Heathcote','michaela.schmeler@example.net','6cb3d5fea746bec937c9','9.54','8.00','6.99',null,'6','1998-05-24');
INSERT INTO UTENTE VALUES ('jalyn99','Iva','Murphy','jkuhlman@example.com','1e354d7e2a301aba0122','8.00','6.75','5.25',null,'3','1996-04-10');
INSERT INTO UTENTE VALUES ('jarod58','Berneice','Hoppe','hturner@example.org','cb383899b54a51a8b0a6','6.96','7.46','8.00',null,'2','2007-04-03');
INSERT INTO UTENTE VALUES ('jmarks','Tessie','Berge','zgulgowski@example.org','af8d10552da47e9b1fc4','5.26','6.00','7.00',null,'3','2000-03-13');
INSERT INTO UTENTE VALUES ('joel.herzog','Abbey','Hirthe','elias.hoeger@example.org','17f953e5e5448f2536b8','8.00','8.29','8.00',null,'2','1978-05-22');
INSERT INTO UTENTE VALUES ('kautzer.helmer','Lemuel','Bogan','otto.keeling@example.org','734cdf809bc9d8102532','9.85','9.00','7.35',null,'7','2002-12-01');
INSERT INTO UTENTE VALUES ('kjerde','Celestine','Kunde','jeff.botsford@example.org','0138b3c5411277085428','0.00','0.00','0.00',null,'0','1982-05-02');
INSERT INTO UTENTE VALUES ('klein.stanton','Davon','Brown','ternser@example.org','ce0fdacb1994a650f739','9.05','10.00','8.59',null,'10','2001-05-24');
INSERT INTO UTENTE VALUES ('kris.stanford','Lessie','Fadel','augusta92@example.com','c60076d51f639d2196d3','0.00','0.00','0.00',null,'0','2001-06-05');
INSERT INTO UTENTE VALUES ('kuhlman.andre','Beaulah','Gleichner','zemlak.bernardo@example.org','e9fd3bd3db252f223857','0.00','0.00','0.00','0.00','0','1975-12-06');
INSERT INTO UTENTE VALUES ('kuphal.lillian','Lonny','Brekke','clare.pagac@example.org','19a36ccad79182a442a8','0.00','0.00','0.00','0.00','0','1988-11-01');
INSERT INTO UTENTE VALUES ('landen02','Jaime','Lang','qschmidt@example.org','df6566fdbc7aa5c1bb70','0.00','0.00','0.00','0.00','0','2015-04-30');
INSERT INTO UTENTE VALUES ('lauretta02','Audreanne','Barton','emmerich.kenya@example.org','49aeec22e6bdfcce1057','0.00','0.00','0.00',null,'0','1985-11-08');
INSERT INTO UTENTE VALUES ('lbecker','Tyreek','Mayer','harvey.cara@example.org','db0af9f5e507b78fc5c9','0.00','0.00','0.00',null,'0','1998-02-26');
INSERT INTO UTENTE VALUES ('leilani.kerluke','Stephen','Mayer','carey.schaefer@example.com','09ba589ec073da36732b','0.00','0.00','0.00',null,'0','1992-08-24');
INSERT INTO UTENTE VALUES ('leland86','Milford','Wisoky','lockman.shane@example.org','89481f064b2373cc8280','5.81','8.69','5.36','6.62','4','2015-05-26');
INSERT INTO UTENTE VALUES ('leta53','Laury','Cronin','karli.howell@example.com','cf540721fcf9346bcfa6','0.00','0.00','0.00','0.00','0','2000-03-04');
INSERT INTO UTENTE VALUES ('lturner','Arlie','Herman','kassulke.eloy@example.net','c03a11f575e9454da84c','9.14','9.16','8.83','9.04','8','1981-10-17');
INSERT INTO UTENTE VALUES ('luna.pfeffer','Nella','O\'Conner','jones.ruthie@example.com','febfc7551886a4536727','0.00','0.00','0.00','0.00','0','1996-03-11');
INSERT INTO UTENTE VALUES ('luna70','Gerard','Murazik','jones.joesph@example.net','4993bd63010f2fd4516e','9.09','6.00','8.21',null,'80','2013-09-29');
INSERT INTO UTENTE VALUES ('margaret.lubowitz','Mertie','Bogan','marianne.gibson@example.com','4efb1393b26b08a1f018','0.00','0.00','0.00','0.00','0','1991-01-27');
INSERT INTO UTENTE VALUES ('maritza16','Mathilde','Conn','pearl04@example.net','26732fc2fc4dbaeda921','8.83','10.00','9.08',null,'60','2008-02-01');
INSERT INTO UTENTE VALUES ('marjolaine.schinner','Liza','Sawayn','jack.borer@example.org','c6b2e4b52d529c2264db','7.66','8.53','9.00',null,'9','2017-01-04');
INSERT INTO UTENTE VALUES ('marlon09','Citlalli','McLaughlin','amante@example.org','2e3871451b1e1e0683db','8.00','8.62','9.45',null,'20','1985-09-07');
INSERT INTO UTENTE VALUES ('martin.armstrong','Katrina','Marvin','emetz@example.net','db21513a9524264925fd','0.00','0.00','0.00','0.00','0','1985-11-16');
INSERT INTO UTENTE VALUES ('marvin.mireille','Clemmie','Boyle','ben73@example.net','b92aa7c26fd595a73604','9.00','8.38','9.47',null,'800','2016-04-29');
INSERT INTO UTENTE VALUES ('matteo12','Oswald','Tremblay','schroeder.natalia@example.net','c05fa85aa36b9a849df1','5.64','8.00','8.67',null,'400','1976-10-10');
INSERT INTO UTENTE VALUES ('maurice52','Lucinda','Auer','hoeger.neha@example.com','3e7338d19ae219c9e59d','8.08','9.34','9.00',null,'521','1990-08-29');
INSERT INTO UTENTE VALUES ('mccullough.amos','Marc','Boyle','katarina.harris@example.com','2a0d15883ebf24bf26b1','6.00','5.43','6.86',null,'72','1998-08-31');
INSERT INTO UTENTE VALUES ('mckenzie.aiden','Anibal','Ernser','waelchi.frederique@example.com','ce1193332d9b5affe4c0','0.00','0.00','0.00','0.00','0','2008-10-20');
INSERT INTO UTENTE VALUES ('mcummerata','Lessie','Williamson','vdouglas@example.com','56600dadc39e44f08d01','0.00','0.00','0.00','0.00','0','2019-08-14');
INSERT INTO UTENTE VALUES ('mitchel.shields','Lyda','Hansen','bhand@example.org','de816db21452f9142277','8.00','9.04','8.69',null,'220','2011-07-01');
INSERT INTO UTENTE VALUES ('nelson.vonrueden','Sven','Krajcik','jast.treva@example.org','5b169d0ec9d3f325d73c','8.53','9.75','9.12','9.13','5002','1971-10-06');
INSERT INTO UTENTE VALUES ('noberbrunner','Matilda','Willms','garrison39@example.net','599192b098d3ccc2d386','7.99','6.86','8.10','7.65','3283','1984-07-11');
INSERT INTO UTENTE VALUES ('o\'connell.burdette','Lexi','Conroy','arianna.kemmer@example.net','6c3d859b2ce733ea1fe3','9.00','7.19','8.00',null,'527','1995-02-02');
INSERT INTO UTENTE VALUES ('qhudson','Lonnie','Windler','ottilie14@example.org','532e2be32590737965c4','0.00','0.00','0.00',null,'0','1976-12-23');
INSERT INTO UTENTE VALUES ('reta.crist','Haven','Blanda','becker.stan@example.org','aee44c16c25779b9636e','9.00','9.00','9.89',null,'4000','2010-01-09');
INSERT INTO UTENTE VALUES ('rhoeger','Devante','Lind','kolby04@example.com','6c4f9221a0c1e2ae2bcb','0.00','0.00','0.00','0.00','0','2010-06-03');
INSERT INTO UTENTE VALUES ('rosendo.hegmann','Elisa','Abshire','wgoyette@example.com','175bc3f048877492f3a9','7.00','5.38','7.68','4.35','212','1980-03-29');
INSERT INTO UTENTE VALUES ('ryley.bartell','Easter','McClure','pbernier@example.com','236082f5b153a91326fb','0.00','0.00','0.00','0.00','0','2000-10-09');
INSERT INTO UTENTE VALUES ('sadye.barrows','Germaine','Shields','reichel.teagan@example.org','ea07d69feae5cfb70802','5.09','6.36','7.00',null,'8','1995-01-04');
INSERT INTO UTENTE VALUES ('saige.swaniawski','Kieran','Gutkowski','brandon35@example.com','3ef343498d40d1f7d6bd','0.00','0.00','0.00','0.00','0','1987-02-26');
INSERT INTO UTENTE VALUES ('schowalter.nyah','Jamil','Schulist','suzanne52@example.com','8828de452adff2c1e20e','9.00','7.79','9.20',null,'1001','2010-10-25');
INSERT INTO UTENTE VALUES ('sofia83','Thurman','Kautzer','camden33@example.net','5a46bc17234b05d97af5','0.00','0.00','0.00','0.00','0','2012-06-11');
INSERT INTO UTENTE VALUES ('sophia89','Arjun','Muller','ceasar27@example.net','598d57eed0d6fc3b6564','9.85','6.30','6.12','7.42','321','1973-10-04');
INSERT INTO UTENTE VALUES ('stella21','Cassidy','Bernhard','leland.kohler@example.org','1a94f2a8f56259ec19c5','6.98','5.65','9.00',null,'233','1990-09-22');
INSERT INTO UTENTE VALUES ('swaniawski.danielle','Beatrice','Baumbach','goldner.maryam@example.com','4078091973a7328c1c6c','0.00','0.00','0.00',null,'0','1998-09-27');
INSERT INTO UTENTE VALUES ('thora.hayes','Dallin','Kiehn','bartoletti.ayana@example.org','97172a7895077a3703a0','0.00','0.00','0.00','0.00','0','1986-05-25');
INSERT INTO UTENTE VALUES ('tillman.spencer','Jalon','Halvorson','queenie.kub@example.com','01fc1f93014865b0eacd','8.00','5.70','9.48',null,'2400','2012-05-07');
INSERT INTO UTENTE VALUES ('tmohr','Georgianna','Walter','tremblay.taurean@example.net','00159b032b988ba7d438','9.16','8.64','9.00',null,'421','2005-12-27');
INSERT INTO UTENTE VALUES ('towne.rashawn','Titus','Pfeffer','alva.feil@example.org','39154868756a5f9f4901','0.00','0.00','0.00','0.00','0','2016-01-20');
INSERT INTO UTENTE VALUES ('uhackett','Derick','Green','eichmann.zane@example.com','f9718504cc078e383692','8.09','5.00','8.21',null,'7','1997-07-27');
INSERT INTO UTENTE VALUES ('vreinger','Randy','Lakin','dejon.lebsack@example.org','d43ae17a9d6a931de474','8.00','5.10','6.90',null,'8','1995-03-18');
INSERT INTO UTENTE VALUES ('wbartoletti','Alvera','Kirlin','vkuhic@example.net','cc8d91540c2975384968','5.27','8.74','5.00',null,'5','1991-07-02');
INSERT INTO UTENTE VALUES ('weber.gudrun','Carlotta','Klocko','cfunk@example.net','3d371aa2db54e12be454','0.00','0.00','0.00',null,'0','1992-11-25');
INSERT INTO UTENTE VALUES ('wintheiser.zula','Estelle','Kirlin','jany26@example.net','d5df960526c545acf9c6','0.00','0.00','0.00','0.00','0','1977-11-10');
INSERT INTO UTENTE VALUES ('xcasper','Brooks','Cremin','euna53@example.net','3c23dd195c4090683b2d','6.84','6.91','8.59','7.45','9','1993-01-27');
INSERT INTO UTENTE VALUES ('xkohler','Violet','Jacobi','hettinger.mac@example.org','4ae69c0eabfae8a3315c','8.53','9.48','8.81','8.94','8','1988-07-26');
INSERT INTO UTENTE VALUES ('ycruickshank','Madie','Mayert','xroberts@example.com','f3408467f5c3fc32ea97','6.97','9.68','7.00',null,'100','1985-10-27');
INSERT INTO UTENTE VALUES ('yrempel','Wellington','Willms','borer.effie@example.org','1729cde4022f21752d7b','0.00','0.00','0.00',null,'0','2013-07-29');
INSERT INTO UTENTE VALUES ('zdonnelly','Makenzie','Cronin','icole@example.org','4b80b98cdc7a2933760e','0.00','0.00','0.00',null,'0','1998-02-09');
INSERT INTO UTENTE VALUES ('zoe.pouros','Erich','Sawayn','bgoodwin@example.com','b7d690477ad1d5ebdec8','0.00','0.00','0.00',null,'0','2004-03-17'); 



