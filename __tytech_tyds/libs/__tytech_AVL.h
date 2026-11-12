#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

namespace __tytech_tyds{
    inline namespace __tytech_AVL{
        template<typename T> class AVL{
            struct node{
                T val;
                node *l;
                int cnt;
                node *r;
                int sz;
                int ht;
                bool is_root;
                node(){
                    l = nullptr;
                    r = nullptr;
                    cnt = 0;
                    sz = 0;
                    ht = 0;
                    is_root = false;
                    val = T();
                }
            };
            node *root;

            void pull(node *u){
                u->sz = u->cnt + (u->l?u->l->sz:0) + (u->r?u->r->sz:0);
                int lh = u->l?u->l->ht:0;
                int rh = u->r?u->r->ht:0;
                u->ht = std::max(lh,rh)+1;
            }

            node* left_rotate(node *u){
                node *nw = u->r;
                u->r = nw->l;
                nw->l = u;
                pull(u);
                pull(nw);
                return nw;
            }
            node* right_rotate(node *u){
                node *nw = u->l;
                u->l = nw->r;
                nw->r = u;
                pull(u);
                pull(nw);
                return nw;
            }

            void _traverse(node *u,std::vector<T>& ret){
                if(u == nullptr) return;
                _traverse(u->l,ret);
                if(u->cnt > 0) ret.push_back(u->val);
                _traverse(u->r,ret);
            }
            node* _findMin(node *u){
                if(u == nullptr) return nullptr;
                while(u->l != nullptr){
                    u = u->l;
                }
                return u;
            }
            node* _findMax(node *u){
                if(u == nullptr) return nullptr;
                while(u->r != nullptr){
                    u = u->r;
                }
                return u;
            }
            bool _find(T x,node *u){
                if(u == nullptr)    return false;
                if(u->cnt>0 && u->val==x) return true;
                if(x < u->val)      return _find(x,u->l);
                else if(x > u->val) return _find(x,u->r);
                else                return false;
            }
            node* _insert(T v,node *u){
                if(u == nullptr){
                    u = new node;
                    u->val = v;
                    u->cnt = 1;
                    pull(u);
                    return u;
                }
                if(u->is_root && !u->cnt && u->l==nullptr && u->r==nullptr){
                    u->val = v;
                    u->cnt = 1;
                    pull(u);
                    return u;
                }
                if(v < u->val){
                    u->l = _insert(v,u->l);
                }
                else if(v > u->val){
                    u->r = _insert(v,u->r);
                }
                else{
                    u->cnt++;
                }
                pull(u);
                return _fix(u);
            }
            node* _erase(T v,node *u){
                if(u == nullptr)    return u;
                if(v < u->val){
                    u->l = _erase(v,u->l);
                }
                else if(v > u->val){
                    u->r = _erase(v,u->r);
                }
                else{
                    if(u->cnt > 1){
                        u->cnt--;
                    }
                    else{
                        if(u->l == nullptr){
                            node* tmp = u->r;
                            delete u;
                            return tmp;
                        }
                        else if(u->r == nullptr){
                            node *tmp = u->l;
                            delete u;
                            return tmp;
                        }
                        else{
                            node *su = _findMin(u->r);
                            u->val = su->val;
                            u->cnt = su->cnt;
                            su->cnt = 1;
                            u->r = _erase(su->val,u->r);
                        }
                    }
                }
                pull(u);
                return _fix(u);
            }

            int _queryRank(T v,node* u){
                if(u == nullptr)    return 0;
                if(u->cnt > 0 && u->val == v) return (u->l?u->l->sz:0)+1;
                if(u->val > v)  return _queryRank(v,u->l);
                if(u->val < v)  return _queryRank(v,u->r)+(u->l?u->l->sz:0)+u->cnt;
                return (u->l?u->l->sz:0)+1;
            }
            std::pair<int,T> _queryKth(int v,node *u){
                if(u == nullptr)    return {-1,T()};
                if(u->l){
                    if(u->l->sz >= v)        return _queryKth(v,u->l);
                    if(u->l->sz+u->cnt >= v) return {0,u->val};
                }
                else{
                    if(u->cnt >= v) return {0,u->val};
                }
                return _queryKth(v-(u->l?u->l->sz:0)-u->cnt,u->r);
            }

            int _bf(node *u){
                return (u->l?u->l->ht:0) - (u->r?u->r->ht:0);
            }

            node* _fix(node *u){
                if(!u) return u;
                pull(u);
                int b = _bf(u);
                if(b > 1){
                    if(_bf(u->l) < 0) u->l = left_rotate(u->l);
                    return right_rotate(u);
                }
                if(b < -1){
                    if(_bf(u->r) > 0) u->r = right_rotate(u->r);
                    return left_rotate(u);
                }
                return u;
            }

            node* _rebalance(node *u){
                if(u == nullptr) return u;
                u->l = _rebalance(u->l);
                u->r = _rebalance(u->r);
                return _fix(u);
            }

            void _destroy(node *u){
                if(!u) return;
                _destroy(u->l);
                _destroy(u->r);
                delete u;
            }
        public:
            AVL(){
                root = new node;
                root->is_root = true;
                root->cnt = 0;
            }

            std::vector<T> traverse(){
                std::vector<T> ret;
                _traverse(root,ret);
                return ret;
            }
            T findMin(){
                node *p = _findMin(root);
                return p?p->val:T();
            }
            T findMax(){
                node *p = _findMax(root);
                return p?p->val:T();
            }
            bool find(T x){return _find(x,root);}
            void insert(T v){root = _insert(v,root);}
            void erase(T v){root = _erase(v,root);}
            int queryRank(T v){return _queryRank(v,root);}
            std::pair<int,T> queryKth(int v){return _queryKth(v,root);}
        };
    }
}