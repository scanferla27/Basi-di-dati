#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

void check_results(PGresult *res, PGconn *conn) {
    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
}

void execute_query(PGconn *conn, const char *query) {
    PGresult *res = PQexec(conn, query);
    check_results(res, conn);
    PQclear(res);
}

void print_query_results(PGconn *conn, const char *query) {
    PGresult *res = PQexec(conn, query);
    check_results(res, conn);

    int nfields = PQnfields(res);
    int ntuples = PQntuples(res);

    // Print column headers
    for (int i = 0; i < nfields; i++) {
        printf("%-30s", PQfname(res, i));
    }
    printf("\n");

    // Print rows
    for (int i = 0; i < ntuples; i++) {
        for (int j = 0; j < nfields; j++) {
            printf("%-30s", PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    PQclear(res);
}

void execute_parametric_query(PGconn *conn, int query_index) {
    char param1[256];
    char param2[256];
    char query[1024];

    switch (query_index) {
        case 1:
            printf("Inserisci il nome del medico di base: ");
            scanf("%s", param1);
            printf("Inserisci il cognome del medico di base: ");
            scanf("%s", param2);
            snprintf(query, sizeof(query), 
                     "SELECT MedicoDiBase.Nome, MedicoDiBase.Cognome, COUNT(Paziente.CF) AS \"Numero di Pazienti\" "
                     "FROM MedicoDiBase "
                     "JOIN Paziente ON MedicoDiBase.CF = Paziente.MedicoDiBase "
                     "WHERE MedicoDiBase.Nome = '%s' AND MedicoDiBase.Cognome = '%s' "
                     "GROUP BY MedicoDiBase.Nome, MedicoDiBase.Cognome "
                     "HAVING COUNT(Paziente.CF) > 0;", param1, param2);
            break;
        case 2:
            printf("Inserisci il sesso (M/F): ");
            scanf("%s", param1);
            snprintf(query, sizeof(query), 
                     "SELECT CaratteristicheFisicoSanitarie.Sesso, "
                     "AVG(EXTRACT(YEAR FROM AGE(Paziente.DataDiNascita))) AS \"Età Media\" "
                     "FROM Paziente "
                     "JOIN CaratteristicheFisicoSanitarie ON Paziente.CF = CaratteristicheFisicoSanitarie.Paziente "
                     "WHERE CaratteristicheFisicoSanitarie.Sesso = '%s' "
                     "GROUP BY CaratteristicheFisicoSanitarie.Sesso;", param1);
            break;
        case 3:
            printf("Inserisci il nome della malattia: ");
            scanf("%s", param1);
            snprintf(query, sizeof(query), 
                     "SELECT Malattia.NomeMalattia, COUNT(Referto.NumeroReferto) AS \"Numero di Referti\" "
                     "FROM Malattia "
                     "LEFT JOIN Referto ON Malattia.NomeMalattia = Referto.Malattia "
                     "WHERE Malattia.NomeMalattia = '%s' "
                     "GROUP BY Malattia.NomeMalattia;", param1);
            break;
        default:
            printf("Query non parametrica selezionata.\n");
            return;
    }

    print_query_results(conn, query);
}

int main() {
    const char *conninfo = "host=localhost port=5432 dbname=postgres user=postgres password=passwd";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    // Creazione delle tabelle se non esistono
    const char *create_tables[] = {
    "CREATE TABLE IF NOT EXISTS MedicoDiBase (CF VARCHAR(16) PRIMARY KEY NOT NULL, Nome VARCHAR(50) NOT NULL, Cognome VARCHAR(50) NOT NULL, Indirizzo VARCHAR(100) NOT NULL, Telefono VARCHAR(15) NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Reparto (CodiceReparto VARCHAR(5) PRIMARY KEY NOT NULL, Tipo VARCHAR(50) NOT NULL, Telefono VARCHAR(15) NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Paziente (CF VARCHAR(16) PRIMARY KEY NOT NULL, Nome VARCHAR(50) NOT NULL, Cognome VARCHAR(50) NOT NULL, DataDiNascita DATE NOT NULL, Indirizzo VARCHAR(100) NOT NULL, Email VARCHAR(100) NOT NULL, Telefono VARCHAR(15) NOT NULL, MedicoDiBase VARCHAR(16) NOT NULL, FOREIGN KEY (MedicoDiBase) REFERENCES MedicoDiBase(CF));",
    "CREATE TABLE IF NOT EXISTS CaratteristicheFisicoSanitarie (Paziente VARCHAR(16) PRIMARY KEY NOT NULL, Altezza VARCHAR(5) NOT NULL, Peso NUMERIC(5,2) NOT NULL, Sesso VARCHAR(10) NOT NULL, GruppoSanguigno VARCHAR(3) NOT NULL, RH VARCHAR(1) NOT NULL, FOREIGN KEY (Paziente) REFERENCES Paziente(CF));",
    "CREATE TABLE IF NOT EXISTS CartellaClinica (CodiceCartella VARCHAR(10) PRIMARY KEY NOT NULL, Data DATE NOT NULL, PressioneMax NUMERIC(3,0) NOT NULL, PressioneMin NUMERIC(3,0) NOT NULL, Frequenza NUMERIC(3,0) NOT NULL, Paziente VARCHAR(16) NOT NULL, FOREIGN KEY (Paziente) REFERENCES Paziente(CF));",
    "CREATE TABLE IF NOT EXISTS Malattia (NomeMalattia VARCHAR(50) PRIMARY KEY NOT NULL, Contagiosa VARCHAR(2) NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Dottore (CF VARCHAR(16) PRIMARY KEY NOT NULL, Nome VARCHAR(50) NOT NULL, Cognome VARCHAR(50) NOT NULL, Indirizzo VARCHAR(100) NOT NULL, Telefono VARCHAR(15) NOT NULL, Reparto VARCHAR(5) NOT NULL, FOREIGN KEY (Reparto) REFERENCES Reparto(CodiceReparto));",
    "CREATE TABLE IF NOT EXISTS Referto (NumeroReferto NUMERIC(10,0) PRIMARY KEY NOT NULL, Note VARCHAR(500) NOT NULL, Data DATE NOT NULL, Ora TIME NOT NULL, Paziente VARCHAR(16) NOT NULL, Dottore VARCHAR(16) NOT NULL, Malattia VARCHAR(50), FOREIGN KEY (Paziente) REFERENCES Paziente(CF), FOREIGN KEY (Dottore) REFERENCES Dottore(CF), FOREIGN KEY (Malattia) REFERENCES Malattia(NomeMalattia));",
    "CREATE TABLE IF NOT EXISTS Medicina (CodiceMedicina VARCHAR(10) PRIMARY KEY NOT NULL, Nome VARCHAR(50) NOT NULL, PrezzoSingolo NUMERIC(5,2) NOT NULL, Forma VARCHAR(50) NOT NULL, Produttore VARCHAR(50) NOT NULL);",
    "CREATE TABLE IF NOT EXISTS Lotto (Medicina VARCHAR(10) NOT NULL, DataOrdine DATE NOT NULL, Quantita NUMERIC(5,0) NOT NULL, PRIMARY KEY (Medicina, DataOrdine), FOREIGN KEY (Medicina) REFERENCES Medicina(CodiceMedicina));",
    "CREATE TABLE IF NOT EXISTS Volontario (CF VARCHAR(16) PRIMARY KEY NOT NULL, Nome VARCHAR(50) NOT NULL, Cognome VARCHAR(50) NOT NULL, Indirizzo VARCHAR(100) NOT NULL, Telefono VARCHAR(15) NOT NULL, Reparto VARCHAR(5) NOT NULL, FOREIGN KEY (Reparto) REFERENCES Reparto(CodiceReparto));",
    "CREATE TABLE IF NOT EXISTS Prescrizione (Referto NUMERIC(10,0) NOT NULL, Medicina VARCHAR(10) NOT NULL, PRIMARY KEY (Referto, Medicina), FOREIGN KEY (Referto) REFERENCES Referto(NumeroReferto), FOREIGN KEY (Medicina) REFERENCES Medicina(CodiceMedicina));"
};

    for (int i = 0; i < sizeof(create_tables) / sizeof(create_tables[0]); i++) {
        execute_query(conn, create_tables[i]);
    }

    // Descrizioni delle query
    const char *query_descriptions[] = {
        "Nome del paziente, la data del suo record medico e le note del suo referto medico",
        "Numero di pazienti per medico di base",
        "Media dell’età dei pazienti per sesso",
        "Numero di referti per malattia",
        "Numero di pazienti per medico di base con almeno 1 paziente",
        "Medici che hanno almeno referto"
    };

    // Query predefinite
    const char *queries[] = {
        "SELECT Paziente.Nome, Paziente.Cognome, CartellaClinica.Data, Referto.Note "
        "FROM Paziente "
        "JOIN CartellaClinica ON Paziente.CF = CartellaClinica.Paziente "
        "JOIN Referto ON Paziente.CF = Referto.Paziente;",
        
        "SELECT MedicoDiBase.Nome, MedicoDiBase.Cognome, COUNT(Paziente.CF) AS Numero_di_Pazienti "
        "FROM MedicoDiBase "
        "JOIN Paziente ON MedicoDiBase.CF = Paziente.MedicoDiBase "
        "WHERE MedicoDiBase.Nome = 'Maria' AND MedicoDiBase.Cognome = 'Rossi' "
        "GROUP BY MedicoDiBase.Nome, MedicoDiBase.Cognome "
        "HAVING COUNT(Paziente.CF) > 0;",
        
        "SELECT CaratteristicheFisicoSanitarie.Sesso, "
        "AVG(EXTRACT(YEAR FROM AGE(Paziente.DataDiNascita))) AS Età_Media "
        "FROM Paziente "
        "JOIN CaratteristicheFisicoSanitarie ON Paziente.CF = CaratteristicheFisicoSanitarie.Paziente "
        "WHERE CaratteristicheFisicoSanitarie.Sesso = 'M' "
        "GROUP BY CaratteristicheFisicoSanitarie.Sesso;",
        
        "SELECT Malattia.NomeMalattia, COUNT(Referto.NumeroReferto) AS Numero_di_Referti "
        "FROM Malattia "
        "LEFT JOIN Referto ON Malattia.NomeMalattia = Referto.Malattia "
        "GROUP BY Malattia.NomeMalattia;",
        
        "SELECT MedicoDiBase.Nome, MedicoDiBase.Cognome, COUNT(Paziente.CF) AS Numero_di_Pazienti "
        "FROM MedicoDiBase "
        "JOIN Paziente ON MedicoDiBase.CF = Paziente.MedicoDiBase "
        "GROUP BY MedicoDiBase.Nome, MedicoDiBase.Cognome "
        "HAVING COUNT(Paziente.CF) > 0;",
        
        "SELECT Dottore.Nome, Dottore.Cognome, COUNT(Referto.NumeroReferto) AS Numero_di_Referti "
        "FROM Dottore "
        "JOIN Referto ON Dottore.CF = Referto.Dottore "
        "GROUP BY Dottore.Nome, Dottore.Cognome "
        "HAVING COUNT(Referto.NumeroReferto) > 0;"
    };

    int query_choice;
    int parametric;

    // Menu per selezionare la query
    printf("Seleziona una query:\n");
    for (int i = 0; i < sizeof(query_descriptions) / sizeof(query_descriptions[0]); i++) {
        printf("%d: %s\n", i + 1, query_descriptions[i]);
    }

    printf("Inserisci il numero della query che desideri eseguire: ");
    scanf("%d", &query_choice);

    if (query_choice < 1 || query_choice > sizeof(queries) / sizeof(queries[0])) {
        printf("Scelta non valida.\n");
        PQfinish(conn);
        exit(1);
    }

    printf("Desideri eseguire una query parametrica? (1: Si, 0: No): ");
    scanf("%d", &parametric);

    if (parametric == 1) {
        execute_parametric_query(conn, query_choice);
    } else {
        print_query_results(conn, queries[query_choice - 1]);
    }


    PQfinish(conn);
    return 0;
}
