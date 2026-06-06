#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QDirIterator>
#include <QScrollArea>
#include <QVBoxLayout>
#include "player.h"
#include <string>
#include <vector>
#include "playerlist.h"
#include "estructuras.h"
#include <QMessageBox>

using namespace  std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);


    connect(ui->combojornada, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboJornada_currentIndexChanged);

    connect(ui->combopartido, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboPartido_currentIndexChanged);

    connect(ui->comboBoxBusquedaEquipo,
            &QComboBox::currentTextChanged,
            this,
            &MainWindow::mostrarJugadoresEquipo);

    ui->tableEquipo->setColumnCount(5);

    QStringList headers1;
    headers1 << "Nombre" << "Dorsal" << "Posición" << "Goles" << "Equipo";

    ui->tableEquipo->setHorizontalHeaderLabels(headers1);

    ui->tableEquipo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEquipo->setSelectionBehavior(QAbstractItemView::SelectRows);


    ui->tableGoleadores->setColumnCount(5);

    QStringList headers;
    headers << "Nombre" << "Equipo" << "Dorsal" << "Posición" << "Goles";

    ui->tableGoleadores->setHorizontalHeaderLabels(headers);
    ui->tableGoleadores->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tabl_estad_equipos->setColumnCount(10);
    QStringList headers3;
    headers3 << "Pos" << "Equipo" << "PJ" << "G" << "E" << "P" << "GF" << "GC" << "DG" << "Pts";
    ui->tabl_estad_equipos->setHorizontalHeaderLabels(headers3);
    ui->tabl_estad_equipos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabl_estad_equipos->setSelectionBehavior(QAbstractItemView::SelectRows);


    for (int i = 1; i <= 19; i++) {
        ui->combojornada->addItem("Jornada " + QString::number(i));
    }

    for (int i = 1; i <= 10; i++) {
        ui->combopartido->addItem("Partido " + QString::number(i));
    }

    ui->comboFase->addItem("Cuartos de Final");
    ui->comboFase->addItem("Semifinales");
    ui->comboFase->addItem("Final");

    connect(ui->comboFase, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboFase_currentIndexChanged);
    connect(ui->comboPartidoElim, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboPartidoElim_currentIndexChanged);

}

