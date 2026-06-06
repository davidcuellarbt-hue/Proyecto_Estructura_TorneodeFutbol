#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "form2.h"
#include "estructuras.h"
#include <QMainWindow>
#include "player.h"
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_agregarequipo_clicked();
    void on_salir_clicked();
    void on_registrojugadores_clicked();
    void on_actualizargoles_clicked();
    void mostrarTablaGoleadores();
    void mostrarJugadoresEquipo();

    void on_buscarname_clicked();

    void on_buscarnum_clicked();

    void on_actualizarequipos_2_clicked();

    void on_comboJornada_currentIndexChanged(int index);
    void on_comboPartido_currentIndexChanged(int index);

    void on_registrarResultado_clicked();

    void on_btnRegistrarResultado_clicked();

    void on_registrarGolLocal_clicked();

    void on_registrarGolVisitante_clicked();

    void on_btnGenerarEliminatorias_clicked();
    void on_comboFase_currentIndexChanged(int index);
    void on_comboPartidoElim_currentIndexChanged(int index);
    void on_registrarResultadoElim_clicked();
    void on_registrarGolLocalElim_clicked();
    void on_registrarGolVisitanteElim_clicked();

private:
    Ui::MainWindow *ui;
    Form2 *nuevaventana;
    void actualizarComboEquipos();
    void on_btnRegistrarJugador_clicked();
    Equipo* arbolEquipos = nullptr;
    Player *head = nullptr;
    std::vector<std::string> equipos;
    bool resultadoRegistrado = false;
    void prepararAsignacionGoles(int jornada, int partido);
    void actualizarComboGolesPendientes(int jornada, int partido);
    void refrescarBracket();
    void prepararAsignacionGolesElim(int fase, int partido);
    void actualizarComboGolesPendientesElim(int fase, int partido);
};

#endif