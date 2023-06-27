#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include<algorithm>
#include <execution>
#include<vector>
#include <string_view>
#include <array>

using namespace std;
namespace nt{
template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];
    friend ostream& operator<<(ostream &ost, const StringLiteral<N> &st){
        ost << st.value;
        return ost;
    }
};

template<size_t N, size_t M>
constexpr bool operator==(const StringLiteral<N> &s1,
    const StringLiteral<M> &s2){
        return string_view(s1.value) == string_view(s2.value);
}

template<StringLiteral lit, class C>
class field{
    public:
    using type = C;
    C value;

    constexpr static auto name() {
        return lit;
    }
    constexpr bool operator == (const auto &other) const {
        return name() == other.name() && typeid(type) == typeid(other.type);
    }
    operator C() const{
        return value;
    }
    operator C&() {
        return value;
    }
    void operator=(const C &c){
        value = c;
    }
};


template<class K>
concept has_type =
requires (){
    typename K::type;
};

template<class K>
concept has_name =
requires (K k){
    k.name();
};

template<class K>
concept has_value =
requires (K k){
    k.value();
};
template<class K>
concept streamoutable =
requires(ostream &o, K k){
    o << k;
};

class not_found{
};

bool operator ==(const not_found &,const not_found &){
    return true;
}
bool operator ==(const auto &, const not_found &){
    return false;
}
bool operator ==(const not_found &, const auto &){
    return false;
}




template<class K>
concept empty_constructable = requires(){
    K{};
};

template<class K>
concept has_empty_constructable_type = has_type<K> && empty_constructable<typename K::type>;

template<class K>
concept field_type = 
    has_empty_constructable_type<K> &&
    has_name<K>;

template<class K>
concept serializable_field = 
    field_type<K> &&
    streamoutable<typename K::type>;


// template<class K>
// concept patch_type = has_fields<K>;


template<field_type C, field_type  ...F>
class patches : public patches<F...>{
    public:
    using type = C::type;
    type value;
    patches() : value{} {};
    patches(const type &c, const F::type& ...f):
        patches<F...>{f...} , value{c} {}
    operator C () const{
        return C{value};
    }
};

template<field_type C>
class patches<C>{
    public:
    using type = C::type;
    type value;
    patches() : value{} {};
    patches(const type &c) : value{c} {}
    constexpr static auto name() {
        return C::name();
    }
    operator C () const{
        return C{value};
    }
};


template<field_type C, field_type  ...F>
auto run(const patches<C,F...> &p, auto func){
    func(p);
    if constexpr(sizeof...(F) > 0){
        run(static_cast<patches<F...>>(p),func);
    }
}




template<field_type f, field_type ...F>
constexpr auto end( const patches<f, F...> &p){
    return  not_found{};
}


constexpr bool operator==(not_found ff, not_found nf){
    return true;
}


template<field_type f, field_type ...F>
constexpr auto operator*( const patches<f, F...> &p){
    return make_pair(f::name(), p.value);
}




namespace serialize{ // Make everything range
    template<serializable_field f, serializable_field...fields>
    struct json{ //TODO
        using patch_type = patches<f,fields...>;
        const patch_type &patch;
        const string_view sep;
        constexpr json(const patch_type &p, const string_view sep = "\t") : patch{p}, sep{sep}{
        
        }

        friend ostream& operator << (ostream &ost, const json<f, fields...> &p){
            ost << p.patch.value;
            if constexpr (sizeof...(fields) > 0){
                ntrun(static_cast<patches<fields...>>(p.patch), [&ost](const auto &q){
                    ost << "\t" <<  q;
                });
            }
            return ost;
        }
    };

    template<serializable_field f, serializable_field...fields>
    struct tsv{
        using patch_type = patches<f,fields...>;
        const patch_type &patch;
        const string_view sep;
        constexpr tsv(const patch_type &p, const string_view sep = "\t") : patch{p}, sep{sep}{
        
        }

        friend ostream& operator << (ostream &ost, const tsv<f, fields...> &p){
            ost << p.patch.value;
            if constexpr (sizeof...(fields) > 0){
                ntrun(static_cast<patches<fields...>>(p.patch), [&ost](const auto &q){
                    ost << "\t" <<  q.value;
                });
            }
            return ost;
        }

        class _header{
            public:
            _header() {}
            friend ostream& operator << (ostream &ost, const tsv<f, fields...>::_header &p){
                ost << f::name();
                (ost << ... << ("\t"s + string{ fields::name().value}));
                return ost;
            }
        };

        static auto header(){
            return _header{};
        }
    };
};
template<StringLiteral lit, StringLiteral... rem>
struct get_impl_fn{
    template<field_type x, field_type y, field_type... f>
    constexpr auto &operator() (patches<x, y, f...> &tup) const {
        
        if constexpr(lit == x::name()){
            if constexpr (sizeof...(rem) == 0){
                return tup.value;
            }
            else{
                return get_impl_fn<rem...>{}(tup.value);
            }
        }
        else{
            return operator()(static_cast<patches<y, f...>&>(tup));
        }
    

    }
    template<field_type x>
    constexpr auto &operator() (patches<x> &tup) const {
        if constexpr(lit == x::name()){
            if constexpr (sizeof...(rem) == 0){
                return tup.value;
            }
            else{
                return get_impl_fn<rem...>{}(tup.value);
            }
        }
        else{
            return not_found{};
        }
    
    }
};
template<StringLiteral... lit>
constexpr get_impl_fn<lit...> getto;
}

