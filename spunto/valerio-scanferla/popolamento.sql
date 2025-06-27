DROP TABLE IF EXISTS "Prescrizione" CASCADE;
DROP TABLE IF EXISTS "Referto" CASCADE;
DROP TABLE IF EXISTS "Volontario" CASCADE;
DROP TABLE IF EXISTS "Lotto" CASCADE;
DROP TABLE IF EXISTS "Medicina" CASCADE;
DROP TABLE IF EXISTS "Dottore" CASCADE;
DROP TABLE IF EXISTS "Reparto" CASCADE;
DROP TABLE IF EXISTS "Malattia" CASCADE;
DROP TABLE IF EXISTS "CartellaClinica" CASCADE;
DROP TABLE IF EXISTS "CaratteristicheFisicoSanitarie" CASCADE;
DROP TABLE IF EXISTS "Paziente" CASCADE;
DROP TABLE IF EXISTS "MedicoDiBase" CASCADE;


CREATE TABLE "MedicoDiBase" (
  "CF" VARCHAR(16) PRIMARY KEY NOT NULL,
  "Nome" VARCHAR(50) NOT NULL,
  "Cognome" VARCHAR(50) NOT NULL,
  "Indirizzo" VARCHAR(100) NOT NULL,
  "Telefono" VARCHAR(15) NOT NULL
);

CREATE TABLE "Paziente" (
  "CF" VARCHAR(16) PRIMARY KEY NOT NULL,
  "Nome" VARCHAR(50) NOT NULL,
  "Cognome" VARCHAR(50) NOT NULL,
  "DataDiNascita" DATE NOT NULL,
  "Indirizzo" VARCHAR(100) NOT NULL,
  "Email" VARCHAR(100) NOT NULL,
  "Telefono" VARCHAR(15) NOT NULL,
  "MedicoDiBase" VARCHAR(16) NOT NULL
);

CREATE TABLE "CaratteristicheFisicoSanitarie" (
  "Paziente" VARCHAR(16) PRIMARY KEY NOT NULL,
  "Altezza" VARCHAR(5) NOT NULL,
  "Peso" NUMERIC(5,2) NOT NULL,
  "Sesso" VARCHAR(10) NOT NULL,
  "GruppoSanguigno" VARCHAR(3) NOT NULL,
  "RH" VARCHAR(1) NOT NULL
);

CREATE TABLE "CartellaClinica" (
  "CodiceCartella" VARCHAR(10) PRIMARY KEY NOT NULL,
  "Data" DATE NOT NULL,
  "PressioneMax" NUMERIC(3,0) NOT NULL,
  "PressioneMin" NUMERIC(3,0) NOT NULL,
  "Frequenza" NUMERIC(3,0) NOT NULL,
  "Paziente" VARCHAR(16) NOT NULL
);

CREATE TABLE "Malattia" (
  "NomeMalattia" VARCHAR(50) PRIMARY KEY NOT NULL,
  "Contagiosa" VARCHAR(2) NOT NULL
);

CREATE TABLE "Reparto" (
  "CodiceReparto" VARCHAR(5) PRIMARY KEY NOT NULL,
  "Tipo" VARCHAR(50) NOT NULL,
  "Telefono" VARCHAR(15) NOT NULL
);

CREATE TABLE "Dottore" (
  "CF" VARCHAR(16) PRIMARY KEY NOT NULL,
  "Nome" VARCHAR(50) NOT NULL,
  "Cognome" VARCHAR(50) NOT NULL,
  "Indirizzo" VARCHAR(100) NOT NULL,
  "Telefono" VARCHAR(15) NOT NULL,
  "Reparto" VARCHAR(5) NOT NULL
);

CREATE TABLE "Medicina" (
  "CodiceMedicina" VARCHAR(10) PRIMARY KEY NOT NULL,
  "Nome" VARCHAR(50) NOT NULL,
  "PrezzoSingolo" NUMERIC(5,2) NOT NULL,
  "Forma" VARCHAR(50) NOT NULL,
  "Produttore" VARCHAR(50) NOT NULL
);

CREATE TABLE "Lotto" (
  "Medicina" VARCHAR(10) NOT NULL,
  "DataOrdine" DATE NOT NULL,
  "Quantita" NUMERIC(5,0) NOT NULL,
  PRIMARY KEY ("Medicina", "DataOrdine")
);

