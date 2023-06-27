#include <patches.hpp>

int main(){
 using pers = nt::patches <
        nt::field<"name", string>,
        nt::field<"age", int>
        >;
    
    using location = nt::patches <
        nt::field<"country", string>,
        nt::field<"province", string>
    >;

    using wine = nt::patches <
        nt::field<"name", string>,
        nt::field<"age", int>,
        nt::field<"location", location>
    >;

    pers fatih {"fatih",42};

    wine w1 {
        "champagne",
        33,
        {"France", "Champagne"}
    };

    auto print_name = [] (contt nt::field<"name",string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_age = [] (contt nt::field<"age",int> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_country = [] (contt nt::field<"country", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_province = [] (contt nt::field<"province", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_location = [&] (contt nt::field<"location", location> &f){
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

    using fam = nt::patches<
        nt::field<"p1", pers>,
        nt::field<"p2", pers>,
        nt::field<"p3", pers>,
        nt::field<"p4", pers>
    >;

    fam nice {
        {"A",20},
        {"B",21},
        {"C",22},
        {"D",23},
    };
    nt::getto<"p3","age">(nice)+=10;

    nt::field<"me", int> q{0};

    q ++;
    ++ q;
 
    
    cout << q << "\n";
    q = 5;
    cout << q << "\n";
    cout << typeid(q).name() << "\n";

    using qq = tuple<
        nt::field<"A",int>,
        nt::field<"B",string>
        >;

    cout << sizeof(qq{}) << "\t" << sizeof(pers{});
    return nt::getto<"p3","age">(nice);    
}
