-- database.sql

PRAGMA foreign_keys = ON;  --per attivare i vincoli su DB browser for sqlite

DROP TABLE IF EXISTS BUDGET;
DROP TABLE IF EXISTS SPESE;
DROP TABLE IF EXISTS CATEGORIE;

CREATE TABLE CATEGORIE (
        nomecat text PRIMARY KEY NOT NULL  
 );

CREATE TABLE SPESE (
    id_spesa INTEGER PRIMARY KEY AUTOINCREMENT,
    nomecat text NOT NULL,
    data text NOT NULL CHECK (data LIKE '____-__-__'),
    importo REAL NOT NULL CHECK (importo > 0),   --al posto di float sqlite usa REAL
    descrizione text DEFAULT 'nessuna',    --sqlite usa text al posto di varchar
    FOREIGN KEY (nomecat) REFERENCES CATEGORIE(nomecat) 
       ON UPDATE CASCADE
       ON DELETE NO ACTION
);

CREATE TABLE BUDGET (
    nomecat text NOT NULL,
    mese text NOT NULL CHECK (mese LIKE '____-__'),
    importob REAL NOT NULL CHECK (importob >= 0),
    PRIMARY KEY  (nomecat,mese),
    FOREIGN KEY (nomecat) REFERENCES CATEGORIE(nomecat)
        ON UPDATE CASCADE
        ON DELETE NO ACTION
);