CREATE TABLE "Volontario" (
  "CF" VARCHAR(16) PRIMARY KEY NOT NULL,
  "Nome" VARCHAR(50) NOT NULL,
  "Cognome" VARCHAR(50) NOT NULL,
  "Indirizzo" VARCHAR(100) NOT NULL,
  "Telefono" VARCHAR(15) NOT NULL,
  "Reparto" VARCHAR(5) NOT NULL
);

CREATE TABLE "Referto" (
  "NumeroReferto" NUMERIC(10,0) PRIMARY KEY NOT NULL,
  "Note" VARCHAR(500) NOT NULL,
  "Data" DATE NOT NULL,
  "Ora" TIME NOT NULL,
  "Paziente" VARCHAR(16) NOT NULL,
  "Dottore" VARCHAR(16) NOT NULL,
  "Malattia" VARCHAR(50)
);

CREATE TABLE "Prescrizione" (
  "Referto" NUMERIC(10,0) NOT NULL,
  "Medicina" VARCHAR(10) NOT NULL,
  PRIMARY KEY ("Referto", "Medicina")
);

-- Aggiungi i vincoli di chiave esterna dopo aver creato tutte le tabelle
ALTER TABLE "Paziente" ADD FOREIGN KEY ("MedicoDiBase") REFERENCES "MedicoDiBase" ("CF");
ALTER TABLE "CaratteristicheFisicoSanitarie" ADD FOREIGN KEY ("Paziente") REFERENCES "Paziente" ("CF");
ALTER TABLE "CartellaClinica" ADD FOREIGN KEY ("Paziente") REFERENCES "Paziente" ("CF");
ALTER TABLE "Dottore" ADD FOREIGN KEY ("Reparto") REFERENCES "Reparto" ("CodiceReparto");
ALTER TABLE "Referto" ADD FOREIGN KEY ("Paziente") REFERENCES "Paziente" ("CF");
ALTER TABLE "Referto" ADD FOREIGN KEY ("Dottore") REFERENCES "Dottore" ("CF");
ALTER TABLE "Referto" ADD FOREIGN KEY ("Malattia") REFERENCES "Malattia" ("NomeMalattia");
ALTER TABLE "Lotto" ADD FOREIGN KEY ("Medicina") REFERENCES "Medicina" ("CodiceMedicina");
ALTER TABLE "Volontario" ADD FOREIGN KEY ("Reparto") REFERENCES "Reparto" ("CodiceReparto");
ALTER TABLE "Prescrizione" ADD FOREIGN KEY ("Referto") REFERENCES "Referto" ("NumeroReferto");
ALTER TABLE "Prescrizione" ADD FOREIGN KEY ("Medicina") REFERENCES "Medicina" ("CodiceMedicina");

-- Popolamento delle tabelle
INSERT INTO "MedicoDiBase"("CF", "Nome", "Cognome", "Indirizzo", "Telefono")VALUES
('MDB001', 'Maria', 'Rossi', 'Via Roma 12, Roma', '0551234561'),
('MDB002', 'Luigi', 'Bianchi', 'Via Roma 13, Roma', '0551234562'),
('MDB003', 'Elisa', 'Verde', 'Via Napoli 11, Napoli', '0551234563'),
('MDB004', 'Paolo', 'Marrone', 'Via Verdi 16, Roma', '0551234564'),
('MDB005', 'Sonia', 'Gialli', 'Via Gialli 21, Milano', '0551234565'),
('MDB006', 'Riccardo', 'Blu', 'Corso Azurri 26, Torino', '0551234566'),
('MDB007', 'Laura', 'Bianco', 'Via Bianchi 31, Palermo', '0551234567'),
('MDB008', 'Simone', 'Grigio', 'Via Grigia 36, Genova', '0551234568'),
('MDB009', 'Chiara', 'Rossi', 'Corso Rossi 41, Firenze', '0551234569'),
('MDB010', 'Giulia', 'Ferrari', 'Via Milano 14, Milano', '0551234570'),
('MDB011', 'Andrea', 'Esposito', 'Via Napoli 15, Napoli', '0551234571'),
('MDB012', 'Alessandro', 'Bianchi', 'Via Torino 16, Torino', '0551234572');

-- Popolamento della tabella Malattia
INSERT INTO "Malattia" ("NomeMalattia", "Contagiosa") VALUES
('Influenza', 'Si'),
('Artrite', 'No'),
('Ipertensione', 'No'),
('Diabete', 'No'),
('Cancro', 'No'),
('Epatite', 'Si'),
('HIV', 'Si');

