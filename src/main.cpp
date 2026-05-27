
#include <iostream>
#include <string>
#include <sqlite3.h>
using namespace std;

void gestionecategorie(sqlite3* db);
void inseriscispesa(sqlite3* db);
void definiscibudget(sqlite3* db);
void visualizzareport(sqlite3* db);

int main () {
    int scelta=0;

    sqlite3* database = nullptr;     // puntatore al database dichiarandolo nullo
    int risultato = sqlite3_open("sql/spese.db", &database);   // apertura database nella cartella del progetto
    if (risultato != SQLITE_OK) {           //verifica che si è aperto il database
        cerr << "Impossibile aprire il database: " << sqlite3_errmsg(database) << endl;  //indica un errore per questo niente cout
        if (database) sqlite3_close(database);
        return 1;
    }

    do {
        cout<<endl;
        cout<< "BENVENUTO NEL TUO SALVASPESE"<< endl;
        cout<<"-------------------------------"<<endl;
        cout << " SISTEMA SPESE PERSONALI"<< endl;
        cout<<"-------------------------------"<<endl;
        cout <<"1. gestione categorie" <<endl;
        cout<<"2. inserisci spesa"<<endl;
        cout<<"3. definisci budget"<<endl;
        cout<<"4. visualizza report"<<endl;
        cout<<"5. esci"<<endl;
        cout<<"-------------------------------"<<endl;
        cout <<" inserisci la tua scelta:";
        cin>> scelta;
        
        switch (scelta) {
            case 1:
                gestionecategorie(database);
                break;
            case 2:
                inseriscispesa(database);
                break;
            case 3:
                definiscibudget(database);
                break;
            case 4:
                visualizzareport(database);
                break;
            case 5:
                cout<<"uscita dal programma";
                sqlite3_close(database);    //chiusura database
                break;
                
            default: cout <<"scelta non valida, riprovare";
        }
        
        } while (scelta!=5);
    return 0;
}


void gestionecategorie( sqlite3* db) {
    string nuovacat;
    cout << " Inserisci la nuova categoria: ";
    cin.ignore(); // Pulisce il buffer 
    getline(cin, nuovacat);   //consente spazi
    
    if (nuovacat == "") {
        cout << " Non puo' essere vuota la categoria!" << endl;
        return;
    }
    string query = "INSERT INTO categorie (nomecat) VALUES (?);";  //inserimento nuova categoria 
    sqlite3_stmt* stmt;  
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Errore nella preparazione della query: " << sqlite3_errmsg(db) << endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, nuovacat.c_str(), -1, SQLITE_STATIC);      //associazione del parametro con il valore inserito
    
    if (sqlite3_step(stmt) == SQLITE_DONE) {           
        cout << "Categoria inserita con successo!" << endl;
    } else {
        cerr << "Errore nell'inserimento: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);     // Liberazione della memoria
       
}
    

void inseriscispesa(sqlite3* db) {
    int anno=0,mese=0,giorno=0;
    char trattino1, trattino2;
    cout <<"inserisci la data della tua spesa (formato: AAAA-MM-GG): ";
    cin >> anno >> trattino1 >> mese >> trattino2 >> giorno;
    if (trattino1 != '-' || trattino2 != '-' || anno < 2000 || mese > 12 || giorno > 31) {
        cout << "Errore: Formato data non valido!" << endl;
        return;
    }

    double importo=0.0;
    do{
        cout <<"quanto hai speso?";
        cin>> importo;
        if(importo<=0) {
            cout <<"errore inserimento importo";
        }
    }while (importo<=0);
    
    string descrizione=" ";
    cout <<"descrizione spesa (facoltativa)";
    cin.ignore(); // Ignora il carattere di nuova linea rimasto nel buffer
    getline(cin, descrizione); // Legge l'intera riga per la descrizione della spesa (legge pure gli spazi)
    if (descrizione == "") {
        descrizione = "nessuna"; 
    }

    string catspesa;
    cout<<"in che categoria hai speso questi soldi?";
    cin>> catspesa;
 
 
    // Costruzione della data nel formato AAAA-MM-GG
    string dataFinale = to_string(anno) + "-" + (mese < 10 ? "0" : "") + to_string(mese) + "-" + (giorno < 10 ? "0" : "") + to_string(giorno);
    
    string query = "INSERT INTO SPESE (data, nomecat, descrizione, importo) VALUES (?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;   // dichiarazione del puntatore alla struttura sqlite3_stmt per la preparazione della query
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);  //preparazione della query con i parametri '?'
                                                                      // Associazione dei parametri con i valori inseriti dall'utente
    sqlite3_bind_text(stmt, 1, dataFinale.c_str(), -1, SQLITE_STATIC);  // 1° '?' -> Data
    sqlite3_bind_text(stmt, 2, catspesa.c_str(), -1, SQLITE_STATIC);   // 2° '?' -> catspesa
    sqlite3_bind_text(stmt, 3, descrizione.c_str(), -1, SQLITE_STATIC); // 3° '?' -> Descrizione
    sqlite3_bind_double(stmt, 4, importo);                             // 4° '?' -> Importo
     
    if (sqlite3_step(stmt) == SQLITE_DONE) {           //verifica se l'inserimento è avenuto 
        cout << "Spesa inserita con successo!" << endl;
    } else {
        cerr << "Errore nell'inserimento: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);     // Liberazione della memoria 
}
    
void definiscibudget (sqlite3* db) {
        string catbudget;
        cout<<"per quale categoria vuoi definire il budget?";
        cin>> catbudget;

        string mese;
        cout << "Per quale mese? (Formato: AAAA-MM): ";
        cin >> mese;
        
        double importob=0.0;
        do{
            cout <<"quanto vuoi spendere al massimo in questa categoria?";
            cin>> importob;
            if(importob<=0) {
                cout <<"errore inserimento importo";
            }
        }while (importob<=0);
        
        string query = "INSERT OR REPLACE INTO BUDGET (nomecat, mese, importob) VALUES (?, ?, ?);";
        
        sqlite3_stmt* stmt;   // dichiarazione del puntatore alla struttura sqlite3_stmt per la preparazione della query
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);  //preparazione della query con i parametri '?'
                                                                        // Associazione dei parametri con i valori inseriti dall'utente
        sqlite3_bind_text(stmt, 1, catbudget.c_str(), -1, SQLITE_STATIC);  // 1° '?' -> catbudget
        sqlite3_bind_text(stmt, 2, mese.c_str(), -1, SQLITE_STATIC);     // 2° '?' -> mese
        sqlite3_bind_double(stmt, 3, importob);                             // 3° '?' -> Importob
        
        if (sqlite3_step(stmt) == SQLITE_DONE) {           //verifica se l'inserimento è avenuto 
            cout << "Budget definito con successo!" << endl;
        } else {
            cerr << "Errore nell'inserimento: " << sqlite3_errmsg(db) << endl;
        }
        
        sqlite3_finalize(stmt);     // Liberazione della memoria
    
    
}

