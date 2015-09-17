#include "cinder/Filesystem.h"
using namespace std;
using namespace ci;

#pragma once

struct wFile
{
	time_t time;
	fs::path path;
	std::function<void()> callback;
	wFile(time_t t, fs::path p, std::function<void()> c) {
		time = t;
		path = p;
		callback = c;
	}
};
class BullDog
{
public:
	BullDog(){};
	~BullDog(){};
	void watch(fs::path p, std::function<void()> callback) {
		if(fs::exists(p))list.emplace_back(last_write_time(p), p, callback);
		callback();
	}

	void check(){
		list.erase(remove_if(list.begin(), list.end(), [&](wFile w){ return !fs::exists(w.path);}), list.end());
		for (auto &item : list){
			auto lw = last_write_time(item.path);

			if (lw > 0 && lw != item.time){
				item.callback();
				item.time = lw;
			}	
		}
	}
private:
	vector<wFile> list;
};


