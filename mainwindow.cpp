#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modbuscrccalculator.h"
#include "stpmcrccalculator.h"

#include <QMessageBox>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <QMenu>
#include <QContextMenuEvent>

using namespace std;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    createActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

using Bytes = vector<uint8_t>;

using StringAndBytes = pair<QString, Bytes>;

static void append_byte(StringAndBytes& dest, const QString& byteString)
{
    dest.first += byteString;
    dest.second.push_back((byteString.toShort(nullptr, 16)) & 0xFF);
}

static const StringAndBytes parse_input_string(const QString& input)
{
    StringAndBytes result;
    QString byte_string;
    for (const auto& symbol : input)
    {
        if (std::isxdigit(symbol.toLatin1()))
        {
            byte_string += symbol.toUpper();
            if (2 == byte_string.size())
            {
                byte_string.append(' ');
                append_byte(result, byte_string);
                byte_string.clear();
            }
        }
    }
    if (byte_string.isEmpty())
    {
        result.first.chop(1); // remove trailing space
    }
    else
    {
        // single character at the end
        byte_string.append('0');
        append_byte(result, byte_string);
    }
    return result;
}

const QString intToHex(const uint16_t value)
{
    return QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper();
}

const QString intToHex(const uint8_t value)
{
    return QString("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper();
}

static uint8_t reverseByte(uint8_t byte)
{
    byte = ((byte >> 1) & 0x55) | ((byte << 1) & 0xaa);
    byte = ((byte >> 2) & 0x33) | ((byte << 2) & 0xcc);
    byte = ((byte >> 4) & 0x0F) | ((byte << 4) & 0xF0);
    return byte;
}

void MainWindow::on_inputBox_returnPressed()
{
    const StringAndBytes string_and_bytes(parse_input_string(ui->inputBox->text()));
    ui->inputBox->setText(string_and_bytes.first);
    ui->inputBox->selectAll();
    ui->reversedBox->clear();
    QString reversed_box_text;
    unsigned bytes_count(0);
    unsigned bits_set(0);
    ModbusCrcCalculator modbus_crc_calculator;
    StpmCrcCalculator stpm_crc_calculator;
    uint8_t sum(0);
    for (const auto byte : string_and_bytes.second)
    {
        ++bytes_count;
        static const uint8_t bitcount[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        bits_set += bitcount[byte >> 4] + bitcount[byte & 0xF];
        modbus_crc_calculator(byte);
        stpm_crc_calculator(byte);
        sum += byte;
        reversed_box_text += intToHex(reverseByte(byte)) + ' ';
    }
    reversed_box_text.chop(1);
    ui->reversedBox->setText(reversed_box_text);
    ui->modbusCrcBox->setText(intToHex(modbus_crc_calculator.getResult()));
    ui->stpmCrcBox->setText(intToHex(stpm_crc_calculator.getResult()));
    ui->sumBox->setText(QString("%1").arg(sum, 1, 16).toUpper());
    if (string_and_bytes.second.size() >= sizeof(float))
    {
        const float float_value = *(reinterpret_cast<const float*>(&string_and_bytes.second[0]));
        ui->floatValueBox->setText(QString::fromStdString(std::to_string(float_value)));
        if (string_and_bytes.second.size() >= sizeof(double))
        {
            const double double_value =
                *(reinterpret_cast<const double*>(&string_and_bytes.second[0]));
            ui->floatValueBox->setText(QString::fromStdString(std::to_string(double_value)));
        }
    }
    ui->statusBar->showMessage(QString("Total bytes: %1(%2h), bits: %3/%4")
                                   .arg(bytes_count)
                                   .arg(bytes_count, 0, 16)
                                   .arg(bytes_count * 8)
                                   .arg(bits_set));
    //
    // QMessageBox::information(this, tr("My Application"), "Test");
}

void MainWindow::setHexInput()
{
    decInput = false;
}

void MainWindow::setDecInput()
{
    decInput = true;
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(setHexInputAction);
    menu.addAction(setDecInputAction);
    menu.exec(event->globalPos());
}

void MainWindow::createActions()
{
    setHexInputAction = new QAction(tr("&Hex"), this);
    setHexInputAction->setCheckable(true);
    //    setHexInputAction->setStatusTip(tr("Interpret input as hex bytes"));
    connect(setHexInputAction, &QAction::triggered, this, &MainWindow::setHexInput);

    setDecInputAction = new QAction(tr("&Dec"), this);
    setDecInputAction->setCheckable(true);
    //    setDecInputAction->setStatusTip(tr("Interpret input as dec bytes"));
    connect(setDecInputAction, &QAction::triggered, this, &MainWindow::setDecInput);

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(setHexInputAction);
    alignmentGroup->addAction(setDecInputAction);
    setHexInputAction->setChecked(true);
}
