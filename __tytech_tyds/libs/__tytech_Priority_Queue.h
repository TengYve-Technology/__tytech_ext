#include<vector>
#include<utility>
#include<stdexcept>

namespace __tytech_tyds{
    inline namespace __tytech_Priority_Queue{
        template<typename T> class mz_Comp{
        public:
            bool operator()(T x,T y){
                return x<y;
            }
        };
        template<typename T,template<typename> class Comp = mz_Comp> class Priority_Queue{
            std::vector<T> tr;
            int tot = 0;
            void up(int x) {
                while (x>0 && Comp<T>()(tr[x],tr[(x-1)/2])) {
                    std::swap(tr[x],tr[(x-1)/2]);
                    x = (x-1)/2;
                }
            }
            void down(int x) {
                while (2*x+1 < tot) {
                    int c = 2*x+1;
                    if (c+1 < tot && Comp<T>()(tr[c+1],tr[c])) c++;
                    if (!Comp<T>()(tr[c],tr[x])) break;
                    std::swap(tr[x],tr[c]);
                    x = c;
                }
            }
            void _push(T x){
                if(tot >= tr.size()){
                    tr.resize(tot*2+5);
                }
                tr[tot] = x;
                up(tot);
                tot++;
            }
            void _pop(){
                if(!tot){
                    throw std::out_of_range("Invalid range");
                }
                tr[0] = tr[tot-1];
                tot--;
                down(0);
            }
            T _top(){
                if(!tot){
                    throw std::out_of_range("Invalid range");
                }
                return tr[0];
            }
        public:
            void push(T x){_push(x);}
            void pop(){_pop();}
            T top(){return _top();}
            bool empty(){return !tot;}
        };
    }
}