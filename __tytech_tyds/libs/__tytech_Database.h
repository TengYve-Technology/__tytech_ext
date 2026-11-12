	#include<vector>
	#include<type_traits>
	#include<string>
	#include<iostream>
	#include<unordered_map>
	#include<fstream>

	namespace __tytech_tyds{
		inline namespace __tytech_Database{
			template<typename T> class Database{
			private:
				struct node{
					std::string name;
					T val;
					bool is_dir;
					bool is_root;
					bool is_del;
					std::unordered_map <std::string,int> data;
				};
				
				std::string now_dir = "/";
				int now_dir_id = 0;
				std::vector<node> database;
				std::vector<int> delnode;
				
				std::vector<std::string> RePath(std::string res,bool last_is_file = true){
					if(res.empty()) return {};
					std::vector<std::string> path;
					path.clear();
					std::string temp = "/";
					if(res[0] != '/')				res = "/" + res;
					if(res[res.size()-1] != '/')	res = res + "/";
					for(int i = 0;i<res.size();i++){
						if(res[i] == '/')	path.push_back(temp),temp = "/";
						else				temp += res[i];
					}
					if(last_is_file)	path[path.size()-1] = path[path.size()-1].substr(1,path[path.size()-1].size()-1);
					return path;
				}
				
				void Del_Tree(int fat,int start_id,bool is_first = true){
					if(is_first){
						database[fat].data.erase(database[start_id].name);
					}
					database[start_id].is_del = true;
					delnode.push_back(start_id);
					if(!database[start_id].is_dir)	return;
					for(std::unordered_map <std::string,int>::iterator it = database[start_id].data.begin();it!=database[start_id].data.end();it++){
						Del_Tree(fat,it->second,false);
					}
				}
				
				int Put(std::string path,T val){
					if(path.empty() || path == "/") return -1;
					try{
						std::vector<std::string> paths = RePath(path);
						std::string filename = paths[paths.size()-1];
						int now = 0;
						for(int i = 1;i<paths.size()-1;i++){
							if(!database[now].is_dir)											return -1;
							if(database[now].is_del)											return -1;
							if(database[now].data.find(paths[i]) == database[now].data.end())	return -1;
							now = database[now].data[paths[i]];
						}
						node newnode;
						int idx;
						newnode.name = filename;
						newnode.val = val;
						newnode.is_dir = false;
						newnode.is_del = false;
						newnode.is_root = false;
						std::unordered_map<std::string,int>::iterator it = database[now].data.find(filename);
						if(it != database[now].data.end()){
							idx = it->second;
							if(database[idx].is_del){
								database[idx] = newnode;
								database[now].data[filename] = idx;
								return 1;
							}
							if(database[idx].is_dir) return -1;
							database[idx] = newnode;
							return 1;
						}
						else if(delnode.size() == 0){
							database.push_back(newnode); 
							idx = database.size()-1;
						}
						else{
							idx = delnode[delnode.size()-1];
							delnode.erase(delnode.end()-1);
							database[idx] = newnode;
						}
						database[now].data[filename] = idx;
						return 1;
					}
					catch(...){
						return 0;
					}
				}
				
				T Get(std::string path){
					try{
						std::vector<std::string> paths = RePath(path);
						if(paths.empty())	return T();	
						int now = 0;
						for(int i = 1;i<paths.size()-1;i++){
							if(!database[now].is_dir)													throw "PNF";
							if(database[now].is_del)													throw "PNF";
							if(database[now].data.find(paths[i]) == database[now].data.end())			throw "PNF";
							now = database[now].data[paths[i]];
						}
						if(database[now].data.find(paths[paths.size()-1]) == database[now].data.end())	throw "PNF";
						now = database[now].data[paths[paths.size()-1]];
						if(database[now].is_del)														throw "PNF";
						return database[now].val;
					}
					catch(...){
						try{
							if(!(path[0] == '/' && now_dir == "/")){
								path = now_dir+"/"+path;
							}
							std::vector<std::string> paths = RePath(path);
							if(paths.empty()) throw "PNF";
							int now = 0;
							for(int i = 1;i<paths.size()-1;i++){
								if(!database[now].is_dir)													throw "PNF";
								if(database[now].is_del)													throw "PNF";
								if(database[now].data.find(paths[i]) == database[now].data.end())			throw "PNF";
								now = database[now].data[paths[i]];
							}
							if(database[now].data.find(paths[paths.size()-1]) == database[now].data.end())	throw "PNF";
							now = database[now].data[paths[paths.size()-1]];
							if(database[now].is_del)														throw "PNF";
							return database[now].val;
						}
						catch(...){
							std::cerr << "Paths Not Found!" << std::endl;
							return T();
						}
					}
				}
				int Del(std::string path){
					if(path.empty() || path == "/") return -1;
					try{
						std::vector<std::string> paths = RePath(path);
						int now = 0;
						int fat = 0;
						for(int i = 1;i<paths.size();i++){
							if(!database[now].is_dir)											return -1;
							if(database[now].is_del)											return -1;
							if(database[now].data.find(paths[i]) == database[now].data.end())	return -1;
							now = database[now].data[paths[i]];
							if(i == paths.size()-2)	fat = now;
						}
						database[fat].data.erase(database[now].name);
						database[now].is_del = true;
						delnode.push_back(now);
						return 1;
					}
					catch(...){
						return 0;
					}	
				}
				
				int Rmdir(std::string path){
					if(path.empty() || path == "/") return -1;
					try{
						std::vector<std::string> paths = RePath(path,false);
						int now = 0;
						for(int i = 1;i<paths.size()-1;i++){
							if(!database[now].is_dir)													return -1;
							if(database[now].is_del)													return -1;
							if(database[now].data.find(paths[i]) == database[now].data.end())			return -1;
							now = database[now].data[paths[i]];
						}
						int fat = now;
						if(!database[now].is_dir)														return -1;
						if(database[now].is_del)														return -1;
						if(database[now].data.find(paths[paths.size()-1]) == database[now].data.end())	return -1;
						now = database[now].data[paths[paths.size()-1]];
						Del_Tree(fat,now);
						return 1;
					}
					catch(...){
						return 0;
					}	
				}
				
				int Mkdir(std::string path){
					if(path.empty() || path == "/") return -1;
					try{
						std::vector<std::string> paths = RePath(path,false);
						int now = 0;
						std::string dirname = paths[paths.size()-1];
						for(int i = 1;i<paths.size()-1;i++){
							if(!database[now].is_dir)											return -1;
							if(database[now].is_del)											return -1;
							if(database[now].data.find(paths[i]) == database[now].data.end())	return -1;
							now = database[now].data[paths[i]];
						}
						node newnode;
						int idx;
						if(database[now].data.find(dirname) != database[now].data.end()) return -1;
						newnode.name = dirname;
						newnode.is_dir = true;
						newnode.is_del = false;
						newnode.is_root = false;
						if(delnode.size() == 0){
							database.push_back(newnode);
							idx = database.size()-1;
						}
						else{
							idx = delnode[delnode.size()-1];
							delnode.erase(delnode.end()-1);
							database[idx] = newnode;
						}
						database[now].data[dirname] = idx;
						return 1;
					}
					catch(...){
						return 0;
					}
				}
				
				int Cd(std::string path){
					if(path.empty()) return -1;
					try{
						std::vector<std::string> paths = RePath(path,false);
						int now = 0;
						std::string dirname = paths[paths.size()-1];
						for(int i = 1;i<paths.size();i++){
							if(!database[now].is_dir)											return -1;
							if(database[now].is_del)											return -1;
							if(database[now].data.find(paths[i]) == database[now].data.end())	return -1;
							now = database[now].data[paths[i]];
						}
						if(!database[now].is_dir) return -1;
						now_dir_id = now;
						if(paths.size() == 1){
							now_dir = "/";
							return 1;
						}
						std::string tr_path = "";
						for(int i = 1;i<paths.size();i++){
							tr_path += paths[i];
						}
						now_dir = tr_path;
						return 1;
					}
					catch(...){
						return 0;
					}
				}

				int Ls(int now,int nowtab = 0){
					try{
						for(std::unordered_map<std::string,int>::iterator it = database[now].data.begin();it!=database[now].data.end();it++){
							for(int j = 1;j<=nowtab;j++)	std::cout << "    ";
							std::unordered_map<std::string,int>::iterator it2 = it;it2++;
							if(it2 == database[now].data.end())	std::cout << "-";
							else								std::cout << "|";
							std::cout << it->first << (database[it->second].is_dir?':':' ') << std::endl;
							if(database[it->second].is_dir)	Ls(it->second,nowtab+1);
						}
						return 1;
					}
					catch(...){
						return 0;
					}
				}
			public:
				Database(){
					database.clear();
					delnode.clear();
					node root;
					root.name = "/";
					root.is_dir = true;
					root.is_del = false;
					root.is_root = true;
					database.push_back(root);
				}
				
				int put(std::string path,T val){
					return Put(path,val);
				}
				
				T get(std::string path){
					return Get(path);
				}
				
				int del(std::string path){
					return Del(path);
				}
				
				int mkdir(std::string path){
					return Mkdir(path);
				}
				
				int rmdir(std::string path){
					return Rmdir(path);
				}
				
				int cd(std::string path){
					return Cd(path);
				}

				int ls(std::string path){
					if(path.empty()) return -1;
					std::vector<std::string> paths = RePath(path,false);
					if(paths.empty()) return -1;
					int now = 0;
					std::string dirname = paths[paths.size()-1];
					for(int i = 1;i<paths.size();i++){
						if(!database[now].is_dir)											return -1;
						if(database[now].is_del)											return -1;
						if(database[now].data.find(paths[i]) == database[now].data.end())	return -1;
						now = database[now].data[paths[i]];
					}
					std::cout << database[now].name << ":\n";
					return Ls(now,1);
				}
			};
		}
	}
