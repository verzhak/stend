
#ifndef VARIOUS_HPP
#define VARIOUS_HPP

#include "all.hpp"

// ############################################################################ 
// Вывод сообщений

void message(const QString msg);
void message_handler(QtMsgType type, const QMessageLogContext & context, const QString & msg);

// ############################################################################ 
// Выделение памяти

void * alloc(const unsigned type_size, const unsigned dim, ...);

#endif

