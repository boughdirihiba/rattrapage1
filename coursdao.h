#ifndef COURSDAO_H
#define COURSDAO_H

#include <QSqlDatabase>
#include "cours.h"

class CoursDAO {
public:
    bool ajouter(const Cours& c);
};

#endif // COURSDAO_H
