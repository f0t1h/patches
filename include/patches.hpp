#include <iostream>
#include <string_view>
#include <string>
#include <algorithm>
#include <utility>

#include <iomanip>
#include <array>
#include <vector>
#include <sstream>
#include <functional>
#include <concepts>
#include <map>

#define FMT_CONSTEXPR constexpr
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/compile.h>
#include <fmt/format.h>



using namespace std;
namespace nt{
namespace detail{
template<size_t N>
using string_literal_type = const char (&)[N];
template<char... C>
struct char_pack{};

template<size_t N>
struct size_const{
    static constexpr size_t val = N;
};
template<class K>
concept has_val = requires {
    (size_t)K::val;
};
template<size_t N>
struct StringLiteral {
    static constexpr size_t size(){
        return N;
    }
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    // template<size_t I, size_t J>
    // constexpr StringLiteral(const char (&str)[I], const char(&str2)[J]) {
    //     std::copy_n(str, I, value);
    //     std::copy_n(str2, J, value + I - 1);
    //     if constexpr( I + J - 1 < N){
    //         for(size_t i = I + J - 1; i < N;i++){
    //             value[i] = 0;
    //         }
    //     }
    // }
    // template<size_t I, size_t J>
    // constexpr StringLiteral(const StringLiteral<I> &s1,const StringLiteral<J> &s2) {
    //     std::copy_n(s1.value, I, value);
    //     std::copy_n(s2.value, J, value + I - 1);
    //     if constexpr( I + J - 1 < N){
    //         for(size_t i = I + J - 1; i < N;i++){
    //             value[i] = 0;
    //         }
    //     }
    // }
    // template<size_t F>
    // constexpr StringLiteral(has_val auto k, const StringLiteral<F> &fs){
    //     std::copy_n(fs.value, F, value + decltype(k)::val - 1);
    //     if constexpr( decltype(k)::val + F - 1 < N){
    //         for(size_t i = decltype(k)::val + F - 1; i < N;i++){
    //             value[i] = 0;
    //         }
    //     }
    // }
    // template< size_t F, size_t...I>
    // constexpr StringLiteral( has_val auto k, const StringLiteral<F> &fs, const StringLiteral<I> &...s)
    // :   StringLiteral (size_const<decltype(k)::val + F - 1>{}, s...){
        
    //     std::copy_n(fs.value, F - 1, value + decltype(k)::val - 1);
    // }
    // template<size_t F, size_t...I>
    // constexpr StringLiteral(const StringLiteral<F> &fs, const StringLiteral<I> &...s)
    // :   StringLiteral(size_const<1>{},fs, s...){
     
    // }
    template<size_t...J>
    constexpr StringLiteral (StringLiteral<J>... q) {
        auto v = std::begin(value);
        ((v = std::copy_n(q.value,J-1,v)), ...);
        
        while(v < value + N){
            *v = 0;
            ++v;
        }
    }
    template<char ...C>
    constexpr StringLiteral(char_pack<C...>) : value{C...}{
    }
    
