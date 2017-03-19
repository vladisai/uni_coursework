#include "virus_genealogy.h"
#include <string>

class Virus {
public:
    typedef std::string id_type;
    Virus(id_type const &_id): id(_id) {
    }
    id_type get_id() const {
        return id;
}
private:
    id_type id;
};


// copy constructor test
#if TEST == 1
int main(void) {
    VirusGenealogy<Virus> Gene0("root");
    VirusGenealogy<Virus> Gene1(Gene0);
}
#endif

// copy assignment operator test
#if TEST == 2
int main(void) {
    VirusGenealogy<Virus> Gene0("root");
    VirusGenealogy<Virus> Gene1("root");

    Gene1 = Gene0;
}
#endif

// move assignment operator test
#if TEST == 3
int main(void) {
    VirusGenealogy<Virus> Gene0("root");
    VirusGenealogy<Virus> Gene1("root");

    Gene0 = std::move(Gene1);
}
#endif