void MainWindow::actualizarComboEquipos(){

    ui->comboBoxEquipos->clear();
     ui->comboBoxBusquedaEquipo->clear();

    for(const string &eq : equipos){
        ui->comboBoxEquipos->addItem(QString::fromStdString(eq));
        ui->comboBoxBusquedaEquipo->addItem(QString::fromStdString(eq));
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (contarEquipos(arbolEquipos) > 0 || head != nullptr) {
        QMessageBox::warning(this, "Error",
                             "Ya hay datos registrados. Reinicia la app para cargar de nuevo.");
        return;
    }

    // 1. Cargar 20 equipos + 220 jugadores
    registrarEquiposPrueba(arbolEquipos, head);

    // 2. Asignar IDs y generar calendario
    int contador = 0;
    asignarIDs(arbolEquipos, contador);
    generarCalendario(arbolEquipos);

    // 3. REGISTRAR TODOS LOS RESULTADOS DE LAS 19 JORNADAS
    registrarResultadosPrueba(arbolEquipos, head);

    // 4. Actualizar UI
    actualizarComboEquipos();
    on_actualizarequipos_2_clicked();
    on_actualizargoles_clicked();

    QMessageBox::information(this, "Carga Completa",
                             "• 20 equipos registrados\n"
                             "• 220 jugadores registrados\n"
                             "• 19 jornadas completadas (190 partidos)\n"
                             "• Tabla de posiciones y goleadores actualizadas\n\n"
                             "Ahora puedes generar las eliminatorias.");
}


void MainWindow::on_pushButton_2_clicked()
{
    nuevaventana = new Form2(this);
    nuevaventana->show();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{

}


void MainWindow::on_agregarequipo_clicked()
{

    QString nombreQt = ui->lineEditEquipo->text().trimmed();

    if (nombreQt.isEmpty()) {
        QMessageBox::warning(this, "Error", "El nombre del equipo no puede estar vacío.");
        return;
    }

    if (contarEquipos(arbolEquipos) >= 20) {
        QMessageBox::warning(this, "Error", "Límite de 20 equipos alcanzado.");
        return;
    }

    std::string nombreStd = nombreQt.toStdString();

    if (buscarEquipoPorNombre(arbolEquipos, nombreStd) != nullptr) {
        QMessageBox::warning(this, "Error", "Ya existe un equipo con ese nombre.");
        return;
    }

    insertarEquipo(arbolEquipos, nombreStd);
    QMessageBox::information(this, "Éxito",
                             "Equipo \"" + nombreQt + "\" registrado.\nTotal: " +
                                 QString::number(contarEquipos(arbolEquipos)) + "/20");

    QString equipo = ui->lineEditEquipo->text();

    if(equipo.isEmpty()){
        return;
    }

    equipos.push_back(equipo.toStdString());

    ui->lineEditEquipo->clear();

    actualizarComboEquipos();

    actualizarComboEquipos();

    if (contarEquipos(arbolEquipos) == 20 && !calendarioGenerado()) {
        generarCalendario(arbolEquipos);
        QMessageBox::information(this, "¡Calendario Generado!",
                                 "Se han generado las 19 jornadas con 10 partidos cada una.\n"
                                 "Ahora puedes registrar resultados.");
    }

}


void MainWindow::on_salir_clicked()
{
    QApplication::quit();
}


void MainWindow::on_registrojugadores_clicked()
{

    QString nombre = ui->lineEditNombre->text();
    QString equipo = ui->comboBoxEquipos->currentText();
    int numero = ui->spinBoxNumero->value();
    QString posicion = ui->lineEditPosicion->text();

    if(nombre.isEmpty() || equipo.isEmpty() || posicion.isEmpty()){
        return;
    }

    insertarplayer(
        head,
        nombre.toStdString(),
        numero,
        equipo.toStdString(),
        posicion.toStdString(),
        0
        );

    ui->lineEditNombre->clear();
    ui->lineEditPosicion->clear();
}

void MainWindow::mostrarTablaGoleadores(){

    PlayerData arr[250];
    int n = cargarJugadores(head, arr);

    mergeSort(arr, 0, n - 1);

    ui->tableGoleadores->setRowCount(n);

    for(int i = 0; i < n; i++){
        ui->tableGoleadores->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(arr[i].name)));
        ui->tableGoleadores->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(arr[i].team)));
        ui->tableGoleadores->setItem(i, 2, new QTableWidgetItem(QString::number(arr[i].number)));
        ui->tableGoleadores->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(arr[i].position)));
        ui->tableGoleadores->setItem(i, 4, new QTableWidgetItem(QString::number(arr[i].gol)));
    }
}


void MainWindow::on_actualizargoles_clicked()
{

    ui->comboBoxEquipos->clear();
    ui->comboBoxBusquedaEquipo->clear();

    Equipo* arreglo[100];
    int contador = 0;
    extraerEquipos(arbolEquipos, arreglo, contador);

    for (int i = 0; i < contador; i++) {
        QString nombre = QString::fromStdString(arreglo[i]->nombre);
        ui->comboBoxEquipos->addItem(nombre);
        ui->comboBoxBusquedaEquipo->addItem(nombre);
    }
    mostrarTablaGoleadores();
}

void MainWindow::mostrarJugadoresEquipo(){

    QString equipoQt = ui->comboBoxBusquedaEquipo->currentText();
    if (equipoQt.isEmpty()) return;

    std::string equipo = equipoQt.toStdString();


    int n = contarJugadoresEquipo(head, equipo);

    ui->tableEquipo->setRowCount(0);
    if (n == 0) return;

    PlayerData *arr = new PlayerData[n];
    copiarJugadoresEquipo(head, equipo, arr);


    ui->tableEquipo->setRowCount(n);
    for (int i = 0; i < n; i++) {
        ui->tableEquipo->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(arr[i].name)));
        ui->tableEquipo->setItem(i, 1, new QTableWidgetItem(QString::number(arr[i].number)));
        ui->tableEquipo->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(arr[i].position)));
        ui->tableEquipo->setItem(i, 3, new QTableWidgetItem(QString::number(arr[i].gol)));
        ui->tableEquipo->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(arr[i].team)));
    }

    delete[] arr;
}