--polamento della tabella reparto :
INSERT INTO "Reparto"("CodiceReparto", "Tipo", "Telefono") VALUES
('CARD', 'Cardiologia', '0551234577'),
('ONCO', 'Oncologia', '0551234578'),
('ORTO', 'Ortopedia', '0551234579'),
('DERM', 'Dermatologia', '0551234580');

-- Popolamento della tabella Dottore
INSERT INTO "Dottore"("CF", "Nome", "Cognome", "Indirizzo", "Telefono", "Reparto") VALUES
('DOC001', 'Giovanni', 'Ferro', 'Via Lungo 2, Milano', '0553216789', 'CARD'),
('DOC002', 'Sara', 'Acqua', 'Via Larga 3, Bologna', '0553216790', 'ONCO'),
('DOC003', 'Luca', 'Legno', 'Via Corta 4, Napoli', '0553216791', 'ORTO'),
('DOC004', 'Elena', 'Pietra', 'Via Alta 5, Torino', '0553216792', 'DERM'),
('DOC005', 'Marco', 'Martini', 'Via Bassa 6, Palermo', '0553216793', 'CARD'),
('DOC006', 'Anna', 'Rossi', 'Via Nuova 7, Genova', '0553216794', 'ONCO'),
('DOC007', 'Alberto', 'Neri', 'Via Vecchia 8, Venezia', '0553216795', 'ORTO'),
('DOC008', 'Francesca', 'Bianchi', 'Via Centro 9, Firenze', '0553216796', 'DERM'),
('DOC009', 'Roberto', 'Verdi', 'Via Fiera 10, Roma', '0553216797', 'CARD');

-- Popolamento della tabella Volontario
INSERT INTO "Volontario"("CF", "Nome", "Cognome", "Indirizzo", "Telefono", "Reparto") VALUES
('VOL001', 'Alice', 'Vento', 'Via Fresca 11, Milano', '0553216788', 'CARD'),
('VOL002', 'Giuseppe', 'Mare', 'Via Mare 12, Genova', '0553216787', 'ONCO'),
('VOL003', 'Simona', 'Lago', 'Via Lago 13, Como', '0553216786', 'ORTO'),
('VOL004', 'Mario', 'Fiume', 'Via Fiume 14, Firenze', '0553216785', 'DERM'),
('VOL005', 'Elisa', 'Montagna', 'Via Montagna 15, Trento', '0553216784', 'CARD');

-- Inserimento dati in tabella Paziente
INSERT INTO "Paziente" ("CF", "Nome", "Cognome", "DataDiNascita", "Indirizzo", "Email", "Telefono", "MedicoDiBase") VALUES
('PZT001', 'Giorgio', 'Verdi', '1980-01-01', 'Via Roma 1, Roma', 'giorgio.verdi@email.it', '0559876543', 'MDB001'),
('PZT002', 'Francesca', 'Neri', '1990-02-01', 'Via Roma 2, Roma', 'francesca.neri@email.it', '0559876544', 'MDB002'),
('PZT003', 'Luca', 'Bianco', '1975-03-15', 'Via Milano 5, Milano', 'luca.bianco@email.it', '0559876545', 'MDB003'),
('PZT004', 'Clara', 'Rosso', '1985-04-22', 'Via Napoli 10, Napoli', 'clara.rosso@email.it', '0559876546', 'MDB004'),
('PZT005', 'Marco', 'Giallo', '1978-05-30', 'Via Torino 15, Torino', 'marco.giallo@email.it', '0559876547', 'MDB005'),
('PZT006', 'Elisa', 'Verde', '1982-06-18', 'Via Firenze 20, Firenze', 'elisa.verde@email.it', '0559876548', 'MDB006'),
('PZT007', 'Roberto', 'Blu', '1969-07-07', 'Via Venezia 25, Venezia', 'roberto.blu@email.it', '0559876549', 'MDB007'),
('PZT008', 'Simona', 'Marrone', '1992-08-16', 'Via Palermo 30, Palermo', 'simona.marrone@email.it', '0559876550', 'MDB008'),
('PZT009', 'Daniele', 'Grigio', '1988-09-09', 'Via Bari 35, Bari', 'daniele.grigio@email.it', '0559876551', 'MDB009'),
('PZT010', 'Sara', 'Russo', '1991-10-10', 'Via Bari 40, Bari', 'sara.russo@email.it', '0559876552', 'MDB010'),
('PZT011', 'Davide', 'Romano', '1989-11-11', 'Via Firenze 45, Firenze', 'davide.romano@email.it', '0559876553', 'MDB011'),
('PZT012', 'Laura', 'Colombo', '1977-12-12', 'Via Roma 50, Roma', 'laura.colombo@email.it', '0559876554', 'MDB012');
	
