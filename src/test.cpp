#include <patches.hpp>


using namespace nt::literals;



auto restricted_name_func (nt::has_field_name<"Name"_sc, "Age"_sc> auto var) {

    return var;
}
auto restricted_func (nt::has_field<
        nt::field<"Name", const char*>,
        nt::field<"Age", int>
        > auto var){
    return var;
}

int main(){
    using namespace nt;
    using namespace nt::literals;
    using person = patches<
        field<"Name", const char*>,
        field<"Age", int>
    > ;
    using wine  = patches<
        field<"Name", const char*>,
        field<"Age", int>,
        field<"Location", const char*>
    > ;
    using couple = patches<
        field< "A", person>,
        field< "B", person>
        >;
    person p{"Fatih", 44};
    wine   w{"Wine", 4, "Ankara"};

    couple c{person{"A",100}, person{"B",98}};
    restricted_func(p);
    restricted_func(w);
    vector<person> people{{
        {"A",10},
        {"B",12},
        {"C",13}
    }};
    // restricted_function(4); // Doesnt work
    //fmt::print("{}\n", c);
    //fmt::print("{} and {}", p, c);
    fmt::print("{}\n", p);
    fmt::print("{}\n", c);
    fmt::print("{}\n", people);
}