void MainWindow::on_buscarnum_clicked(){

    string equipo = ui->comboBoxBusquedaEquipo->currentText().toStdString();

    bool ok;
    int dorsal = ui->lineEditDorsal->text().toInt(&ok);

    if(!ok){
        return;
    }

    PlayerData *arr = new PlayerData[100];

    int n = construirEquipo(head, equipo, arr);

    sort(arr, arr + n, [](PlayerData a, PlayerData b){
        return a.number < b.number;
    });

    int idx = busquedaBinariaDorsal(arr, n, dorsal);

    ui->tableEquipo->setRowCount(0);

    if(idx == -1){
        delete[] arr;
        return;
    }

    ui->tableEquipo->setRowCount(1);

    ui->tableEquipo->setItem(0,0,new QTableWidgetItem(QString::fromStdString(arr[idx].name)));
    ui->tableEquipo->setItem(0,1,new QTableWidgetItem(QString::number(arr[idx].number)));
    ui->tableEquipo->setItem(0,2,new QTableWidgetItem(QString::fromStdString(arr[idx].position)));
    ui->tableEquipo->setItem(0,3,new QTableWidgetItem(QString::number(arr[idx].gol)));
    ui->tableEquipo->setItem(0,4,new QTableWidgetItem(QString::fromStdString(arr[idx].team)));

    delete[] arr;
}



void MainWindow::on_buscarname_clicked()
{
        string equipo = ui->comboBoxBusquedaEquipo->currentText().toStdString();

        string nombre = ui->lineEditNombreBuscar->text().toStdString();

        PlayerData *arr = new PlayerData[100];

        int n = construirEquipo(head, equipo, arr);

        int idx = busquedaNombre(arr, n, nombre);

        ui->tableEquipo->setRowCount(0);

        if(idx == -1){
            delete[] arr;
            return;
        }

        ui->tableEquipo->setRowCount(1);

        ui->tableEquipo->setItem(0,0,new QTableWidgetItem(QString::fromStdString(arr[idx].name)));
        ui->tableEquipo->setItem(0,1,new QTableWidgetItem(QString::number(arr[idx].number)));
        ui->tableEquipo->setItem(0,2,new QTableWidgetItem(QString::fromStdString(arr[idx].position)));
        ui->tableEquipo->setItem(0,3,new QTableWidgetItem(QString::number(arr[idx].gol)));
        ui->tableEquipo->setItem(0,4,new QTableWidgetItem(QString::fromStdString(arr[idx].team)));

        delete[] arr;

}


void MainWindow::on_actualizarequipos_2_clicked()
{

    Equipo* arreglo[100];
    int total = obtenerTablaPosiciones(arbolEquipos, arreglo);

    ui->tabl_estad_equipos->setRowCount(total);

    for (int i = 0; i < total; i++) {
        ui->tabl_estad_equipos->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tabl_estad_equipos->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(arreglo[i]->nombre)));
        ui->tabl_estad_equipos->setItem(i, 2, new QTableWidgetItem(QString::number(arreglo[i]->pj)));
        ui->tabl_estad_equipos->setItem(i, 3, new QTableWidgetItem(QString::number(arreglo[i]->ganados)));
        ui->tabl_estad_equipos->setItem(i, 4, new QTableWidgetItem(QString::number(arreglo[i]->empatados)));
        ui->tabl_estad_equipos->setItem(i, 5, new QTableWidgetItem(QString::number(arreglo[i]->perdidos)));
        ui->tabl_estad_equipos->setItem(i, 6, new QTableWidgetItem(QString::number(arreglo[i]->goles_favor)));
        ui->tabl_estad_equipos->setItem(i, 7, new QTableWidgetItem(QString::number(arreglo[i]->goles_contra)));
        ui->tabl_estad_equipos->setItem(i, 8, new QTableWidgetItem(QString::number(arreglo[i]->diferencia_goles)));
        ui->tabl_estad_equipos->setItem(i, 9, new QTableWidgetItem(QString::number(arreglo[i]->puntos)));
    }

}

void MainWindow::on_comboPartido_currentIndexChanged(int index)
{
    on_comboJornada_currentIndexChanged(ui->combojornada->currentIndex());
}


void MainWindow::on_comboJornada_currentIndexChanged(int index)
{
    if (!calendarioGenerado()) return;

    int jornada = index;
    int partido = ui->combopartido->currentIndex();

    if (partido < 0) return;

    // Mostrar equipos
    int idLocal = calendario[jornada].partidos[partido].id_local;
    int idVisitante = calendario[jornada].partidos[partido].id_visitante;

    ui->labelEquipoLocal->setText(QString::fromStdString(nombreEquipoPorID(arbolEquipos, idLocal)));
    ui->labelEquipoVisitante->setText(QString::fromStdString(nombreEquipoPorID(arbolEquipos, idVisitante)));

    // NUEVO: Verificar si ya fue jugado
    if (partidoJugado(jornada, partido)) {
        // Mostrar resultado guardado y bloquear
        int gl = calendario[jornada].partidos[partido].goles_local;
        int gv = calendario[jornada].partidos[partido].goles_visitante;

        ui->lineEditGolesLocal->setText(QString::number(gl));
        ui->lineEditGolesVisitante->setText(QString::number(gv));
        ui->lineEditGolesLocal->setEnabled(false);
        ui->lineEditGolesVisitante->setEnabled(false);
        ui->btnRegistrarResultado->setEnabled(false);

    } else {
        // Partido nuevo: limpiar y habilitar
        ui->lineEditGolesLocal->clear();
        ui->lineEditGolesVisitante->clear();
        ui->lineEditGolesLocal->setEnabled(true);
        ui->lineEditGolesVisitante->setEnabled(true);
        ui->btnRegistrarResultado->setEnabled(true);
    }
}

