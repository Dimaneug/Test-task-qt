#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// Добавление задачи
    void addTask();
    /// Удаление задачи
    void deleteTask();
    /// Изменяет состояние задачи
    void changeTaskState( QListWidgetItem * item );
    /// Сохранение задач в файл
    void saveData();
    /// Загрузка задач из файла
    void loadData();

private:
    /// Объект ui
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
