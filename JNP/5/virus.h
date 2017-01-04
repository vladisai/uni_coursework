#ifndef VIRUS_H
#define VIRUS_H

#include <string>

class Virus {
  public:
    typedef typename std::string id_type;
    Virus(id_type const &_id) : id(_id) {}
    id_type get_id() const { return id; }

  private:
    id_type id;
};

#endif // VIRUS_H