void MainWindow::on_registrarResultado_clicked()
{
    int jornada = ui->combojornada->currentIndex();
    int partido = ui->combopartido->currentIndex();


    if (partidoJugado(jornada, partido)) {
        QMessageBox::warning(this, "Error", "Este partido ya fue registrado. No se puede modificar.");
        return;
    }


    bool okLocal, okVisitante;
    int golesLocal = ui->lineEditGolesLocal->text().toInt(&okLocal);
    int golesVisitante = ui->lineEditGolesVisitante->text().toInt(&okVisitante);

    if (!okLocal || !okVisitante || golesLocal < 0 || golesVisitante < 0) {
        QMessageBox::warning(this, "Error", "Ingresa números válidos de goles (0 o más).");
        return;
    }

    bool exito = registrarResultado(arbolEquipos, jornada, partido, golesLocal, golesVisitante);

    if (exito) {
        QMessageBox::information(this, "Éxito",
                                 "Resultado registrado: " + ui->labelEquipoLocal->text() + " " +
                                     QString::number(golesLocal) + " - " +
                                     QString::number(golesVisitante) + " " +
                                     ui->labelEquipoVisitante->text());


        ui->lineEditGolesLocal->setEnabled(false);
        ui->lineEditGolesVisitante->setEnabled(false);
        ui->btnRegistrarResultado->setEnabled(false);


        on_actualizarequipos_2_clicked();

    } else {
        QMessageBox::warning(this, "Error", "No se pudo registrar el resultado.");
    }
}


void MainWindow::on_btnRegistrarResultado_clicked()
{
    int jornada = ui->combojornada->currentIndex();
    int partido = ui->combopartido->currentIndex();

    if (partidoJugado(jornada, partido)) {
        QMessageBox::warning(this, "Error",
                             "Este partido ya fue registrado. No se puede modificar.");
        return;
    }

    bool okLocal, okVisitante;
    int golesLocal = ui->lineEditGolesLocal->text().toInt(&okLocal);
    int golesVisitante = ui->lineEditGolesVisitante->text().toInt(&okVisitante);

    if (!okLocal || !okVisitante || golesLocal < 0 || golesVisitante < 0) {
        QMessageBox::warning(this, "Error",
                             "Ingresa números válidos de goles (0 o más).");
        return;
    }

    bool exito = registrarResultado(arbolEquipos, jornada, partido, golesLocal, golesVisitante);

    if (exito) {

        QString msg = "Resultado registrado:\n" +
                      ui->labelEquipoLocal->text() + " " + QString::number(golesLocal) +
                      " - " +
                      QString::number(golesVisitante) + " " + ui->labelEquipoVisitante->text();

        QMessageBox::information(this, "Éxito", msg);

        ui->lineEditGolesLocal->setEnabled(false);
        ui->lineEditGolesVisitante->setEnabled(false);
        ui->btnRegistrarResultado->setEnabled(false);

        on_actualizarequipos_2_clicked();
        prepararAsignacionGoles(jornada, partido);

    } else {
        QMessageBox::warning(this, "Error", "No se pudo registrar el resultado.");
    }
}

