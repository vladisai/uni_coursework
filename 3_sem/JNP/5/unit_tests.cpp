#include <cassert>
#include <vector>
#include <algorithm>

#include "virus_genealogy.h"


class Virus {
public:
	typedef std::string id_type;
	Virus(id_type const &_id) : id(_id) {
	}
	id_type get_id() const {
		return id;
	}
private:
	id_type id;
};

template<typename _id_type>
class _Virus {
public:
    typedef _id_type id_type;
    _Virus(id_type const &_id): id(_id) {
    }
    id_type get_id() const {
        return id;
    }
private:
    id_type id;
};

std::vector<std::string> _sort(std::vector<std::string> vec) {
    std::sort(vec.begin(), vec.end());
    return vec;
}

void test_z_tresci() {
	VirusGenealogy<Virus> gen("A1H1");
	Virus::id_type const id1 = gen.get_stem_id();
	assert(gen.exists(id1));
	assert(gen.get_parents(id1).size() == 0);
	gen.create("A", id1);
	gen.create("B", id1);
	assert(gen.get_children(id1).size() == 2);
	gen.create("C", "A");
	gen.connect("C", "B");
	assert(gen.get_parents("C").size() == 2);
	assert(gen.get_children("A").size() == 1);
	std::vector<Virus::id_type> parents;
	parents.push_back("A");
	parents.push_back("B");
	gen.create("D", parents);
	assert(gen.get_parents("D").size() == parents.size());
	assert(gen.get_children("A").size() == 2);
	assert("D" == gen["D"].get_id());
	gen.remove("A");
	assert(!gen.exists("A"));
	assert(gen.exists("B"));
	assert(gen.exists("C"));
	assert(gen.exists("D"));
	gen.remove("B");
	assert(!gen.exists("A"));
	assert(!gen.exists("B"));
	assert(!gen.exists("C"));
	assert(!gen.exists("D"));
	try {
		gen["E"];
	}
	catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
	}
	try {
		gen.create("E", "A1H1");
		gen.create("E", "A1H1");
	}
	catch (std::exception &e) {
        assert(e.what() == "VirusAlreadyCreated");
	}
	try {
		gen.remove("A1H1");
	}
	catch (std::exception &e) {
		assert(e.what() == "TriedToRemoveStemVirus");
	}
}

void test_konstruktora() {
    VirusGenealogy<Virus> Gene0("root");
    VirusGenealogy<_Virus<double>> Gene1(0.0000001);
    VirusGenealogy<_Virus<std::pair<int, std::string>>> Gene2(std::make_pair(1, "q"));

    assert(Gene0.exists("root") && Gene0.get_children("root").size() == 0 && Gene0.get_parents("root").size() == 0);
    assert(Gene1.exists(0.0000001) && Gene1.get_children(0.0000001).size() == 0 && Gene1.get_parents(0.0000001).size() == 0);
    assert(Gene2.exists(std::make_pair(1, "q")) && Gene2.get_children(std::make_pair(1, "q")).size() == 0 && Gene2.get_parents(std::make_pair(1, "q")).size() == 0);
}

void test_get_stem_id() {
    VirusGenealogy<Virus> Gene0("root");
    assert(Gene0.get_stem_id() == "root");

    VirusGenealogy<_Virus<double>> Gene1(0.0000001);
    assert(Gene1.get_stem_id() == 0.0000001);

    VirusGenealogy<_Virus<std::pair<int, std::string>>> Gene2(std::make_pair(1, "q"));
    assert(Gene2.get_stem_id() == std::make_pair(1, std::string("q")));
}

