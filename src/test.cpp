#include <patches.hpp>

int main(){
    using person = ns::patches< 
        ns::field<"Age", int>,
        ns::field<"Name", string>
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
    // ns::serialize::tsv printer{Y};
    
    // cout << printer.header() << "\n";
    // cout << printer << "\n";
    // cout << ns::serialize::tsv{Z};
    //check_FR<decltype(Y)>{};
    return ns::getto<"Age">(Y) + ns::getto<"Age">(Z);
}