void MainWindow::prepararAsignacionGoles(int jornada, int partido)
{
    int idLocal = calendario[jornada].partidos[partido].id_local;
    int idVisitante = calendario[jornada].partidos[partido].id_visitante;
    std::string nombreLocal = nombreEquipoPorID(arbolEquipos, idLocal);
    std::string nombreVisitante = nombreEquipoPorID(arbolEquipos, idVisitante);


    ui->comboJugadorLocal->clear();
    ui->comboJugadorVisitante->clear();

    Player *aux = head;
    while (aux != nullptr) {
        if (aux->team == nombreLocal) {
            ui->comboJugadorLocal->addItem(QString::fromStdString(aux->name));
        }
        if (aux->team == nombreVisitante) {
            ui->comboJugadorVisitante->addItem(QString::fromStdString(aux->name));
        }
        aux = aux->next;
    }


    actualizarComboGolesPendientes(jornada, partido);

    bool hayLocal = (calendario[jornada].partidos[partido].goles_local > 0);
    bool hayVisitante = (calendario[jornada].partidos[partido].goles_visitante > 0);

    ui->comboGolLocal->setEnabled(hayLocal);
    ui->comboJugadorLocal->setEnabled(hayLocal);
    ui->registrarGolLocal->setEnabled(hayLocal && puedeAsignarGolLocal(jornada, partido));

    ui->comboGolVisitante->setEnabled(hayVisitante);
    ui->comboJugadorVisitante->setEnabled(hayVisitante);
    ui->registrarGolVisitante->setEnabled(hayVisitante && puedeAsignarGolVisitante(jornada, partido));
}

void MainWindow::actualizarComboGolesPendientes(int jornada, int partido)
{
    Partido &p = calendario[jornada].partidos[partido];

    ui->comboGolLocal->clear();
    for (int i = 0; i < p.goles_local; i++) {
        if (p.autorGolLocal[i].empty()) {
            ui->comboGolLocal->addItem("Gol " + QString::number(i + 1), i);
        }
    }

    ui->comboGolVisitante->clear();
    for (int i = 0; i < p.goles_visitante; i++) {
        if (p.autorGolVisitante[i].empty()) {
            ui->comboGolVisitante->addItem("Gol " + QString::number(i + 1), i);
        }
    }

    ui->registrarGolLocal->setEnabled(ui->comboGolLocal->count() > 0);
    ui->registrarGolVisitante->setEnabled(ui->comboGolVisitante->count() > 0);
}

void MainWindow::on_registrarGolLocal_clicked()
{
    int jornada = ui->combojornada->currentIndex();
    int partido = ui->combopartido->currentIndex();

    if (!puedeAsignarGolLocal(jornada, partido)) {
        QMessageBox::warning(this, "Error", "No hay goles locales pendientes por asignar.");
        return;
    }

    int numGol = ui->comboGolLocal->currentData().toInt();
    QString nombreJugadorQt = ui->comboJugadorLocal->currentText();

    if (nombreJugadorQt.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona un jugador.");
        return;
    }

    int idLocal = calendario[jornada].partidos[partido].id_local;
    std::string equipo = nombreEquipoPorID(arbolEquipos, idLocal);
    std::string nombreJugador = nombreJugadorQt.toStdString();

    // Validar que el jugador exista en ese equipo
    if (buscarJugador(head, nombreJugador, equipo) == nullptr) {
        QMessageBox::warning(this, "Error", "El jugador no pertenece al equipo local.");
        return;
    }

    bool exito = asignarGolAJugador(jornada, partido, true, numGol, head, nombreJugador, equipo);

    if (exito) {
        QMessageBox::information(this, "Éxito",
                                 "Gol " + QString::number(numGol + 1) + " asignado a " + nombreJugadorQt);

        actualizarComboGolesPendientes(jornada, partido);
        on_actualizargoles_clicked();

    } else {
        QMessageBox::warning(this, "Error", "Ese gol ya fue asignado o no existe.");
    }
}

void MainWindow::on_registrarGolVisitante_clicked()
{
    int jornada = ui->combojornada->currentIndex();
    int partido = ui->combopartido->currentIndex();

    if (!puedeAsignarGolVisitante(jornada, partido)) {
        QMessageBox::warning(this, "Error", "No hay goles visitantes pendientes por asignar.");
        return;
    }

    int numGol = ui->comboGolVisitante->currentData().toInt();
    QString nombreJugadorQt = ui->comboJugadorVisitante->currentText();

    if (nombreJugadorQt.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona un jugador.");
        return;
    }

    int idVisitante = calendario[jornada].partidos[partido].id_visitante;
    std::string equipo = nombreEquipoPorID(arbolEquipos, idVisitante);
    std::string nombreJugador = nombreJugadorQt.toStdString();

    if (buscarJugador(head, nombreJugador, equipo) == nullptr) {
        QMessageBox::warning(this, "Error", "El jugador no pertenece al equipo visitante.");
        return;
    }

    bool exito = asignarGolAJugador(jornada, partido, false, numGol, head, nombreJugador, equipo);

    if (exito) {
        QMessageBox::information(this, "Éxito",
                                 "Gol " + QString::number(numGol + 1) + " asignado a " + nombreJugadorQt);

        actualizarComboGolesPendientes(jornada, partido);
        on_actualizargoles_clicked();

    } else {
        QMessageBox::warning(this, "Error", "Ese gol ya fue asignado o no existe.");
    }
}


