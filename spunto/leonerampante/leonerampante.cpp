#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bits/stdc++.h> 
#include "dependencies/include/libpq-fe.h"

#define PG_PORT 5432

using namespace std;

// Metodo per ottenere i parametri in ingresso
char* getConnectionParameters(string param) {
    
	char* parameters = new char[50];
	
	cout << "Inserisci "<< param << " : \n";
	fgets(parameters, 50, stdin);

	return parameters;
}

// Metodo per la connessione con parametri da input
PGconn* connect(const char* user, const char* pwd, const char* db, const char* host, const char* port) {

    char conninfo[250];
    
	sprintf(conninfo, "user=%s password=%s dbname=%s hostaddr=%s port=%s",
        user, pwd, db, host, port);

    PGconn* conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "\nErrore di connessione:" << endl << PQerrorMessage(conn);
        PQfinish(conn);
        exit(1);
    }

	cout << "\nConnessione avvenuta con successo\n" << endl;
    return conn;
}

// Metodo di esecuzione della query
PGresult* execute(PGconn* conn, const char* query) {
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cout << "\nErrore di visualizzazione dati:\n" << PQerrorMessage(conn) << "\n" << endl;
        PQclear(res);
        exit(1);
    }

    return res;
}

// Metodo per stampare la tabella
void printResult(PGresult* result) {
	cout<< "Risultato: \n" << endl;

	int tuple = PQntuples(result);
	int campi = PQnfields(result);
	
	// Stampa intestazioni
	for (int i = 0; i < campi ; ++ i){
		cout << left << setw(30) << PQfname(result, i);
	}

	cout << "\n\n"; 
	// Stampa valori
	for(int i=0; i < tuple; ++i) {
		for (int n=0; n<campi; ++n) {
			cout << left << setw(30) << PQgetvalue(result, i, n);
		}
		cout << '\n';
	}

	cout<<'\n'<< endl;
	
	PQclear(result);
}

// Metodo per stampare la lista delle descrizioni delle queries
void printQueryList(){
	cout << "\nLista delle Queries : \n";
	cout << "1: Visualizzare il numero di prenotazioni di ogni evento associato ad un determinato LARP.\n";
	cout << "2: Visualizzare tutti i personaggi non ancora assegnati o prenotati di uno specifico evento.\n";
	cout << "3: Visualizzare il luogo dove si sono svolti il maggior numero di eventi.\n";
	cout << "4: Visualizzare quante volte e' stato scelto un determinato personaggio tra prenotazioni e assegnazioni.\n";
	cout << "5: Visualizzare i titoli di ogni storia, il numero di prenotazioni di tutti gli eventi a loro associati e il loro ricavo totale.\n";
	cout << "6: Visualizzare, in base al codice fiscale di un giocatore, i personaggi di altri giocatori iscritti allo stesso evento con cui il personaggio prenotato ha delle interazioni, e il tipo di legame descritto. \n" << endl;
}

