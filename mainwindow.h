#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_tic1_clicked();

    void on_pushButton_tic2_clicked();

    void on_pushButton_tic3_clicked();

    void on_pushButton_tic4_clicked();

    void on_pushButton_tic5_clicked();

    void on_pushButton_tic6_clicked();

    void on_pushButton_tic7_clicked();

    void on_pushButton_tic8_clicked();

    void on_pushButton_tic9_clicked();

    void on_pushButton_restart_clicked();

    void comboBox_first_currentIndexChanged();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QList<int> current_state;
    void next_step(QPushButton* button, int index);
    QList<QPushButton*> buttons;
    enum class Player{
        O,
        X,
    };
    enum class Winner{
        O,
        X,
        NONE,
        DRAW,
    };

    Player current_player;
    Winner who_wins(QList<int>& state);
    bool finished;
    int get_state_value(QList<int>& state, Player next_player);
    int max_value(QList<int>& state);
    int min_value(QList<int>& state);
    int get_state_value_fast(QList<int>& state, Player next_player, int alpha, int beta);
    int max_value_fast(QList<int>& state, int alpha, int beta);
    int min_value_fast(QList<int>& state, int alpha, int beta);
    void player_bot();
    int steps;
};
#endif // MAINWINDOW_H
