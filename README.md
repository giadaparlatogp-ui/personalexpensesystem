// utilizzato Visual Studio Code per il codice e DB Browser for SQlite per il database

//librerie utilizzate: <iostream>, <string>, <sqlite3.h>

// per attivare il programma su un mac, dopo aver aperto la cartella del progetto, entrare nella cartella "src" ed aprire il file "main"


// sucessivamente aprire il terminale di Visual Studio Code e riportare questi comandi:

1* compilazone del codice sorgente: clang++ src/main.cpp -o main -lsqlite3

2* inizializzazione delle tabelle (opzionale): sqlite3 sql/spese.db < sql/database.sql

3* per avviare l'interfaccia testuale e interattiva: ./main