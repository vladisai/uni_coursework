#include <vector>
#include <string>

#include "virus_genealogy.h"

const int TEST_SIZE = 10'000;

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

std::vector<std::string> reversed_vector(std::string names) {
    std::vector<std::string> ret;
    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        ret.push_back(names + std::to_string(i));
    }
    return ret;
}

void create_children(VirusGenealogy<Virus>& Gene, std::string virus, std::string names) {
    for (int i = TEST_SIZE - 1; i >= 0; i--) {
        Gene.create(names + std::to_string(i), virus);
    }
}

void create_path(VirusGenealogy<Virus>& Gene, std::string virus, std::string names) {
    Gene.create(names + std::to_string(0), virus);
    std::string prev = names + std::to_string(0);
    for (int i = 1; i < TEST_SIZE; i++) {
        Gene.create(names + std::to_string(i), virus);
        std::string prev = names + std::to_string(i);
    }
}

void create_big_tree(VirusGenealogy<Virus>& Gene) {
    create_children(Gene, "root", "spread ");
    create_path(Gene, "root", "line ");
}

void testp_connect() {
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene.connect("1", "root");
    }
}

void testp_get_children() {
    VirusGenealogy<Virus> Gene("root");
    create_children(Gene, "root", "virus ");

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene.get_children("root");
    }
}

void testp_get_parents() {
    VirusGenealogy<Virus> Gene("root");
    create_children(Gene, "root", "virus ");
    Gene.create("Mutant", reversed_vector("virus "));

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene.get_parents("Mutant");
    }
}


void testp_exists_non_existing() {
    VirusGenealogy<Virus> Gene("root");
    create_big_tree(Gene);

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene.exists("r00t");
    }
}

void testp_exists() {
    VirusGenealogy<Virus> Gene("root");
    create_big_tree(Gene);

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene.exists("spread " + std::to_string(i));
        Gene.exists("line " + std::to_string(i));
        Gene.exists("root");
    }
}

void testp_access() {
    VirusGenealogy<Virus> Gene("root");
    create_big_tree(Gene);

    for (int i = 0; i < TEST_SIZE; i++) {
        Gene["spread " + std::to_string(i)];
        Gene["line " + std::to_string(i)];
        Gene["root"];
    }
}

void testp_create() {
    VirusGenealogy<Virus> Gene("root");
    create_children(Gene, "root", "virus ");
    auto vec = reversed_vector("virus ");
    int sqroot = sqrt(TEST_SIZE);

    for (int i = 0; i < sqroot; i++) {
        Gene.create("Mutant " + std::to_string(i), vec);
    }
}

void testp_remove() {
    VirusGenealogy<Virus> Gene("root");
    create_children(Gene, "root", "virus ");
    create_path(Gene, "root", "_virus ");
    auto vec = reversed_vector("virus ");
    int sqroot = sqrt(TEST_SIZE);

    for (int i = 0; i < sqroot; i++) {
        Gene.create("Mutant " + std::to_string(i), vec);
    }

    for (int i = 0; i < sqroot; i++) {
        Gene.remove("Mutant " + std::to_string(i));
        Gene.remove("_virus " + std::to_string(i));
        Gene.remove("virus " + std::to_string(i));
    }

    for (int i = sqroot; i < TEST_SIZE; i++) {
        Gene.remove("_virus " + std::to_string(i));
        Gene.remove("virus " + std::to_string(i));
    }
}

int main(void) {
    testp_connect();
    testp_get_children();
    testp_get_parents();
    testp_exists_non_existing();
    testp_exists();
    testp_access();
    testp_create();
    testp_remove();

    return 0;
}
