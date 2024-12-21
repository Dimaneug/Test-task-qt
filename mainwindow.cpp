#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->listWidget, SIGNAL( itemChanged( QListWidgetItem * ) ), this, SLOT( changeTaskState( QListWidgetItem * ) ) );
    connect( ui->addButton, &QPushButton::clicked, this, &MainWindow::addTask );
    connect( ui->deleteButton, SIGNAL( clicked() ), this, SLOT( deleteTask() ) );
    connect( ui->saveButton, SIGNAL( clicked() ), this, SLOT( saveData() ) );
    connect( ui->loadButton, SIGNAL( clicked() ), this, SLOT( loadData() ) );
}

//----------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------
void MainWindow::addTask() {
    if( ui->lineEdit->text().isEmpty() ) {
        QMessageBox::warning( this, "Ошибка", "Вы не ввели задачу!" );
        return;
    }

    QListWidgetItem * item = new QListWidgetItem( ui->lineEdit->text() );
    item->setCheckState( Qt::Unchecked );

    ui->listWidget->addItem( item );

    ui->lineEdit->clear();
}

//----------------------------------------------------------------------
void MainWindow::deleteTask() {
    for( auto * item : ui->listWidget->selectedItems() ) {
        delete item;
    }
}

//----------------------------------------------------------------------
void MainWindow::changeTaskState( QListWidgetItem * item ) {
    QFont font = item->font();
    font.setStrikeOut( item->checkState() == Qt::Checked ? true : false );
    item->setFont( font );
}

//----------------------------------------------------------------------
void MainWindow::saveData() {
    QString filename = QFileDialog::getSaveFileName( this, "Сохранить файл" );

    QFile file( filename );

    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        QMessageBox::warning( this, "Ошибка", "Не удалось открыть файл для записи." );
        return;
    }

    QTextStream out( &file );

    for ( int i = 0; i < ui->listWidget->count(); ++i ) {
        QListWidgetItem* item = ui->listWidget->item( i );
        out << item->text() << ";" << ( item->checkState() == Qt::Checked ? 1 : 0 ) << "\n";
    }

    file.close();
    QMessageBox::information( this, "Успех", "Данные успешно сохранены в файл." );
}

//----------------------------------------------------------------------
void MainWindow::loadData() {
    QString filename = QFileDialog::getOpenFileName( this, "Загрузить файл" );

    QFile file( filename );

    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        QMessageBox::warning( this, "Ошибка", "Не удалось открыть файл для чтения." );
        return;
    }

    QTextStream in( &file );

    ui->listWidget->clear();

    while ( !in.atEnd() ) {
        QString line = in.readLine();
        QStringList parts = line.split( ";" );

        if ( parts.size() == 2 ) {
            QString text = parts[0];
            bool checked = parts[1].toInt();

            QListWidgetItem* item = new QListWidgetItem( text );
            item->setCheckState( checked ? Qt::Checked : Qt::Unchecked );
            changeTaskState( item );
            ui->listWidget->addItem( item );
        }
    }

    file.close();
    QMessageBox::information( this, "Успех", "Данные успешно загружены из файла." );
}