-- Popolamento della tabella Medicina
INSERT INTO "Medicina" ("CodiceMedicina", "Nome", "PrezzoSingolo", "Forma", "Produttore") VALUES
('MED001', 'Aspirina', 0.50, 'Pillola', 'Pharma1'),
('MED002', 'Paracetamolo', 0.20, 'Pillola', 'Pharma2'),
('MED003', 'Ibuprofene', 0.30, 'Pillola', 'Pharma3'),
('MED004', 'Amoxicillina', 1.50, 'Capsula', 'Pharma4'),
('MED005', 'Metformina', 0.75, 'Capsula', 'Pharma5'),
('MED006', 'Lisinopril', 0.25, 'Pillola', 'Pharma6'),
('MED007', 'Simvastatina', 0.45, 'Pillola', 'Pharma7');

-- Popolamento della tabella Lotto
INSERT INTO "Lotto" ("Medicina", "DataOrdine", "Quantita") VALUES
('MED001', '2023-05-01', 1000),
('MED002', '2023-05-02', 1500),
('MED003', '2023-05-03', 1200),
('MED004', '2023-05-04', 800),
('MED005', '2023-05-05', 950),
('MED006', '2023-05-06', 1100),
('MED007', '2023-05-07', 700);

-- Inserimento dati in tabella CaratteristicheFisicoSanitarie
INSERT INTO "CaratteristicheFisicoSanitarie" ("Paziente", "Altezza", "Peso", "Sesso", "GruppoSanguigno", "RH") VALUES
('PZT001', '175', '70', 'M', 'A', '+'),
('PZT002', '165', '60', 'F', '0', '-'),
('PZT003', '180', '78', 'M', 'B', '+'),
('PZT004', '170', '58', 'F', 'AB', '-'),
('PZT005', '182', '88', 'M', 'A', '+'),
('PZT006', '160', '52', 'F', '0', '-'),
('PZT007', '174', '85', 'M', 'B', '-'),
('PZT008', '168', '62', 'F', 'AB', '+'),
('PZT009', '177', '76', 'M', 'A', '+'),
('PZT010', '163', '55', 'F', '0', '+'),
('PZT011', '185', '90', 'M', 'B', '-'),
('PZT012', '170', '65', 'F', 'AB', '+');

-- Inserimento dati in tabella CartellaClinica
INSERT INTO "CartellaClinica" ("CodiceCartella", "Data", "PressioneMax", "PressioneMin", "Frequenza", "Paziente") VALUES
('CC101', '2023-06-01', 120, 80, 70, 'PZT001'),
('CC102', '2023-06-02', 130, 85, 72, 'PZT002'),
('CC103', '2023-06-03', 125, 82, 68, 'PZT003'),
('CC104', '2023-06-04', 135, 90, 74, 'PZT004'),
('CC105', '2023-06-05', 140, 85, 76, 'PZT005'),
('CC106', '2023-06-06', 115, 75, 65, 'PZT006'),
('CC107', '2023-06-07', 118, 78, 64, 'PZT007'),
('CC108', '2023-06-08', 123, 79, 69, 'PZT008'),
('CC109', '2023-06-09', 129, 84, 75, 'PZT009'),
('CC110', '2023-06-10', 125, 85, 72, 'PZT010'),
('CC111', '2023-06-11', 130, 90, 75, 'PZT011'),
('CC112', '2023-06-12', 120, 80, 70, 'PZT012');

-- Popolamento della tabella Referto
INSERT INTO "Referto"("NumeroReferto", "Note", "Data", "Ora", "Paziente", "Dottore", "Malattia") VALUES
(101, 'Visita di routine, nessun problema rilevato.', '2023-06-10', '10:30:00', 'PZT001', 'DOC001', NULL),
(102, 'Diagnosi di influenza, prescritto riposo e fluidi.', '2023-06-11', '11:00:00', 'PZT002', 'DOC002', 'Influenza'),
(103, 'Controllo pressione, valori leggermente elevati.', '2023-06-12', '09:45:00', 'PZT003', 'DOC003', NULL),
(104, 'Visita di controllo per artrite, stabile.', '2023-06-13', '14:15:00', 'PZT004', 'DOC004', 'Artrite'),
(105, 'Segnalazione di mal di testa frequenti, esami ulteriori richiesti.', '2023-06-14', '16:20:00', 'PZT005', 'DOC005', NULL),
(106, 'Visita di routine, nessun problema rilevato.', '2023-06-15', '10:30:00', 'PZT010', 'DOC006', NULL),
(107, 'Diagnosi di influenza, prescritto riposo e fluidi.', '2023-06-16', '11:00:00', 'PZT011', 'DOC007', 'Influenza'),
(108, 'Controllo pressione, valori leggermente elevati.', '2023-06-17', '09:45:00', 'PZT012', 'DOC008', NULL);

