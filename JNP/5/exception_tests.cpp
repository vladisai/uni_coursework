#include "virus_genealogy.h"
#include <bits/stdc++.h>
#include <cassert>

using namespace std;

static bool throw_get_id = false;
static bool throw_constructor = false;
static bool throw_id_type = false;

class string2 : std::string {
  public:
    std::string name;

    void throw_() const {
        if (throw_id_type == true) {
            throw 1;
        }
    }

    bool operator<(const string2 x) const {
        throw_();
        return this->name < x.name;
    }

    bool operator>(const string2 x) const {
        throw_();
        return this->name > x.name;
    }

    bool operator!=(const string2 x) const {
        throw_();
        return this->name != x.name;
    }

    bool operator==(const string2 x) const {
        throw_();
        return this->name == x.name;
    }

    bool operator<=(const string2 x) const {
        throw_();
        return this->name <= x.name;
    }

    bool operator>=(const string2 x) const {
        throw_();
        return this->name >= x.name;
    }

    string2(const string2 &x) { this->name = x.name; }

    string2() {}

    string2(std::string x) { this->name = x; }
};

class Virus__ {
  public:
    typedef string2 id_type;
    Virus__(id_type const &_id) : id(_id) {
        if (throw_constructor) {
            throw 1;
        }
    }
    id_type get_id() const {
        if (throw_get_id) {
            throw 1;
        }
        return id;
    }

  private:
    id_type id;
};

void set_to_not_throw() {
    throw_get_id = false;
    throw_constructor = false;
    throw_id_type = false;
}

void set_to_throw() {
    throw_get_id = true;
    throw_constructor = true;
    throw_id_type = true;
}

void teste_constructor() {
    set_to_not_throw();

    bool exception_occured = false;
    try {
        VirusGenealogy<Virus__> Gene1(string2("root"));
    } catch (...) {
        exception_occured = true;
    }
    assert(exception_occured);
}

void teste_get_stem_id() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    set_to_throw();

    try {
        Gene0.get_stem_id();
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.get_stem_id() == string2("root"));
        assert(Gene0.get_children(string2("root")).size() == 0);
        assert(Gene0.get_parents(string2("root")).size() == 0);
    }
}

void teste_get_children() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), string2("root"));
    set_to_throw();

    try {
        Gene0.get_children(string2("root"));
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.get_children(string2("root")).size() == 3);
        assert(Gene0.get_parents(string2("root")).size() == 0);
        assert(Gene0.exists(string2("root")));
    }
}

void teste_get_parents() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), {string2("root"), string2("2"), string2("1")});
    set_to_throw();

    try {
        Gene0.get_parents(string2("3"));
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.get_parents(string2("3")).size() == 3);
        assert(Gene0.get_children(string2("3")).size() == 0);
        assert(Gene0.exists(string2("3")));
    }
}

void teste_exists() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), string2("1"));
    set_to_throw();

    try {
        Gene0.exists(string2("3"));
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.get_parents(string2("3")).size() == 1);
        assert(Gene0.get_children(string2("3")).size() == 0);
        assert(Gene0.exists(string2("3")));
    }
}

void teste_access() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), string2("1"));
    set_to_throw();

    try {
        Gene0[string2("3")];
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.get_parents(string2("3")).size() == 1);
        assert(Gene0.get_children(string2("3")).size() == 0);
        assert(Gene0.exists(string2("3")));
    }
}

void teste_create_single() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), string2("1"));
    set_to_throw();

    try {
        Gene0.create(string2("4"), string2("2"));
    } catch (...) {
        set_to_not_throw();
        assert(!Gene0.exists(string2("4")));
        assert(Gene0.get_children(string2("2")).size() == 0);
    }
}

void teste_create_many() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("root"));
    Gene0.create(string2("3"), string2("1"));
    set_to_throw();

    try {
        Gene0.create(string2("4"), {string2("1"), string2("2"), string2("3")});
    } catch (...) {
        set_to_not_throw();
        assert(!Gene0.exists(string2("4")));
        assert(Gene0.get_children(string2("2")).size() == 0);
        assert(Gene0.get_children(string2("3")).size() == 0);

        auto vec = Gene0.get_children(string2("1"));
        assert(vec.size() == 1);
        assert(std::find(vec.begin(), vec.end(), string2("3")) != vec.end());
    }
}

void teste_connect() {
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("2"), string2("1"));

    set_to_throw();

    try {
        Gene0.connect(string2("2"), string2("root"));
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.exists(string2("2")));
        assert(Gene0.exists(string2("root")));

        auto vec = Gene0.get_children(string2("root"));
        assert(vec.size() == 1);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());

        vec = Gene0.get_parents(string2("2"));
        assert(vec.size() == 1);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());
    }

    set_to_not_throw();
    Gene0.connect(string2("2"), string2("root"));
    set_to_throw();

    try {
        Gene0.connect(string2("2"), string2("root")); // double connection
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.exists(string2("2")));
        assert(Gene0.exists(string2("root")));

        auto vec = Gene0.get_children(string2("root"));
        assert(vec.size() == 2);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());
        assert(std::find(vec.begin(), vec.end(), string2("2")) != vec.end());

        vec = Gene0.get_parents(string2("2"));
        assert(vec.size() == 2);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());
        assert(std::find(vec.begin(), vec.end(), string2("root")) != vec.end());
    }
}

void teste_remove() {
    //      -> 1 -> 2 -> 3
    // root    |--> 2.
    //      -> 1.
    set_to_not_throw();
    VirusGenealogy<Virus__> Gene0(string2("root"));
    Gene0.create(string2("1"), string2("root"));
    Gene0.create(string2("1."), string2("root"));
    Gene0.create(string2("2"), string2("1"));
    Gene0.create(string2("2."), string2("1"));
    Gene0.connect(string2("2."), string2("1."));
    Gene0.create(string2("3"), string2("2"));
    set_to_throw();

    try {
        Gene0.remove(string2("1"));
    } catch (...) {
        set_to_not_throw();
        assert(Gene0.exists(string2("root")));
        assert(Gene0.exists(string2("1")));
        assert(Gene0.exists(string2("1.")));
        assert(Gene0.exists(string2("2")));
        assert(Gene0.exists(string2("2.")));
        assert(Gene0.exists(string2("3")));

        auto vec = Gene0.get_children(string2("root"));
        assert(vec.size() == 2);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());
        assert(std::find(vec.begin(), vec.end(), string2("1.")) != vec.end());

        vec = Gene0.get_parents(string2("2"));
        assert(vec.size() == 1);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());

        vec = Gene0.get_parents(string2("2."));
        assert(vec.size() == 2);
        assert(std::find(vec.begin(), vec.end(), string2("1")) != vec.end());
        assert(std::find(vec.begin(), vec.end(), string2("1.")) != vec.end());

        vec = Gene0.get_parents(string2("1"));
        assert(vec.size() == 1);
        assert(std::find(vec.begin(), vec.end(), string2("root")) != vec.end());

        vec = Gene0.get_children(string2("1"));
        assert(vec.size() == 2);
        assert(std::find(vec.begin(), vec.end(), string2("2")) != vec.end());
        assert(std::find(vec.begin(), vec.end(), string2("2.")) != vec.end());
    }
}

int main(void) {
    // teste_constructor();
    teste_get_stem_id();
    teste_get_children();
    teste_get_parents();
    teste_exists();
    teste_access();
    teste_create_single();
    teste_create_many();
    teste_connect();
    teste_remove();

    return 0;
}
