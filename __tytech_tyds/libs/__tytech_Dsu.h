#include<vector>
#include<unordered_map>

namespace __tytech_tyds{
    inline namespace __tytech_Dsu{
        template<typename T> class Dsu{
            std::vector<int> f;
            std::unordered_map<T,int> ump;
            int sz = 0;
            void update(){
                if(sz+10 > f.size()){
                    int pos = f.size();
                    f.resize(sz*2+5);
                    for(int i = pos;i<f.size();i++){
                        f[i] = i;
                    }
                }
            }

            int _find(int x){
                if(f[x] == x)   return x;
                return          f[x] = _find(f[x]);
            }

            void _merge(int x,int y){
                f[x] = y;
            }
        public:
            void merge(T x,T y){
                if(ump.find(x) == ump.end())    ump[x] = ++sz;
                update();
                if(ump.find(y) == ump.end())    ump[y] = ++sz;
                update();
                _merge(_find(ump[x]),_find(ump[y]));
            }

            int find(T x){
                if(ump.find(x) == ump.end())    ump[x] = ++sz;
                update();
                return _find(ump[x]);
            }
        };
    };
}