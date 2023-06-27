#include <patches.hpp>

int main(){
 using pers = ns::patches <
        ns::field<"name", string>,
        ns::field<"age", int>
        >;
    
    using location = ns::patches <
        ns::field<"country", string>,
        ns::field<"province", string>
    >;

    using wine = ns::patches <
        ns::field<"name", string>,
        ns::field<"age", int>,
        ns::field<"location", location>
    >;

    pers fatih {"fatih",42};

    wine w1 {
        "champagne",
        33,
        {"France", "Champagne"}
    };

    auto print_name = [] (const ns::field<"name",string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_age = [] (const ns::field<"age",int> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_country = [] (const ns::field<"country", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_province = [] (const ns::field<"province", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_location = [&] (const ns::field<"location", location> &f){
        cout << f.name() << ":(";
        print_country(f.value);
        cout << ", ";
        print_province(f.value);
        cout << ")";
    };

    print_name(fatih);
    cout << "\n";
    print_age(fatih);


    cout << "\n";

    print_name(w1);
    cout << "\n";
    print_age(w1);
    cout << "\n";
    print_location(w1);

    using fam = ns::patches<
        ns::field<"p1", pers>,
        ns::field<"p2", pers>,
        ns::field<"p3", pers>,
        ns::field<"p4", pers>
    >;

    fam nice {
        {"A",20},
        {"B",21},
        {"C",22},
        {"D",23},
    };
    ns::getto<"p3","age">(nice)+=10;
    return ns::getto<"p3","age">(nice);    
