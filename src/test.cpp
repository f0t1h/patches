#include <patches.hpp>


void foo( const HAS_FIELDS(NTF(name,string), NTF(age,int)) auto &k){
    const nt::field<"name", string> & name = k;
    const nt::field<"age", int> & age = k;
    cout << name.value << "\t" << age.value;
 }




int main(){
    NTF(BOB, int) bob{5};
    ++ bob;
    std::cout << sizeof(bob) << "\n";
    using pers = nt::patches <
        nt::field<"name", string>,
        nt::field<"age", int>
    >;
    using qq_wn = tuple<
        nt::field<"B",string>,
        nt::field<"A",int>
        >;
    using qq = tuple<string,int>;
    cout << sizeof(qq{}) << "\t"<< sizeof(qq_wn{}) << "\t" << sizeof(pers{}) << "\n"; // Shouldn't use more space than std tuples.;

    pers fatih {"fatih",42};
    foo(fatih);
    
    using location = nt::patches <
        nt::field<"country", string>,
        nt::field<"province", string>
    >;
    using wine = nt::patches <
        nt::field<"name", string>,
        nt::field<"age", int>,
        nt::field<"location", location>
    >;

    wine w1 {
        "champagne",
        33,
        {"France", "Champagne"}
    };

    auto ff = []( const HAS_FIELDS(
                    NTF(name,string),
                    NTF(age,int), 
                    NTF(location,location)
                ) auto &k){
        const NTF(name, string) & name = k;
        int age = k;
        const location &loc = k;
        const NTF(province,string) prov = loc;
        cout << "\n---\n---" << name.value << "\t" << age << "\t" << prov.value << "\n---\n" ;
    };

    auto ff2 = []( const HAS_FIELDS(
                    NTF(name,string),
                    NTF(age,int)
                ) auto &k){
                    const string & name = k;
                    int age = k;
                    cout << "name:" << name << "\nage:" << age << "\n";
                }  ;
    

    ff(w1);
    // ff(fatih); // Error because pers doenst have location
    ff2(w1);
    ff2(fatih);

    

    auto ff3 = [] (const HAS_FIELDS(
                nt::field<"country", string>,
                nt::field<"province", string>
    ) auto &k){
        const string &s = k;
        const NTF(province,string) &p = k;
        cout << s << "\n" <<  p.value << "\n\n";
    };

    ff3(w1.next().next().value);

    auto print_name = [] (const nt::field<"name",string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_age = [] (const nt::field<"age",int> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_country = [] (const nt::field<"country", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_province = [] (const nt::field<"province", string> &f){
        cout << f.name() << ":" << f.value;
    };
    auto print_location = [&] (const nt::field<"location", location> &f){
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

    cout << "\n" << nt::getto<"location", "country">(w1) << "\n";

    using fam = nt::patches<
        nt::field<"p1", pers>,
        nt::field<"p2", pers>,
        nt::field<"p3", pers>,
        nt::field<"p4", pers>
    >;

    fam nt {
        {"A",20},
        {"B",21},
        {"C",22},
        {"D",23},
    };


    nt::getto<"p3","age">(nt)+=10;

    nt::field<"me", int> q{0};
    nt::field<"you", int> z{8};
    q ++;
    ++ q;
    q += z;
    
    cout << q << "\n";
    q = 5;
    cout << q << "\n";
    cout << typeid(q).name() << "\n";

    
    auto l1 = [](nt::field<"B",int> ){};
    auto l2 = [](nt::field<"B",string> ) {};
    using qv = nt::patches <
        nt::field<"B",int>,
        nt::field<"B",string>
    >;
    l1(qv{});
    l2(qv{});
    return 0;    
}
