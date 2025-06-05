#include "workspace.h"

Workspace::Workspace(QObject* parent) : QObject(parent) {}
Workspace::Workspace(QObject* parent, QJsonObject data) : QObject(parent) {
    m_pc = data.value("pc").toString();
    m_monitor = data.value("monitor").toString();
    m_tool = data.value("tool").toString();
    m_scanner = data.value("scanner").toString();
    m_tablet = data.value("tablet").toString();
    m_mouse = data.value("mouse").toString();
    m_printer = data.value("printer").toString();
    m_desktop = data.value("desktop").toString();
    m_music = data.value("music").toString();
    m_desk = data.value("desk").toString();
    m_chair = data.value("chair").toString();
    m_comment = data.value("comment").toString();
}
