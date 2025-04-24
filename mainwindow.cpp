#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , current_state(9, 0)
    , finished(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buttons.append(ui->pushButton_tic1);
    buttons.append(ui->pushButton_tic2);
    buttons.append(ui->pushButton_tic3);
    buttons.append(ui->pushButton_tic4);
    buttons.append(ui->pushButton_tic5);
    buttons.append(ui->pushButton_tic6);
    buttons.append(ui->pushButton_tic7);
    buttons.append(ui->pushButton_tic8);
    buttons.append(ui->pushButton_tic9);
    ui->comboBox_first->addItem("Computer");
    ui->comboBox_first->addItem("Me");
    ui->comboBox_first->setCurrentIndex(1);
    current_player = Player::O;
    connect(ui->comboBox_first, &QComboBox::currentIndexChanged, this, &MainWindow::comboBox_first_currentIndexChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_tic1_clicked()
{
    next_step(ui->pushButton_tic1, 0);
}


void MainWindow::on_pushButton_tic2_clicked()
{
    next_step(ui->pushButton_tic2, 1);
}


void MainWindow::on_pushButton_tic3_clicked()
{
    next_step(ui->pushButton_tic3, 2);
}


void MainWindow::on_pushButton_tic4_clicked()
{
    next_step(ui->pushButton_tic4, 3);
}


void MainWindow::on_pushButton_tic5_clicked()
{
    next_step(ui->pushButton_tic5, 4);
}


void MainWindow::on_pushButton_tic6_clicked()
{
    next_step(ui->pushButton_tic6, 5);
}


void MainWindow::on_pushButton_tic7_clicked()
{
    next_step(ui->pushButton_tic7, 6);
}


void MainWindow::on_pushButton_tic8_clicked()
{
    next_step(ui->pushButton_tic8, 7);
}


void MainWindow::on_pushButton_tic9_clicked()
{
    next_step(ui->pushButton_tic9, 8);
}

void MainWindow::next_step(QPushButton *button, int index)
{
    if (current_state[index] == 0 && !finished) {
        switch (current_player) {
        case Player::O:
            button->setText("O");
            current_player = Player::X;
            current_state[index] = 1;
            break;
        case Player::X:
            button->setText("X");
            current_player = Player::O;
            current_state[index] = -1;
            break;
        }
        Winner winner = who_wins(current_state);
        switch (winner) {
        case Winner::O:
            ui->statusbar->showMessage("Player O Wins!");
            finished = true;
            break;
        case Winner::X:
            ui->statusbar->showMessage("Player X Wins!");
            finished = true;
            break;
        case Winner::DRAW:
            ui->statusbar->showMessage("Draw!");
            finished = true;
            break;
        case Winner::NONE:
            // use robot
            if (current_player == Player::X) {
                player_bot();
            }
            break;
        }
    }
}

int MainWindow::get_state_value(QList<int> &state, Player next_player)
{
    switch (who_wins(state)) {
    case Winner::O:
        return 3;
    case Winner::X:
        return -3;
    case Winner::DRAW:
        return 0;
    default:
        break;
    }
    switch (next_player) {
    case Player::O:
        return max_value(state);
    case Player::X:
        return min_value(state);
    }
    return -100;
}

int MainWindow::max_value(QList<int> &state)
{
    int v = -10;
    for (auto&& var : state) {
        if (var == 0) {
            steps++;
            var = 1;
            v = std::max(v, get_state_value(state, Player::X));
            var = 0;
        }
    }
    return v;
}

int MainWindow::min_value(QList<int> &state)
{
    int v = 10;
    for (auto&& var : state) {
        if (var == 0) {
            steps++;
            var = -1;
            v = std::min(v, get_state_value(state, Player::O));
            var = 0;
        }
    }
    return v;
}

int MainWindow::get_state_value_fast(QList<int> &state, Player next_player, int alpha, int beta)
{
    switch (who_wins(state)) {
    case Winner::O:
        return 3;
    case Winner::X:
        return -3;
    case Winner::DRAW:
        return 0;
    default:
        break;
    }
    switch (next_player) {
    case Player::O:
        return max_value_fast(state, alpha, beta);
    case Player::X:
        return min_value_fast(state, alpha, beta);
    }
    return -100;
}

int MainWindow::max_value_fast(QList<int> &state, int alpha, int beta)
{
    int v = -10;
    for (auto&& var : state) {
        if (var == 0) {
            steps++;
            var = 1;
            v = std::max(v, get_state_value_fast(state, Player::X, alpha, beta));
            var = 0;
            if (v >= beta){
                // qDebug() << "return!";
                return v;
            }
            alpha = std::max(alpha, v);
        }
    }
    return v;
}

int MainWindow::min_value_fast(QList<int> &state, int alpha, int beta)
{
    int v = 10;
    for (auto&& var : state) {
        if (var == 0) {
            steps++;
            var = -1;
            v = std::min(v, get_state_value_fast(state, Player::O, alpha, beta));
            var = 0;
            if (v <= alpha) {
                // qDebug() << "return!";
                return v;
            }
            beta = std::min(beta, v);
        }
    }
    return v;
}

void MainWindow::player_bot()
{
    int v = 10;
    QPushButton* choice = nullptr;
    auto state = current_state;
    for (int i = 0; i < state.length(); i++) {
        if (state[i] == 0) {
            choice = buttons[i];
        }
    }
    QList<std::pair<int, int>> move_rate;
    int all_steps = 0;
    for (int i = 0; i < state.length(); i++) {
        if (state[i] == 0) {
            state[i] = -1;
            int next_value;
            steps = 0;
            if (ui->checkBox_fast->isChecked()) {
                next_value = get_state_value_fast(state, Player::O, -10, 10);
                qDebug() << "Use alpha-beta cut off: " << steps;
                all_steps += steps;
            }else{
                next_value = get_state_value(state, Player::O);
                qDebug() << "Just minimax: " << steps;
                all_steps += steps;
            }
            move_rate.append({i, next_value});
            if (next_value < v){
                choice = buttons[i];
                v = next_value;
            }
            state[i] = 0;
        }
    }
    ui->label_steps->setText(QString::number(all_steps * 1.0 / state.length()));
    qDebug() << move_rate;
    if (choice){
        choice->animateClick();
    }
}

MainWindow::Winner MainWindow::who_wins(QList<int>& state)
{
    static const int winPatterns[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // rows
        {0,3,6}, {1,4,7}, {2,5,8}, // cols
        {0,4,8}, {2,4,6}           // diags
    };

    for (auto& pattern : winPatterns) {
        int sum = state[pattern[0]] + state[pattern[1]] + state[pattern[2]];
        if (sum == 3) return Winner::O;
        if (sum == -3) return Winner::X;
    }
    for (auto&& i: state){
        if (i == 0){
            return Winner::NONE;
        }
    }
    return Winner::DRAW;
}


void MainWindow::on_pushButton_restart_clicked()
{
    current_state.fill(0);
    finished = false;
    ui->statusbar->clearMessage();
    ui->label_steps->clear();
    for (auto&& button : buttons) {
        button->setText("");
    }
    switch (ui->comboBox_first->currentIndex()) {
    case 0:
        current_player = Player::X;
        player_bot();
        break;
    case 1:
        current_player = Player::O;
        break;
    }
}


void MainWindow::comboBox_first_currentIndexChanged()
{
    on_pushButton_restart_clicked();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

