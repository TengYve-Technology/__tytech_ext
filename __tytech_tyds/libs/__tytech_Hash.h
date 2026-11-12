#include<string>
#include<vector>
#include<stdexcept>

namespace __tytech_tyds{
    inline namespace __tytech_Hash{
        template<typename T> class convert_String{
        public:
            std::string convert(const T& s){
                return to_string(s);
            }
        };
        
        template<> class convert_String<std::string>{
        public:
            std::string convert(const std::string& s){
                return s;
            }
        };
        
        template<> class convert_String<const char*>{
        public:
            std::string convert(const char* s){
                return std::string(s);
            }
        };
        
        template<typename T,template <typename> class U = convert_String> class Hash{
        private:
            std::vector <unsigned long long> h;
            std::vector <unsigned long long> p;
            long long n;
            U<T> convert2str;
            unsigned int id(char c) const {
                return static_cast<unsigned char>(c); 
            }
        public:
            Hash(T x){
                int _p = 998244353;
                std::string s = convert2str.convert(x);
                if(_p % 2 == 0){
                    throw std::invalid_argument("Base must be odd");
                }
                n = s.size();
                p.resize(n+5);
                h.resize(n+5);
                h[0] = 0;
                p[0] = 1;
                for(int i = 1;i<=n;i++){
                    p[i]=p[i-1]*_p;
                }
                for(int i = 1;i<=n;i++){
                    unsigned int val = id(s[i-1]);
                    h[i]=h[i-1]*_p+val;
                }
            }
            Hash(T x,int _p){
                std::string s = convert2str.convert(x);
                if(_p % 2 == 0){
                    throw std::invalid_argument("Base must be odd");
                }
                n = s.size();
                p.resize(n+5);
                h.resize(n+5);
                h[0] = 0;
                p[0] = 1;
                for(int i = 1;i<=n;i++){
                    p[i]=p[i-1]*_p;
                }
                for(int i = 1;i<=n;i++){
                    unsigned int val = id(s[i-1]);
                    h[i]=h[i-1]*_p+val;
                }
            }
            Hash(T x,int _p,std::string (*func)(T)){
                std::string s = func(x);
                if(_p % 2 == 0){
                    throw std::invalid_argument("Base must be odd");
                }
                n = s.size();
                p.resize(n+5);
                h.resize(n+5);
                h[0] = 0;
                p[0] = 1;
                for(int i = 1;i<=n;i++){
                    p[i]=p[i-1]*_p;
                }
                for(int i = 1;i<=n;i++){
                    unsigned int val = id(s[i-1]);
                    h[i]=h[i-1]*_p+val;
                }
                return;
            }
            unsigned long long gethash(int l,int r) const {
                if(l < 1 || r > n || l > r) {
                    throw std::out_of_range("Invalid range");
                }
                return h[r] - h[l-1] * p[r-l+1];
            }
            unsigned long long gethash() const{
                return h[n];
            }
            bool operator==(const Hash& other) const {
                return n == other.n && h[n] == other.h[n];
            }
        };
    }
}