void visualizzareport (sqlite3* db) {
    int sceltaReport = 0;
    do {
        cout << "\nMENU DEI REPORT" << endl;
        cout << "1. Totale spese per categoria" << endl;
        cout << "2. Spese mensili vs budget" << endl;
        cout << "3. Elenco completo delle spese ordinate per data" << endl;
        cout << "4. Ritorna al menu principale" << endl;
        cout << "-------------------------------" << endl;
        cout << "Inserisci la tua scelta: ";
        cin >> sceltaReport;

        string query;
        sqlite3_stmt* stmt;

        switch (sceltaReport) {
            case 1:
                query = "SELECT c.nomecat, COALESCE(SUM(s.importo), 0) AS totale "
                        "FROM CATEGORIE c "
                        "LEFT JOIN SPESE s ON c.nomecat = s.nomecat "
                        "GROUP BY c.nomecat "
                        "ORDER BY totale DESC;";
                break;
            case 2:
                query = "SELECT b.mese, b.nomecat, COALESCE(SUM(s.importo), 0) AS speso, b.importob AS budget "
                        "FROM BUDGET b "
                        "LEFT JOIN SPESE s ON b.nomecat = s.nomecat "
                        "AND substr(s.data, 1, 7) = b.mese "
                        "GROUP BY b.mese, b.nomecat, b.importob "
                        "ORDER BY b.mese DESC, b.nomecat;";
                break;
            case 3:
                query = "SELECT data, nomecat, descrizione, importo FROM SPESE ORDER BY data DESC;";
                break;
            case 4:
                break;
            default:
                cout << "Scelta non valida, riprova." << endl;
                break;
        }

        if (sceltaReport >= 1 && sceltaReport <= 3) {
            if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                cerr << "Errore nella preparazione della query: " << sqlite3_errmsg(db) << endl;
            } else {
                if (sceltaReport == 1) {
                    cout << "\nCategoria\tTotale" << endl;
                } else if (sceltaReport == 2) {
                    cout << "\nMese\tCategoria\tSpeso\tBudget\tStato Budget" << endl;
                } else if (sceltaReport == 3) {
                    cout << "\nData\t\tCategoria\tDescrizione\tImporto" << endl;
                }
                cout << "------------------------------------------------------------------------------------------" << endl;

                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    if (sceltaReport == 1) {
                        const unsigned char* nomecat = sqlite3_column_text(stmt, 0);
                        double totale = sqlite3_column_double(stmt, 1);
                        cout << nomecat << "\t\t" << totale << " EUR" << endl;
                    } 
                    else if (sceltaReport == 2) {
                        // 1. Prima estraiamo le variabili dal database (Così il C++ sa cosa sono!)
                        const unsigned char* mese = sqlite3_column_text(stmt, 0);
                        const unsigned char* nomecat = sqlite3_column_text(stmt, 1);
                        double speso = sqlite3_column_double(stmt, 2);
                        double budget = sqlite3_column_double(stmt, 3);
                        
                        // 2. Ora applichiamo la logica di controllo usando le variabili appena estratte
                        string statoBudget;
                        if (speso > budget) {
                            statoBudget = "--> SOGLIA SUPERATA!";
                        } else {
                            statoBudget = "[OK - Ancora disponibile: " + to_string(budget - speso) + " EUR]";
                        }
                        
                        // 3. Stampiamo la riga completa a schermo
                        cout << mese << "\t" << nomecat << "\t\t" << speso << "\t" << budget << "\t" << statoBudget << endl;
                    } 
                    else if (sceltaReport == 3) {
                        const unsigned char* data = sqlite3_column_text(stmt, 0);
                        const unsigned char* catspesa = sqlite3_column_text(stmt, 1);
                        const unsigned char* descspesa = sqlite3_column_text(stmt, 2);
                        double importo = sqlite3_column_double(stmt, 3);
                        cout << data << "\t" << catspesa << "\t\t" << descspesa << "\t\t" << importo << " EUR" << endl;
                    }
                }
                sqlite3_finalize(stmt);
            }
        }
    } while (sceltaReport != 4);
}