void MainWindow::on_btnGenerarEliminatorias_clicked()
{
    if (!calendarioGenerado()) {
        QMessageBox::warning(this, "Error", "Primero termina la fase regular.");
        return;
    }

    bool ok = generarEliminatorias(arbolEquipos);
    if (!ok) {
        QMessageBox::warning(this, "Error", "No se pudieron generar eliminatorias (¿ya iniciadas o faltan equipos?).");
        return;
    }

    refrescarBracket();
    QMessageBox::information(this, "Eliminatorias", "¡Cuartos de final generados! Top 8 clasificados.");

    ui->comboFase->setCurrentIndex(0);
    on_comboFase_currentIndexChanged(0);
}

void MainWindow::on_comboFase_currentIndexChanged(int index)
{
    ui->comboPartidoElim->clear();

    int maxPartidos = 0;
    if (index == 0) maxPartidos = 4;
    else if (index == 1) maxPartidos = 2;
    else if (index == 2) maxPartidos = 1;

    for (int i = 1; i <= maxPartidos; i++) {
        ui->comboPartidoElim->addItem("Partido " + QString::number(i));
    }

    on_comboPartidoElim_currentIndexChanged(0);
}

void MainWindow::on_comboPartidoElim_currentIndexChanged(int index)
{
    int fase = ui->comboFase->currentIndex();
    int partido = index;

    if (fase < 0 || partido < 0) return;

    // Mostrar equipos
    QString local = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, fase, partido, true));
    QString visitante = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, fase, partido, false));

    ui->labelEquipoLocalElim->setText(local);
    ui->labelEquipoVisitanteElim->setText(visitante);

    // Verificar si ya fue jugado
    if (partidoEliminatoriaJugado(fase, partido)) {
        // Obtener goles guardados
        int gl = 0, gv = 0;
        if (fase == 0) { gl = eliminatoriasData.cuartos[partido].goles_local; gv = eliminatoriasData.cuartos[partido].goles_visitante; }
        else if (fase == 1) { gl = eliminatoriasData.semis[partido].goles_local; gv = eliminatoriasData.semis[partido].goles_visitante; }
        else if (fase == 2) { gl = eliminatoriasData.final.goles_local; gv = eliminatoriasData.final.goles_visitante; }

        ui->lineEditGolesLocalElim->setText(QString::number(gl));
        ui->lineEditGolesVisitanteElim->setText(QString::number(gv));
        ui->lineEditGolesLocalElim->setEnabled(false);
        ui->lineEditGolesVisitanteElim->setEnabled(false);
        ui->registrarResultadoElim->setEnabled(false);

        prepararAsignacionGolesElim(fase, partido);
    } else {
        ui->lineEditGolesLocalElim->clear();
        ui->lineEditGolesVisitanteElim->clear();
        ui->lineEditGolesLocalElim->setEnabled(true);
        ui->lineEditGolesVisitanteElim->setEnabled(true);
        ui->registrarResultadoElim->setEnabled(true);

        // Deshabilitar goles hasta que haya resultado
        ui->comboGolLocalElim->setEnabled(false);
        ui->comboJugadorLocalElim->setEnabled(false);
        ui->registrarGolLocalElim->setEnabled(false);
        ui->comboGolVisitanteElim->setEnabled(false);
        ui->comboJugadorVisitanteElim->setEnabled(false);
        ui->registrarGolVisitanteElim->setEnabled(false);
    }
}