    char value[N];
    friend ostream& operator<<(ostream &ost, const StringLiteral<N> &st){
        ost << st.value;
        return ost;
    }
};
template<size_t...N>
constexpr auto literal_cat(const StringLiteral<N>... t){
    return StringLiteral<((N) + ...)>{t...};
}

template<StringLiteral L>
struct string_constant{
    static constexpr auto value = L;
};
template<string_constant...C>
struct sc_pack{
    static constexpr auto to_string() -> array<string, sizeof...(C)> {
        return {{C.value.value...}};
    }
};

template<size_t N, size_t M>
constexpr bool operator==(const StringLiteral<N> &s1,
    const StringLiteral<M> &s2){
        return string_view(s1.value) == string_view(s2.value);
}


template<char N, char... rest>
void print_n(){
    cout << N << "_";
    if constexpr(sizeof...(rest) > 0){
        print_n<rest...>();
    }
}

}
template<detail::StringLiteral N, class I>
struct field {
    using type = I;
    type value;
    static constexpr auto name = N;
    constexpr field(const type &t) : value{t} {}
    constexpr field() {}
    constexpr auto &operator[](detail::string_constant<N>){
        return value;
    }
    constexpr auto operator[](detail::string_constant<N>) const{
        return value;
    }
};
template<class K>
concept leaf_type =
requires {
    typename K::type;
} && 
requires (K k) {
    k.value;
} &&
requires {
    K::name;
};
namespace serialize2{
template<leaf_type F, leaf_type ...M>
struct json_obj;
}

template<leaf_type ...M>
struct patches: public M...{
    using M::operator[]...;
    using names = detail::sc_pack<detail::string_constant<M::name>{}...>;
    constexpr operator std::tuple<typename M::type...> ()  {
        return  {operator[](detail::string_constant<M::name>{})...};
    }
    constexpr auto named_pairs ()  -> std::tuple<std::pair<decltype(M::name),typename M::type>...> {
        return  {std::pair{M::name,operator[](detail::string_constant<M::name>{})}...};
    }
    constexpr auto cnamed_pairs ()  const -> std::tuple<std::pair<decltype(M::name),typename M::type>...>  {
        return  {std::pair{M::name,operator[](detail::string_constant<M::name>{})}...};
    }    constexpr auto tuple () ->  std::tuple<typename M::type...>  {
        return  std::tuple<typename M::type...> {*this};
    }
    using json = serialize2::json_obj<M...>;    
};
template<detail::StringLiteral index>
struct getter{
    auto &operator ()(auto &patch){
        return patch[detail::string_constant<index>{}];
    }
};
namespace literals{
template<class T, T...t>
constexpr auto operator ""_get() {
    return getter<detail::StringLiteral<sizeof...(t)+1>{detail::char_pack<t...>{}}>{};
}
template<class T, T...t>
constexpr auto operator ""_sc() {
    return detail::string_constant<detail::StringLiteral<sizeof...(t)+1>{detail::char_pack<t...>{}}>{};
}
template<class T, T...t>
constexpr auto operator ""_lit() {
    return detail::StringLiteral<sizeof...(t)+1>{detail::char_pack<t...>{}};
}
}

template<class Projection, int depth = 0, detail::StringLiteral sep="\t">
struct printer{
    Projection wrapper;
    template<class first>
    void operator()(auto &stream, const first &f) const{
        for(int i = 0; i < depth; ++i){
            stream << "\t";
        }
        stream << wrapper(f);
    }
    template<class first, class...N>
    void operator()(auto &stream, const first &f, const N&...n) const{
        for(int i = 0; i < depth; ++i){
            stream << "\t";
        }
        stream << wrapper(f) << sep;
        operator()(stream, n...);
    }
};

namespace serialize2{
// template<class... F>
// class json_obj{
// };
    using namespace nt::literals;
template<class T>
concept StringLike = std::is_convertible_v<T, std::string_view>;
template<class T>
concept leaf_type_with_stringlike_type =
    nt::leaf_type<T> && StringLike<typename T::type>;

template<class K>
struct get_quot{
    static constexpr nt::detail::StringLiteral<3> value{"{}"};
};
template<leaf_type_with_stringlike_type K>
struct get_quot<K>{
    static constexpr nt::detail::StringLiteral<5> value{"\"{}\""};
};
template<leaf_type F, leaf_type ...T>
struct json_obj_helper{
    static constexpr auto val = 
        nt::detail::literal_cat("\""_lit, F::name, "\":"_lit, get_quot<F>::value, ","_lit, json_obj_helper<T...>::val);
};


template<leaf_type F>
struct json_obj_helper<F>{
    static constexpr auto val = 
        nt::detail::literal_cat("\""_lit, F::name, "\":"_lit, get_quot<F>::value);
};
template<leaf_type F, leaf_type ...T>
struct json_obj{
    static constexpr auto val =
      nt::detail::literal_cat("{{"_lit, json_obj_helper<F,T...>::val, "}}"_lit);
};

}

template<class K, nt::detail::string_constant... sc>
concept has_field_name  = requires(K k){
    (k[sc] , ...);
};
template<class K, nt::detail::string_constant name, class type>
concept has_field_single  = requires(K k){
    {k[name]} -> convertible_to<type>;
 };
template<class K, class ...F> 
concept has_field =
    (has_field_single<K,nt::detail::string_constant<F::name>{}, typename F::type> && ...);

}

template <nt::leaf_type...L> 
struct fmt::formatter<nt::patches<L...>> : formatter<string_view> {
  // parse is inherited from formatter<string_view>.

    auto format(const nt::patches<L...>& c, format_context& ctx) const{
        return fmt::format_to(ctx.out(), 
        FMT_COMPILE(nt::patches<L...>::json::val.value),
        c[nt::detail::string_constant<L::name>{}]...);
    }
};


