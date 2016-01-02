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
	BullDog(){ bInit = false; };
	~BullDog(){};

	void disconnectUpdateSignal(){
		updateConection.disconnect();
	}
	void watch(const fs::path &p, std::function<void()> callback) {
		if (!bInit) setUpdate();
		if(fs::exists(p)) list.emplace_back(last_write_time(p), p, callback);
		callback();
	}
	void watch(const std::vector<fs::path> &fileList, std::function<void()> callback){
		for (auto &f : fileList){
			watch(f, callback);
		}
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
	bool bInit;
	signals::Connection updateConection;
	void setUpdate(){
		updateConection = app::getWindow()->getSignalPostDraw().connect([=] {check(); });
		bInit = true;
	}
};