void MainWindow::on_registrarResultadoElim_clicked()
{
    int fase = ui->comboFase->currentIndex();
    int partido = ui->comboPartidoElim->currentIndex();

    if (partidoEliminatoriaJugado(fase, partido)) {
        QMessageBox::warning(this, "Error", "Este partido ya fue registrado.");
        return;
    }

    bool okL, okV;
    int gl = ui->lineEditGolesLocalElim->text().toInt(&okL);
    int gv = ui->lineEditGolesVisitanteElim->text().toInt(&okV);

    if (!okL || !okV || gl < 0 || gv < 0) {
        QMessageBox::warning(this, "Error", "Ingresa goles válidos.");
        return;
    }

    bool exito = registrarResultadoEliminatoria(arbolEquipos, fase, partido, gl, gv);
    if (!exito) {
        QMessageBox::warning(this, "Error", "No se pudo registrar. ¿Empate? Las eliminatorias no permiten empates.");
        return;
    }

    QMessageBox::information(this, "Éxito", "Resultado registrado.");

    // Bloquear
    ui->lineEditGolesLocalElim->setEnabled(false);
    ui->lineEditGolesVisitanteElim->setEnabled(false);
    ui->registrarResultadoElim->setEnabled(false);

    // Preparar goles
    prepararAsignacionGolesElim(fase, partido);

    // Avanzar fase y refrescar bracket
    int avance = avanzarFaseEliminatoria();
    refrescarBracket();

    if (avance == 1) QMessageBox::information(this, "¡Semifinales!", "Se han generado las semifinales.");
    if (avance == 2) QMessageBox::information(this, "¡Final!", "Se ha generado la final.");
    if (avance == 3) {
        QString campeon = QString::fromStdString(nombreCampeon(arbolEquipos));
        PlayerData bota = obtenerBotaOro(head);
        QMessageBox::information(this, "¡CAMPEÓN!", "Campeón: " + campeon);
        ui->labelCampeon->setText(campeon);
        ui->labelBotaOro->setText(QString::fromStdString(bota.name));
    }
}

void MainWindow::prepararAsignacionGolesElim(int fase, int partido)
{
    std::string nombreLocal = ui->labelEquipoLocalElim->text().toStdString();
    std::string nombreVisitante = ui->labelEquipoVisitanteElim->text().toStdString();

    ui->comboJugadorLocalElim->clear();
    ui->comboJugadorVisitanteElim->clear();

    if (head != nullptr) {
        Player *aux = head;
        while (aux != nullptr) {
            if (aux->team == nombreLocal) ui->comboJugadorLocalElim->addItem(QString::fromStdString(aux->name));
            if (aux->team == nombreVisitante) ui->comboJugadorVisitanteElim->addItem(QString::fromStdString(aux->name));
            aux = aux->next;
        }
    }

    actualizarComboGolesPendientesElim(fase, partido);

    bool hayLocal = (fase == 0 ? eliminatoriasData.cuartos[partido].goles_local :
                         (fase == 1 ? eliminatoriasData.semis[partido].goles_local :
                              eliminatoriasData.final.goles_local)) > 0;

    bool hayVisitante = (fase == 0 ? eliminatoriasData.cuartos[partido].goles_visitante :
                             (fase == 1 ? eliminatoriasData.semis[partido].goles_visitante :
                                  eliminatoriasData.final.goles_visitante)) > 0;

    ui->comboGolLocalElim->setEnabled(hayLocal && ui->comboGolLocalElim->count() > 0);
    ui->comboJugadorLocalElim->setEnabled(hayLocal && ui->comboJugadorLocalElim->count() > 0);
    ui->registrarGolLocalElim->setEnabled(hayLocal && puedeAsignarGolLocalElim(fase, partido) && ui->comboJugadorLocalElim->count() > 0);

    ui->comboGolVisitanteElim->setEnabled(hayVisitante && ui->comboGolVisitanteElim->count() > 0);
    ui->comboJugadorVisitanteElim->setEnabled(hayVisitante && ui->comboJugadorVisitanteElim->count() > 0);
    ui->registrarGolVisitanteElim->setEnabled(hayVisitante && puedeAsignarGolVisitanteElim(fase, partido) && ui->comboJugadorVisitanteElim->count() > 0);
}

void MainWindow::actualizarComboGolesPendientesElim(int fase, int partido)
{
    PartidoEliminatoria* p = nullptr;
    if (fase == 0 && partido < 4) p = &eliminatoriasData.cuartos[partido];
    else if (fase == 1 && partido < 2) p = &eliminatoriasData.semis[partido];
    else if (fase == 2 && partido == 0) p = &eliminatoriasData.final;
    if (!p) return;

    ui->comboGolLocalElim->clear();
    for (int i = 0; i < p->goles_local; i++) {
        if (p->autorGolLocal[i].empty()) ui->comboGolLocalElim->addItem("Gol " + QString::number(i + 1), i);
    }

    ui->comboGolVisitanteElim->clear();
    for (int i = 0; i < p->goles_visitante; i++) {
        if (p->autorGolVisitante[i].empty()) ui->comboGolVisitanteElim->addItem("Gol " + QString::number(i + 1), i);
    }

    ui->registrarGolLocalElim->setEnabled(ui->comboGolLocalElim->count() > 0);
    ui->registrarGolVisitanteElim->setEnabled(ui->comboGolVisitanteElim->count() > 0);
}

