#include<cstdio>
#include<cassert>
#include<cstdint>
#include<cstring>
#include<string> 
#include<type_traits>

namespace __tytech_tyds{
    inline namespace __tytech_FastIO{
        class FastIO{
            uint8_t in_buf[1<<20],*p1,*p2;
            char out_buf[1<<20];
            int out_pos;
            int gc(){
                return (p1==p2&&(p2=(p1=in_buf)+fread(in_buf,1,1<<20,stdin),p1==p2))?EOF:*p1++;
            }
            template <typename T> inline T readint(){
                T k = 0;
                int c = gc();
                for(;c!=EOF && !('0'<=c&&c<='9') && c!='-'; c = gc()){}
                bool neg = false;
                if(c == '-') { neg = true; c = gc(); }
                for(;'0'<=c && c<='9';c = gc())
                    k = k*10+(c^48);
                if constexpr (std::is_signed<T>::value) return neg?-k:k;
                else                                    return k;
            }

            inline std::string readstr(){
                int c = gc();
                std::string ret = "";
                while(c!='\n' && c!='\r' && c!=EOF){
                    ret += char(c);
                    c = gc();
                }
                return ret;
            }

            template<typename T> inline T readDouble() {
                T x = 0;
                int f = 1;
                int c = gc();
                while(c<'0' || c>'9') {
                    if(c == '-') f = -1;
                    c = gc();
                }
                while(c!=EOF && c>='0' && c<='9') {
                    x = x*10+(c-'0');
                    c = gc();
                }
                if(c == EOF) return 0;
                if(c == '.'){
                    c = gc();
                    T y = 0;
                    T div = 1;
                    while(c>='0' && c <= '9'){
                        y = y*10+(c-'0');
                        div *= 10;
                        c = gc();
                    }
                    x += y/div;
                }

                return x*f;
            }

            template <typename T> inline void writeint(T x,char end = '\n'){
                if (out_pos >= (1 << 20) - 40) flush();

                using U = typename std::make_unsigned<T>::type;
                U ux;
                bool neg = false;
                if constexpr (std::is_signed<T>::value){
                    if(x < 0){neg = true;ux = U(-(x+1))+1;}
                    else ux = U(x);
                } 
                else ux = x;
                if(neg) out_buf[out_pos++] = '-';
                static char sta[40];
                int top = 0;
                do{
                    sta[top++] = char(ux%10)+'0';
                    ux /= 10;
                }while (ux);
                while (top) out_buf[out_pos++] = sta[--top];
                out_buf[out_pos++] = end;
            }

            inline void writestr(const std::string& x, char end='\n'){
                for(char ch:x){
                    if(out_pos >= (1<<20)-2) flush();
                    out_buf[out_pos++] = ch;
                }
                if(out_pos >= (1<<20)-2) flush();
                out_buf[out_pos++] = end;
            }

            template<typename T> void writeDouble(T x, int num_digits, char end='\n'){
                char buf[64];
                int n = snprintf(buf,sizeof(buf),"%.*f",num_digits,(double)x);
                writestr(std::string(buf,n),end);
            }
        public:
            FastIO(){
                memset(out_buf,0,sizeof(out_buf));
                p1=in_buf,p2=in_buf;
                out_pos = 0;
            }
            inline void flush() {
                if(out_pos) fwrite(out_buf, 1, out_pos, stdout);
                out_pos = 0;
            }
            template <typename T> inline T read(){
                static_assert(std::is_integral_v<T>||std::is_floating_point_v<T>||std::is_same_v<T,char>||std::is_same_v<T,std::string>,"T must be integral, floating_point, string or char!");
                if constexpr (std::is_same_v<T,char>){
                    return gc();
                }
                else if constexpr (std::is_integral<T>::value){
                    return readint<T>();
                }
                else if constexpr (std::is_floating_point<T>::value){
                    return readDouble<T>();
                }
                else{
                    return readstr();
                }
            }

            template <typename T> inline void write(T x,char end = '\n',int num_digits = -1){
                static_assert(std::is_integral_v<T>||std::is_floating_point_v<T>||std::is_same_v<T,char>||std::is_same_v<T,std::string>,"T must be integral, floating_point, string or char!");
                if(std::is_floating_point_v<T>) num_digits = (num_digits==-1?6:num_digits);
                else if(num_digits != -1)       {fputs("This function can't keep decimals for types other than floats!",stderr);abort();}
                if constexpr (std::is_same_v<T,char>){
                    if(out_pos >= (1<<20)-40) flush();
                    out_buf[out_pos++] = x;
                    out_buf[out_pos++] = end;
                }
                else if constexpr (std::is_integral<T>::value){
                    writeint<T>(x,end);
                }
                else if constexpr (std::is_floating_point<T>::value){
                    writeDouble<T>(x,num_digits,end);
                }
                else if constexpr (std::is_same_v<T,std::string>){
                    writestr(x,end);
                }
            }
        };
    }
}