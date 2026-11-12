#include<vector>

namespace __tytech_tyds{
    inline namespace __tytech_BST{
        template<typename T> class BST{
            struct node{
                T val;// 值
                node *l;
                int cnt;// 重复数量
                node *r;
                int sz;// 子树大小
                bool is_root;
                node(){
                    l = nullptr;
                    r = nullptr;
                    cnt = 0;
                    sz = 0;
                    val = T();
                }
            };
            node *root;
            void _traverse(node *u,std::vector<T>& ret){
                if(u == nullptr) return;
                _traverse(u->l,ret);
                ret.push_back(u->val);
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
                if(u->val == x)     return true;
                else if(x < u->val) return _find(x,u->l);
                else                return _find(x,u->r);
            }
            node* _insert(T v,node *u){
                if(u == nullptr){
                    u = new node;
                    u->val = v;
                    u->cnt = 1;
                    u->sz  = 1;
                    return u;
                }
                if(u->is_root && !u->cnt && u->l==nullptr && u->r==nullptr){
                    u->val = v;
                    u->cnt = 1;
                    u->sz  = 1;
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
                u->sz = u->cnt+(u->l?u->l->sz:0)+(u->r?u->r->sz:0);
                return u;
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
                u->sz = u->cnt+(u->l?u->l->sz:0)+(u->r?u->r->sz:0);
                return u;
            }
            int _queryRank(T v,node* u){
                if(u == nullptr)    return 0;
                if(u->val == v) return (u->l?u->l->sz:0)+1;
                if(u->val > v)  return _queryRank(v,u->l);
                return _queryRank(v,u->r)+(u->l?u->l->sz:0)+u->cnt;
            }
            std::pair<int,T> _queryKth(int v,node *u){
                if(u == nullptr)    return {-1,T()};
                if(u->l){
                    if(u->l->sz >= v)        return _queryKth(v,u->l);
                    if(u->l->sz+u->cnt >= v) return {0,u->val};
                }
                else{
                    if(v <= u->cnt) return {0,u->val};
                }
                return _queryKth(v-(u->l?u->l->sz:0)-u->cnt,u->r);
            }
        public:
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] BST(){
                root = new node;
                root->is_root = true;
                root->cnt = 0;
            }
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] std::vector<T> traverse(){
                std::vector<T> ret;
                _traverse(root,ret);
                return ret;
            }
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] T findMin(){return _findMin(root)->val;}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] T findMax(){return _findMax(root)->val;}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] bool find(T x){return _find(x,root);}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] void insert(T v){root = _insert(v,root);}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] void erase(T v){root = _erase(v,root);}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] int queryRank(T v){return _queryRank(v,root);}
            [[deprecated("This BST has been deprecated; please use ASL<T> instead.")]] std::pair<int,T> queryKth(int v){return _queryKth(v,root);}
        };
    }
}