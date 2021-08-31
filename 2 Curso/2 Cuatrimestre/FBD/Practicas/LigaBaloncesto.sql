CREATE TABLE Equipos(
codE VARCHAR2(10) PRIMARY KEY,
nombreE VARCHAR2(30) UNIQUE NOT NULL,
localidad VARCHAR2(30),
entrenador VARCHAR2(20),
fecha_crea DATE);

CREATE TABLE Jugadores(
codJ VARCHAR2(10) PRIMARY KEY,
codE NOT NULL REFERENCES Equipos(codE),
nombreJ VARCHAR2(30) NOT NULL);

CREATE TABLE Encuentros(
ELocal REFERENCES Equipos(codE),
EVisitante REFERENCES Equipos(codE),
PRIMARY KEY(ELocal,EVisitante),
fecha DATE,
PLocal INT DEFAULT 0 CHECK(PLocal>=0),
PVisitante INT DEFAULT 0 CHECK(PVisitante>=0));

CREATE TABLE Faltas(
codJ REFERENCES Jugadores(codJ),
ELocal VARCHAR2(10),
EVisitante VARCHAR2(10),
FOREIGN KEY(ELocal,EVisitante) REFERENCES Encuentros(ELocal,EVisitante),
PRIMARY KEY(codJ,ELocal,EVisitante),
numFaltas INT CHECK(numFaltas>=0 and numFaltas<6));

DESCRIBE Equipos;
DESCRIBE Jugadores;
DESCRIBE Encuentros;
DESCRIBE Faltas;

