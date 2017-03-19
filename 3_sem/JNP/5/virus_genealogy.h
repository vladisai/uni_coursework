#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>
#include <iostream>  // remove

using namespace std; // remove

using std::vector;
using std::shared_ptr;
using std::set;
using std::make_shared;

class VirusNotFound : public std::exception {
  public:
    char const *what() const throw() { return "VirusNotFound"; }
};

class VirusAlreadyCreated : public std::exception {
  public:
    char const *what() const throw() { return "VirusAlreadyCreated"; }
};

class TriedToRemoveStemVirus : public std::exception {
  public:
    char const *what() const throw() { return "TriedToRemoveStemVirus"; }
};

template <class Virus> class VirusGenealogy {
  public:
    typedef typename Virus::id_type virus_id_type;

    VirusGenealogy(virus_id_type const &stem_id) : stem_id(stem_id) {
        viruses.insert({stem_id, make_shared<Virus>(stem_id)});
    }

    // Usunięcie domyślnych konstruktorów i operatorów przypisania
    VirusGenealogy() = delete;

    VirusGenealogy(const VirusGenealogy &) = delete;

    VirusGenealogy(VirusGenealogy &&) = delete;

    VirusGenealogy &operator=(const VirusGenealogy &) = delete;

    VirusGenealogy &operator=(VirusGenealogy &&) = delete;

    // Zwraca identyfikator wirusa macierzystego.
    virus_id_type get_stem_id() const { return stem_id; }

    // Zwraca listę identyfikatorów bezpośrednich następników wirusa
    // o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
    std::vector<virus_id_type> get_children(virus_id_type const &id) const {
        if (!exists(id)) {
            throw VirusNotFound();
        } else {
            auto it = children.find(id);
            if (it == children.end()) {
                return vector<virus_id_type>();
            } else {
                return vector<virus_id_type>(it->second.begin(),
                                             it->second.end());
            }
        }
    }

    // Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
    // o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
    std::vector<virus_id_type> get_parents(virus_id_type const &id) const {
        if (!exists(id)) {
            throw VirusNotFound();
        } else {
            auto it = parents.find(id);
            if (it == parents.end()) {
                return vector<virus_id_type>();
            } else {
                return vector<virus_id_type>(it->second.begin(),
                                             it->second.end());
            }
        }
    }

    // Sprawdza, czy wirus o podanym identyfikatorze istnieje.
    bool exists(const virus_id_type &id) const {
        return (viruses.count(id) > 0);
    }

    // Zwraca referencję do obiektu reprezentującego wirus o podanym
    // identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
    Virus &operator[](const virus_id_type &id) const {
        if (!exists(id)) {
            throw VirusNotFound();
        } else {
            auto it = viruses.find(id);
            return *(it->second);
        }
    }

    // Tworzy węzeł reprezentujący nowy wirus o identyfikatorze id
    // powstały z wirusów o podanym identyfikatorze parent_id lub
    // podanych identyfikatorach parent_ids.
    // Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
    // id już istnieje.
    // Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
    // poprzedników nie istnieje.
    void create(virus_id_type const &id, virus_id_type const &parent_id) {
        create(id, vector<virus_id_type>{parent_id});
    }

    void create(virus_id_type const &id,
                std::vector<virus_id_type> const &parent_ids) {
        if (exists(id)) {
            throw VirusAlreadyCreated();
        } else {
            if (parent_ids.size() == 0) {
                throw VirusNotFound();
            }
            for (auto parent_id : parent_ids) {
                if (!exists(parent_id)) {
                    throw VirusNotFound();
                }
            }
            auto state = saveCurrentState();
            try {
                viruses.insert({id, make_shared<Virus>(id)});
                for (auto parent_id : parent_ids) {
                    connect(id, parent_id);
                }
            } catch (exception &e){
                rollback(state);
                throw e;
            }
        }
    }

    // Dodaje nową krawędź w grafie genealogii.
    // Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie
    // istnieje.
    void connect(virus_id_type const &child_id,
                 virus_id_type const &parent_id) {
        if (!exists(child_id) || !exists(parent_id)) {
            throw VirusNotFound();
        } else {
            children[parent_id].insert(child_id);
            parents[child_id].insert(parent_id);
        }
    }

    // Usuwa wirus o podanym identyfikatorze.
    // Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
    // Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
    // wirusa macierzystego.
    void remove(virus_id_type const &id) {
        if (!exists(id)) {
            throw VirusNotFound();
        } else if (id == stem_id) {
            throw TriedToRemoveStemVirus();
        } else {
            auto state = saveCurrentState();
            try {
                disconnect(id);
                dispose(id);
                cleanUp();
            } catch (exception &e){
                rollback(state);
                throw e;
            }
        }
    }

  private:

    void dispose(virus_id_type const &id) { viruses.erase(id); }

    void disconnect(virus_id_type const &id) {
        if (!exists(id)) {
            throw VirusNotFound();
        }
        for (auto parent_id : parents[id]) {
            children[parent_id].erase(id);
        }
        for (auto child_id : children[id]) {
            parents[child_id].erase(id);
        }
    }

    void cleanUp() {
        auto reachable = getReachableFromStem();
        vector<virus_id_type> to_be_disposed;
        for (auto e : viruses) {
            virus_id_type id = e.first;
            if (reachable.find(id) == reachable.end()) {
                disconnect(id);
                to_be_disposed.push_back(id);
                // can't delete now, as viruses set would change
            }
        }
        for (auto id : to_be_disposed) {
            dispose(id);
        }
    }

    set<virus_id_type> getReachableFromStem() {
        set<virus_id_type> ret;
        dfs(stem_id, ret);
        return ret;
    }

    void dfs(virus_id_type const &id, set<virus_id_type> &visited) {
        if (visited.find(id) == visited.end()) {
            visited.insert(id);
            for (auto child : children[id]) {
                dfs(child, visited);
            }
        }
    }

    using virus_map = std::map<virus_id_type, shared_ptr<Virus>>;
    using relations_map = std::map<virus_id_type, std::set<virus_id_type>>;
    using genealogy_state = std::tuple<virus_map, relations_map, relations_map>;

    genealogy_state saveCurrentState() {
        return make_tuple(viruses, children, parents);
    }

    void rollback(genealogy_state state) {
        tie(viruses, children, parents) = state;
    }

    const virus_id_type stem_id;
    std::map<virus_id_type, std::set<virus_id_type>> children;
    std::map<virus_id_type, std::set<virus_id_type>> parents;
    std::map<virus_id_type, shared_ptr<Virus>> viruses;

};

#endif // VIRUS_GENEALOGY_H