void test_get_children() {
    //      -> 1  -
    // root -> 1. -|-> 2
    //      -> 1, -
    //            ---> 2. -> 3
    //            ---> 2,
    //      -----------^
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("1.", "root");
    Gene.create("1,", "root");
    Gene.create("2", "1");
    Gene.connect("2", "1.");
    Gene.connect("2", "1,");
    Gene.create("2.", "1,");
    Gene.create("2,", "1,");
    Gene.connect("2,", "root");
    Gene.create("3", "2.");

    assert(_sort(Gene.get_children("root")) == _sort({"1", "1.", "1,", "2,"}));
    assert(_sort(Gene.get_children("1")) == _sort({"2"}));
    assert(_sort(Gene.get_children("1.")) == _sort({"2"}));
    assert(_sort(Gene.get_children("1,")) == _sort({"2", "2.", "2,"}));
    assert(_sort(Gene.get_children("2")) == _sort({}));
    assert(_sort(Gene.get_children("2.")) == _sort({"3"}));
    assert(_sort(Gene.get_children("2,")) == _sort({}));
    assert(_sort(Gene.get_children("3")) == _sort({}));

    bool exception_occured = false;
    try {
        Gene.get_children("r00t");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}

void test_get_parents() {
    //      -> 1  -
    // root -> 1. -|-> 2
    //      -> 1, -
    //            ---> 2. -> 3
    //            ---> 2,
    //      -----------^
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("1.", "root");
    Gene.create("1,", "root");
    Gene.create("2", "1");
    Gene.connect("2", "1.");
    Gene.connect("2", "1,");
    Gene.create("2.", "1,");
    Gene.create("2,", "1,");
    Gene.connect("2,", "root");
    Gene.create("3", "2.");

    assert(_sort(Gene.get_parents("root")) == _sort({}));
    assert(_sort(Gene.get_parents("1")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("1.")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("1,")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("2")) == _sort({"1", "1.", "1,"}));
    assert(_sort(Gene.get_parents("2.")) == _sort({"1,"}));
    assert(_sort(Gene.get_parents("2,")) == _sort({"1,", "root"}));
    assert(_sort(Gene.get_parents("3")) == _sort({"2."}));

    bool exception_occured = false;
    try {
        Gene.get_parents("r00t");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}

void test_exists() {
    // root -> 1 -> 2  -> 3
    //           -> 2. ---^
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("2", "1");
    Gene.create("2.", "1");
    Gene.create("3", "2");
    Gene.connect("3", "2.");

    assert(Gene.exists("root"));
    assert(Gene.exists("1"));
    assert(Gene.exists("2"));
    assert(Gene.exists("2."));
    assert(Gene.exists("3"));
    assert(!Gene.exists("r00t"));

    // root -> 1 -> 2. -> 3
    Gene.remove("2");

    assert(Gene.exists("root"));
    assert(Gene.exists("1"));
    assert(!Gene.exists("2"));
    assert(Gene.exists("2."));
    assert(Gene.exists("3"));
    assert(!Gene.exists("r00t"));

    // root -> 1
    Gene.remove("2.");
    
    assert(Gene.exists("root"));
    assert(Gene.exists("1"));
    assert(!Gene.exists("2"));
    assert(!Gene.exists("2."));
    assert(!Gene.exists("3"));
    assert(!Gene.exists("r00t"));
}

void test_access() {
    //      -> 1
    // root -> 1. -> 2 -> 3
    //      ---------^
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("1.", "root");
    Gene.create("2", "1.");
    Gene.connect("2", "root");
    Gene.create("3", "2");

    assert(Gene["root"].get_id() == "root");
    assert(Gene["1"].get_id() == "1");
    assert(Gene["1."].get_id() == "1.");
    assert(Gene["2"].get_id() == "2");
    assert(Gene["3"].get_id() == "3");

    //      -> 1
    // root -> 2 -> 3
    Gene.remove("1.");

    assert(Gene["root"].get_id() == "root");
    assert(Gene["1"].get_id() == "1");
    assert(Gene["2"].get_id() == "2");
    assert(Gene["3"].get_id() == "3");

    // root -> 2
    Gene.remove("1");
    Gene.remove("3");

    assert(Gene["root"].get_id() == "root");
    assert(Gene["2"].get_id() == "2");

    bool exception_occured = false;
    try {
        Gene["1"];
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}

void test_create_single() {
    // root -> 1  -> 2
    //      -> 1. -> 2.
    //            -> 2,
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("1.", "root");
    Gene.create("2", "1");
    Gene.create("2.", "1.");
    Gene.create("2,", "1.");

    assert(_sort(Gene.get_parents("root")) == _sort({}));
    assert(_sort(Gene.get_parents("1")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("1.")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("2")) == _sort({"1"}));
    assert(_sort(Gene.get_parents("2.")) == _sort({"1."}));
    assert(_sort(Gene.get_parents("2,")) == _sort({"1."}));

    assert(_sort(Gene.get_children("root")) == _sort({"1", "1."}));
    assert(_sort(Gene.get_children("1")) == _sort({"2"}));
    assert(_sort(Gene.get_children("1.")) == _sort({"2.", "2,"}));
    assert(_sort(Gene.get_children("2")) == _sort({}));
    assert(_sort(Gene.get_children("2.")) == _sort({}));
    assert(_sort(Gene.get_children("2,")) == _sort({}));

    bool exception_occured = false;
    try {
        Gene.create("root", "root");
    } catch (std::exception &e) {
        assert(e.what() == "VirusAlreadyCreated");
        exception_occured = true;
    }
    assert(exception_occured);

    exception_occured = false;
    try {
        Gene.create("42", "3");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}

void test_create_many() {
    //            ---------. 
    // root -> 1  -
    //      -> 1. -|-> 2 -> 3
    //      -> 1, -
    //            ----------^
    //      ----------------^
    // ~~~~~~~~~~~~~~~~~~
    //  4
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", std::vector<std::string>{"root"});
    Gene.create("1.", std::vector<std::string>{"root"});
    Gene.create("1,", std::vector<std::string>{"root"});
    Gene.create("2", std::vector<std::string>{"1", "1.", "1,"});
    Gene.create("3", std::vector<std::string>{"root", "1", "1,", "2"});

    assert(_sort(Gene.get_parents("root")) == _sort({}));
    assert(_sort(Gene.get_parents("1")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("1.")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("1,")) == _sort({"root"}));
    assert(_sort(Gene.get_parents("2")) == _sort({"1", "1.", "1,"}));
    assert(_sort(Gene.get_parents("3")) == _sort({"root", "1", "1,", "2"}));

    assert(_sort(Gene.get_children("root")) == _sort({"1", "1.", "1,", "3"}));
    assert(_sort(Gene.get_children("1")) == _sort({"2", "3"}));
    assert(_sort(Gene.get_children("1.")) == _sort({"2"}));
    assert(_sort(Gene.get_children("1,")) == _sort({"2", "3"}));
    assert(_sort(Gene.get_children("2")) == _sort({"3"}));
    assert(_sort(Gene.get_children("3")) == _sort({}));


    bool exception_occured = false;
    try {
        Gene.create("root", "root");
    } catch (std::exception &e) {
        assert(e.what() == "VirusAlreadyCreated");
        exception_occured = true;
    }
    assert(exception_occured);

    exception_occured = false;
    try {
        Gene.create("42", std::vector<std::string>{"root", "1", "1,", "2", "42"});
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);

    exception_occured = false;
    try {
        Gene.create("4", std::vector<std::string>{});
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
    assert(!Gene.exists("4"));
}

void test_connect() {
    // root -> 1 -> 2
    //      --------^
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("2", "1");
    Gene.connect("2", "root");
    Gene.connect("2", "root"); // double connect
    
    auto vec = Gene.get_children("root");
    assert(std::find(vec.begin(), vec.end(), "2") != vec.end()); // vec contains "2"

    vec = Gene.get_parents("2");
    assert(std::find(vec.begin(), vec.end(), "root") != vec.end()); // vec contains "root"

    bool exception_occured = false;
    try {
        Gene.connect("r00t", "root");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);

    exception_occured = false;
    try {
        Gene.connect("root", "r00t");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}

void test_remove() {
    //      -> 1 -> 2 -> 3
    // root    |--> 2.
    //      -> 1.
    VirusGenealogy<Virus> Gene("root");
    Gene.create("1", "root");
    Gene.create("1.", "root");
    Gene.create("2", "1");
    Gene.connect("2", "1"); // double connection - it shoud be removed anyway
    Gene.create("2.", "1");
    Gene.connect("2.", "1.");
    Gene.create("3", "2");
    Gene.remove("1");

    // root -> 1. -> 2.
    assert(Gene.exists("root"));
    assert(Gene.exists("1."));
    assert(Gene.exists("2."));
    assert(!Gene.exists("2"));
    assert(!Gene.exists("2"));
    assert(!Gene.exists("3"));

    bool exception_occured = false;
    try {
        Gene.remove("root");
    }
    catch (std::exception &e) {
        assert(e.what() == "TriedToRemoveStemVirus");
        exception_occured = true;
    }
    assert(exception_occured);

    exception_occured = false;
    try {
        Gene.remove("1");
    } catch (std::exception &e) {
        assert(e.what() == "VirusNotFound");
        exception_occured = true;
    }
    assert(exception_occured);
}


int main() {
    test_z_tresci();
    test_konstruktora();
    test_get_stem_id();
    test_get_children();
    test_get_parents();
    test_exists();
    test_access();
    test_create_single();
    test_create_many();
    test_connect();
    test_remove();

    return 0;
}