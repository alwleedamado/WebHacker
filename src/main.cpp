#include "Analyzer.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <fstream>

int
main (int argc, char **argv)
{
  QApplication app (argc, argv);
  QMainWindow win;
  QWidget central;
  QVBoxLayout layout;
  QPushButton open ("Open File"), analyze ("Analyze"), exportBtn ("Export JSON");
  QTextEdit editor;
  QTableWidget table;
  layout.addWidget (&open);
  layout.addWidget (&analyze);
  layout.addWidget (&exportBtn);
  layout.addWidget (&editor);
  layout.addWidget (&table);
  central.setLayout (&layout);
  win.setCentralWidget (&central);
  Analyzer analyzer;
  std::string cur_path;
  QObject::connect (&open, &QPushButton::clicked, [&] ()
                      {
    QString file = QFileDialog::getOpenFileName(&win, "Open TS/JS file");
    if(file.isEmpty()) return;
    cur_path = file.toStdString();
    std::ifstream ifs(cur_path);
    std::stringstream ss; ss<<ifs.rdbuf();
    editor.setPlainText(QString::fromStdString(ss.str())); });
  QObject::connect (&analyze, &QPushButton::clicked, [&] ()
                      {
    std::string src = editor.toPlainText().toStdString();
    auto report = analyzer.analyze_source(src, cur_path.empty() ? "pasted" : cur_path);
    table.clear();
    table.setColumnCount(3);
    table.setRowCount(report.diagnostics.size());
    table.setHorizontalHeaderLabels({"Rule","Message","Line"});
    for (int i=0;i<report.diagnostics.size();++i){
      table.setItem(i,0,new QTableWidgetItem(QString::fromStdString(report.diagnostics[i].rule)));
      table.setItem(i,1,new QTableWidgetItem(QString::fromStdString(report.diagnostics[i].message)));
      table.setItem(i,2,new QTableWidgetItem(QString::number(report.diagnostics[i].line)));
    } });
  QObject::connect (&exportBtn, &QPushButton::clicked, [&] ()
                      {
    std::string src = editor.toPlainText().toStdString();
    auto report = analyzer.analyze_source(src, cur_path.empty() ? "pasted" : cur_path);
    nlohmann::json j = report.to_json();
    QString file = QFileDialog::getSaveFileName(&win, "Save Report", "report.json");
    if(!file.isEmpty()){
      std::ofstream ofs(file.toStdString());
      ofs<<j.dump(2);
    } });
  win.show ();
  return app.exec ();
}