void MainWindow::on_registrarGolLocalElim_clicked()
{
    int fase = ui->comboFase->currentIndex();
    int partido = ui->comboPartidoElim->currentIndex();

    if (!puedeAsignarGolLocalElim(fase, partido)) {
        QMessageBox::warning(this, "Error", "No hay goles locales pendientes.");
        return;
    }
    if (ui->comboGolLocalElim->count() == 0) return;

    int numGol = ui->comboGolLocalElim->currentData().toInt();
    QString nombreJugadorQt = ui->comboJugadorLocalElim->currentText();
    if (nombreJugadorQt.isEmpty()) return;

    std::string equipo = ui->labelEquipoLocalElim->text().toStdString();
    std::string nombreJugador = nombreJugadorQt.toStdString();

    if (buscarJugador(head, nombreJugador, equipo) == nullptr) {
        QMessageBox::warning(this, "Error", "Jugador no encontrado.");
        return;
    }

    bool exito = asignarGolAJugadorElim(fase, partido, true, numGol, head, nombreJugador, equipo);
    if (exito) {
        QMessageBox::information(this, "Éxito", "Gol asignado a " + nombreJugadorQt);
        actualizarComboGolesPendientesElim(fase, partido);
        on_actualizargoles_clicked();
    } else {
        QMessageBox::warning(this, "Error", "Ese gol ya fue asignado.");
    }
}

void MainWindow::on_registrarGolVisitanteElim_clicked()
{
    int fase = ui->comboFase->currentIndex();
    int partido = ui->comboPartidoElim->currentIndex();

    if (!puedeAsignarGolVisitanteElim(fase, partido)) {
        QMessageBox::warning(this, "Error", "No hay goles visitantes pendientes.");
        return;
    }
    if (ui->comboGolVisitanteElim->count() == 0) return;

    int numGol = ui->comboGolVisitanteElim->currentData().toInt();
    QString nombreJugadorQt = ui->comboJugadorVisitanteElim->currentText();
    if (nombreJugadorQt.isEmpty()) return;

    std::string equipo = ui->labelEquipoVisitanteElim->text().toStdString();
    std::string nombreJugador = nombreJugadorQt.toStdString();

    if (buscarJugador(head, nombreJugador, equipo) == nullptr) {
        QMessageBox::warning(this, "Error", "Jugador no encontrado.");
        return;
    }

    bool exito = asignarGolAJugadorElim(fase, partido, false, numGol, head, nombreJugador, equipo);
    if (exito) {
        QMessageBox::information(this, "Éxito", "Gol asignado a " + nombreJugadorQt);
        actualizarComboGolesPendientesElim(fase, partido);
        on_actualizargoles_clicked();
    } else {
        QMessageBox::warning(this, "Error", "Ese gol ya fue asignado.");
    }
}

void MainWindow::refrescarBracket()
{
    // CUARTOS (8 labels)
    for (int i = 0; i < 4; i++) {
        QString l = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 0, i, true));
        QString v = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 0, i, false));
        // Adapta estos nombres a tus labels reales:
        if (i == 0) { ui->labelCuartos1_1->setText(l); ui->labelCuartos1_2->setText(v); }
        if (i == 1) { ui->labelCuartos2_1->setText(l); ui->labelCuartos2_2->setText(v); }
        if (i == 2) { ui->labelCuartos3_1->setText(l); ui->labelCuartos3_2->setText(v); }
        if (i == 3) { ui->labelCuartos4_1->setText(l); ui->labelCuartos4_2->setText(v); }
    }

    // SEMIS (4 labels)
    for (int i = 0; i < 2; i++) {
        QString l = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 1, i, true));
        QString v = QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 1, i, false));
        if (i == 0) { ui->labelSemis1_1->setText(l); ui->labelSemis1_2->setText(v); }
        if (i == 1) { ui->labelSemis2_1->setText(l); ui->labelSemis2_2->setText(v); }
    }

    // FINAL (2 labels)
    ui->labelFinal1->setText(QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 2, 0, true)));
    ui->labelFinal2->setText(QString::fromStdString(nombreEquipoEliminatoria(arbolEquipos, 2, 0, false)));

    // CAMPEÓN (si ya hay)
    if (eliminatoriasData.final.jugado) {
        ui->labelCampeon->setText(QString::fromStdString(nombreCampeon(arbolEquipos)));
        PlayerData bota = obtenerBotaOro(head);
        ui->labelBotaOro->setText(QString::fromStdString(bota.name));
    }
}

