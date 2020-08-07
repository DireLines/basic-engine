#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H
//This code is flawed and is not being used. I'm just committing it so I can go back and fix it later
//TODO: make this work or steal somebody else's implementation and replace class specific pools with the generic one
using namespace std;
#include <vector>
#include <memory>

#include <iostream>
template <class Obj_T>
class ObjectPool {
public:
	Obj_T* create() {
		for (int i = 0; i < pool.size(); ++i) {
			if (!inUse[i]) {
				inUse[i] = true;
				return pool[i];
			}
		}
		Obj_T new_obj;
		pool.push_back(&new_obj);
		inUse.push_back(true);
		return &new_obj;
	}
	void release(Obj_T* ptr) {
		for (int i = 0; i < pool.size(); ++i) {
			if (pool[i] == ptr) {
				release(i);
			}
		}
	}
private:
	void release(int index) {
		inUse[index] = false;
	}
	vector<Obj_T*> pool;
	vector<bool> inUse;
};

#endif