// Metodo di scelta della query
char* choseQuery(PGconn* conn){
	int n = 0;
	while (n <-1 || n == 0 || n > 6) {
		cout << "Digitare: \n-1 : Chiusura del programma.\n0 : Visualizzazione della lista delle queries disponibile.\n[1-6] : Selezione della query specifica." << endl;
		cin >> n;
		if(n == 0){
			printQueryList();
		}
	}
	if(n == -1){
		PQfinish(conn);
		exit(1);
	}
		
	string query;
	char paramquery[1500];
	int dim = 0;
	if(n == 1){
		// "1: Visualizzare il numero di prenotazioni di ogni evento associato ad un determinato LARP.\n";
		dim = 6;
		query =  "SELECT pr.evento, count(*) as conteggio FROM Prenotazione as pr \
		WHERE pr.evento LIKE '%s%%' GROUP BY pr.evento ORDER BY pr.evento ASC;";
		char codice1[dim];
		cout << "Inserisci il codice del LARP: ";
		cin >> codice1;
		sprintf(paramquery, strcpy(new char[query.length() +1],query.c_str()), codice1);
		return paramquery;
	}
	if(n == 2){
		// "2: Visualizzare tutti i personaggi non ancora assegnati o prenotati di uno specifico evento.\n";
		dim = 9;
		query =  "SELECT pg.* as evento from Personaggio as pg JOIN Ruolo as r \
		on r.personaggio = pg.codice JOIN larp as lp on lp.codice = r.larp JOIN evento \
		as e on e.larp = lp.codice WHERE e.codice LIKE '%s' AND pg.codice NOT IN \
		( SELECT pr.personaggio from prenotazioni_personaggi as pr WHERE pr.evento \
		LIKE '%s') AND pg.codice NOT IN ( SELECT ap.personaggio from \
		assegnazioni_personaggi as ap WHERE ap.evento LIKE '%s');";
		char codice2[dim];
		cout << "Inserisci il codice dell'evento: ";
		cin >> codice2;
		sprintf(paramquery, strcpy(new char[query.length() +1],query.c_str()), codice2,codice2,codice2);
		return paramquery;
	}
	if(n == 3){
		// "3: Visualizzare il luogo dove si sono svolti il maggior numero di eventi.\n";
		query =  "SELECT * FROM luoghi_eventi as le limit 1;";
		return strcpy(new char[query.length() +1],query.c_str());
	}
	if(n == 4){
		// "4: Visualizzare quante volte e' stato scelto un determinato personaggio tra prenotazioni e assegnazioni.\n";
		dim = 6;
		query =  "SELECT pgtotal.codice, count(pgtotal.codice) FROM ( SELECT pg.codice \
		FROM personaggio as pg JOIN prenotazione as pr ON pr.personaggio = pg.codice \
		UNION ALL SELECT pg1.codice FROM personaggio as pg1 JOIN assegnazione as asg \
		ON asg.personaggio = pg1.codice ) as pgtotal GROUP BY pgtotal.codice \
		HAVING pgtotal.codice LIKE '%s';";
		char codice4[dim];
		cout << "Inserisci il codice del personaggio: ";
		cin >> codice4;
		sprintf(paramquery, strcpy(new char[query.length() +1],query.c_str()), codice4);
		return paramquery;
	}
	if(n == 5){
		// "5: Visualizzare i titoli di ogni storia, il numero di prenotazioni di tutti gli eventi a loro associati e il loro ricavo totale.\n";
		query =  "SELECT Storia.titolo, COUNT(pr.evento) as conteggio, \
		SUM(pr.importo) as ricavo FROM Storia JOIN LARP ON LARP.storia = Storia.titolo \
		JOIN evento as e ON e.larp = LARP.codice JOIN prenotazione as pr \
		on pr.evento = e.codice GROUP BY Storia.titolo;";
		return strcpy(new char[query.length() +1],query.c_str());
	}
	// "6: Visualizzare, in base al codice fiscale di un giocatore, i personaggi di altri giocatori iscritti allo stesso evento con cui il personaggio prenotato ha delle interazioni, e il tipo di legame descritto. \n"
	dim = 16;
	query =  "SELECT pp1.utente, pp1.evento, pp1.personaggio, r1.personaggio2 as interazione,\
		r1.tipo as relazione FROM prenotazioni_personaggi as pp1 JOIN Relazione as r1 \
		on r1.personaggio1 = pp1.personaggio WHERE pp1.utente LIKE '%s' \
		AND r1.personaggio2 IN (SELECT pp2.personaggio FROM prenotazioni_personaggi as pp2 \
		WHERE pp2.personaggio = r1.personaggio2 AND pp2.evento = pp1.evento) \
		UNION \
		SELECT pp3.utente, pp3.evento, pp3.personaggio, r2.personaggio1 as interazione, \
		r2.tipo as relazione FROM prenotazioni_personaggi as pp3 JOIN Relazione as r2 \
		on r2.personaggio2 = pp3.personaggio WHERE pp3.utente LIKE '%s' \
		AND r2.personaggio1 IN (SELECT pp4.personaggio from prenotazioni_personaggi \
		as pp4 WHERE pp4.personaggio = r2.personaggio1 AND pp4.evento = pp3.evento);";
	char codice6[dim];
	cout << "Inserisci il codice fiscale dell'utente: ";
	cin >> codice6;
	sprintf(paramquery, strcpy(new char[query.length() +1],query.c_str()), codice6, codice6);
	return paramquery;
}

int main ( int argc , char ** argv ) {
	// Richiesta dei parametri necessari alla connessione
	char* user = getConnectionParameters("utente");
	char* pwd = getConnectionParameters("password");
	char* db = getConnectionParameters("database");
	char* host = getConnectionParameters("host");
	char* port = getConnectionParameters("porta");

	// Connessione effettiva
    PGconn* conn = connect(user,pwd,db,host,port);
	
	
	while(true){
		char* chosenquery = choseQuery(conn);

		PGresult* result = execute(conn, chosenquery);
		
		printResult(result);
	}

	PQfinish(conn);
	
	return 0;
}