-- Popolamentodella tabella Prescrizione:
INSERT INTO "Prescrizione" ("Referto", "Medicina") VALUES
(101, 'MED001'),
(102, 'MED002'),
(103, 'MED003'),
(104, 'MED004'),
(105, 'MED005'),
(101, 'MED006'),
(102, 'MED007'),
(106, 'MED006'),
(107, 'MED007'),
(108, 'MED001');

--CREAZIONE DEGLI INDICI    

CREATE INDEX "idx_paziente_cf" ON "Paziente"("CF");

CREATE INDEX "idx_paziente_medicodibase" ON "Paziente"("MedicoDiBase");

-- 1: Nome del paziente, la data del suo record medico e le note del suo referto medico.
SELECT  
    "Paziente"."Nome",  
    "Paziente"."Cognome",  
    "CartellaClinica"."Data",  
    "Referto"."Note" 
FROM  
    "Paziente" 
JOIN  
    "CartellaClinica" ON "Paziente"."CF" = "CartellaClinica"."Paziente" 
JOIN  
    "Referto" ON "Paziente"."CF" = "Referto"."Paziente";


--2 Numero di pazienti per medico di base: 

SELECT   
    "MedicoDiBase"."Nome",   
    "MedicoDiBase"."Cognome",   
    COUNT("Paziente"."CF") AS "Numero di Pazienti"  
FROM   
    "MedicoDiBase"  
JOIN   
    "Paziente" ON "MedicoDiBase"."CF" = "Paziente"."MedicoDiBase"  
WHERE  
    "MedicoDiBase"."Nome" = 'Maria' AND 
    "MedicoDiBase"."Cognome" = 'Rossi'
GROUP BY   
    "MedicoDiBase"."Nome",   
    "MedicoDiBase"."Cognome" 
HAVING  
    COUNT("Paziente"."CF") > 0;


--3 Media dell’età dei pazienti per sesso:	
SELECT   "CaratteristicheFisicoSanitarie"."Sesso",   
    AVG(EXTRACT(YEAR FROM AGE("Paziente"."DataDiNascita"))) AS "Età Media"  
FROM   
    "Paziente"  
JOIN   
    "CaratteristicheFisicoSanitarie" ON "Paziente"."CF" = "CaratteristicheFisicoSanitarie"."Paziente"  
WHERE  
    "CaratteristicheFisicoSanitarie"."Sesso" = 'M' 
GROUP BY   
    "CaratteristicheFisicoSanitarie"."Sesso";

--4 Numero di referti per malattia:
SELECT  
    "Malattia"."NomeMalattia",  
    COUNT("Referto"."NumeroReferto") AS "Numero di Referti" 
FROM  
    "Malattia" 
LEFT JOIN  
    "Referto" ON "Malattia"."NomeMalattia" = "Referto"."Malattia" 
GROUP BY  
    "Malattia"."NomeMalattia"; 
  

--5 Numero di pazienti per medico di base con almeno un paziente:
SELECT  
    "MedicoDiBase"."Nome",  
    "MedicoDiBase"."Cognome",  
    COUNT("Paziente"."CF") AS "Numero di Pazienti" 
FROM  
    "MedicoDiBase" 
JOIN  
    "Paziente" ON "MedicoDiBase"."CF" = "Paziente"."MedicoDiBase" 
GROUP BY  
    "MedicoDiBase"."Nome",  
    "MedicoDiBase"."Cognome" 
HAVING  
    COUNT("Paziente"."CF") > 0; 


--6 Medici che hanno fatto almeno un referto:
SELECT  
    "Dottore"."Nome",  
    "Dottore"."Cognome",  
    COUNT("Referto"."NumeroReferto") AS "Numero di Referti" 
FROM  
    "Dottore" 
JOIN  
    "Referto" ON "Dottore"."CF" = "Referto"."Dottore" 
GROUP BY  
    "Dottore"."Nome",  
    "Dottore"."Cognome" 
HAVING  
    COUNT("Referto"."NumeroReferto") > 0; 
