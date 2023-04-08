#include <patches.hpp>

int main(){
    using person = ns::patches< 
        ns::field<"Age", int>,
        ns::field<"Name", const char *>
    >;
 
    person Y;
    ns::getto<"Age">(Y) = 30;
    ns::getto<"Name">(Y) = "Foti";
    
    person Z {21, "HOIII"};

    vector<person> persons;
    persons.emplace_back(12, "ME");

    array<person, 2> onlar {{
        {33, "o"},
        {0, "biz"}
    }};

    using evlicift = ns::patches<
        ns::field<"e1", person>,
        ns::field<"e2", person>
    >;

    evlicift cif {
        {30,"merve"},
        {32,"mahmut"}
    };
    ns::getto<"e1","Age"> (cif) +=25;
    cout << ns::serialize::tsv(ns::getto<"e2">(cif)) <<"\n";

    return ns::getto<"Age">(Y) + ns::getto<"Age">(Z);
}
