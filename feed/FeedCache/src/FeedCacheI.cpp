#include <boost/lexical_cast.hpp>
#include "FeedCacheI.h"
#include "ServiceI.h"
#include "FeedDBAdapter.h"
#include "SSDFeedDBAdapter.h"
#include "PerformanceStat.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

#include "FeedCacheLogicReplicaAdapter.h"

#include<map>

using namespace xce::feed;

using namespace std;


void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedCacheI::instance(), service.createIdentity("M", ""));
  TaskManager::instance().scheduleRepeated(&PerformanceStat::instance());

  //---------------HA---------------//
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt(
      "FeedCache.Mod");
  int interval =
    service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(
        "FeedCache.Interval", 5); 
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerFeedCacheR",
      &FeedCacheI::instance(), mod, interval, new XceFeedChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);



}

//////////////////////////////////////////////////////////////////
void UserFeed::put(const FeedItem& item){

	MCE_DEBUG("UserFeed::put --> " << _end << " " << item.feed);
	if(_feeds[_end].feed == 0){
		_size = ((++_size>FEED_COUNT) ? FEED_COUNT : _size);
	}
	_feeds[_end] = item;
	_end = (_end + 1)%FEED_COUNT;

//  bool resortFlag = false;
//  {
//    IceUtil::Mutex::Lock lock(_mutex);
//    if (_size < FEED_COUNT) {
//      _feeds[_end] = item;
//      _end = (_end + 1) % FEED_COUNT;
//      _size++;
//
//      if (_size == FEED_COUNT) {
//        resortFlag = true;
//      }
//    } else if (_size == FEED_COUNT) {
//      _feeds[_end] = item;
//      _end = (_end + 1) % FEED_COUNT;
//      return;
//    }
//  }
//
//  MyUtil::LongSeq resortedIds;
//
//  if (resortFlag == true) {
//    FeedItemSeq seq;
//    for (int i = 0; i < FEED_COUNT; i++) {
//      seq.push_back(_feeds[i]);
//    }
//    try {
//      resortedIds = FeedCacheLogicAdapter::instance().reSortFeedItems(_userid,
//          seq);
//    } catch (Ice::Exception& e) {
//      MCE_WARN("FeedCacheLogicAdapter getEjectedFeedItems Failed!! " << _userid << " " << e);
//      if (_feeds[_end].feed == 0) {
//        _size = ((++_size > FEED_COUNT) ? FEED_COUNT : _size);
//      }
//      _feeds[_end] = item;
//      _end = (_end + 1) % FEED_COUNT;
//    }
//  }
//
//  {
//    IceUtil::Mutex::Lock lock(_mutex);
//    FeedItem tmpFeeds[FEED_COUNT];
//    set<long> id_set;
//    for (int i = 0; i < resortedIds.size(); i++) {
//      id_set.insert(resortedIds[i]);
//    }
//
//    int count = 0;
//    for (int i = 0; i < FEED_COUNT; i++) {
//      if (id_set.find(_feeds[i].feed) == id_set.end()) {
//        tmpFeeds[count] = _feeds[i];
//        count++;
//      }
//    }
//
//    _size = count;
//    if(_size>=1){
//    _end = (_size - 1) % FEED_COUNT;
//    }else{
//      _end =0;
//    }
//
//    for (int i = 0; i < _size; i++) {
//      _feeds[i] = tmpFeeds[i];
//    }
//
//  }

}

FeedItemSeq UserFeed::get(){
	FeedItemSeq seq;
	seq.reserve(_size);
	int tmp = FEED_COUNT + _end - 1;
	for(int c=0; c<_size&&tmp>=_end; tmp--){
		if(_feeds[tmp%FEED_COUNT].feed!=0){
			c++;
			seq.push_back(_feeds[tmp%FEED_COUNT]);
		}
	}
	//MCE_DEBUG("UserFeed::get --> " << _userid << " _size:" << _size << " end:" << _end << " beg:" << tmp%FEED_COUNT);
	return seq;
}

void UserFeed::remove(Ice::Long feedid){
	//MCE_DEBUG("UserFeed::remove --> " << _userid << " feedid:" << feedid << " _size:" << _size);
	if(feedid == 0){
		return;
	}
	FeedCacheConfigPtr cfg = FeedCacheI::instance().getConfig();
	bool tidy = (_deletedCount >= cfg->deletedCount);
	bool deleted = false;
	int begin = -1;
	int c = 0;
	for(int i=_end+FEED_COUNT-1; i>=_end&&c<_size; i--){
		if(_feeds[i%FEED_COUNT].feed > 0){
			c++;
		}
		if(_feeds[i%FEED_COUNT].feed == feedid){
			MCE_INFO("UserFeed::remove --> deleted " << _userid << " feedid:" << feedid << " size:" << _size << " index:" << (i%FEED_COUNT));
			_feeds[i%FEED_COUNT].feed = 0;
			deleted = true;
			if(!tidy){
				break;
			}
		}
		if(tidy && (begin==-1) && (_feeds[i%FEED_COUNT].feed==0)){
			MCE_INFO("UserFeed::remove --> tidy " << _userid << " size:" << _size << " index:" << (i%FEED_COUNT));
			begin = i;
		}
		if(begin>0 && _feeds[i%FEED_COUNT].feed > 0){
			_feeds[begin%FEED_COUNT] = _feeds[i%FEED_COUNT];
			_feeds[i%FEED_COUNT].feed = 0;
			begin--;
		}
	}
	if(deleted){
		_deletedCount++;
		_size--;
	}
	if(tidy){
		_deletedCount = 0;
	}
}



//////////////////////////////////////////////////////////////////
FeedItemSeq UserFeedPool::get(int userid){
	FeedCacheConfigPtr cfg = FeedCacheI::instance().getConfig();
	timeval beg, end;
	gettimeofday(&beg, NULL);
	if(cfg->enableCache){
		IceUtil::Mutex::Lock lock(_mutex);
		UserIdIndex& index = _users.get<1>();
		UserIdIndex::iterator it = index.find(userid);
		if(it != index.end()){
			SequenceIndex& seqIndex = _users.get<0>();
			seqIndex.relocate(seqIndex.begin(), _users.project<0>(it));
			gettimeofday(&end, NULL);
			float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
			timeuse/=1000;
			PerformanceStat::instance().incHitGet(timeuse);
			return (*it)->get();
		}
	}

	//可能被调用多次
	FeedItemSeq items;
	try{
		if(cfg->loadFrom == "FeedDB"){
			MCE_INFO("UserFeedPool::get --> load from FeedDB " << userid);
			items = FeedDBAdapter::instance().get(userid, 0, UserFeed::FEED_COUNT);
		}else if(cfg->loadFrom == "SSDFeedDB"){
			MCE_INFO("UserFeedPool::get --> load from SSDFeedDB " << userid);
			items = SSDFeedDBAdapter::instance().get(userid, 0, UserFeed::FEED_COUNT);
		}
	}catch(Ice::Exception& e){
		MCE_WARN("UserFeedPool::get --> load items from db err, " << userid << " " << e);
		return items;
	}
	
	if(items.empty()){
		return items;
	}
	
	{
		IceUtil::Mutex::Lock lock(_mutex);
		UserIdIndex& index = _users.get<1>();
		UserIdIndex::iterator it = index.find(userid);
		if(it != index.end()){
			return (*it)->get();
		}

		UserFeedPtr user = new UserFeed(userid);
		for(int i=(int)items.size()-1; i>=0; i--){
			user->put(items.at(i));
		}
		_users.push_front(user);
		int maxSize = cfg->cacheSize/FeedCacheI::POOL_SIZE;
		if((int)_users.size() > maxSize){
			while((int)_users.size() > maxSize){
				SequenceIndex& index = _users.get<0>();
				UserFeedPtr p = index.back();
				MCE_INFO("UserFeedPool::get --> evict " << p->userid() << " size:" << _users.size());
				index.pop_back();
				delete p;
			}
		}
		gettimeofday(&end, NULL);
		float timeuse=1000000*(end.tv_sec-beg.tv_sec)+end.tv_usec-beg.tv_usec;
		timeuse/=1000;
		PerformanceStat::instance().incLoadGet(timeuse);
		MCE_INFO("UserFeedPool::get --> create user " << userid << " " << items.size() << " cost:" << timeuse << "ms");	
		return items;
	}
}

void UserFeedPool::put(const FeedItem& item,
    const MyUtil::Int2IntMap& usersWeight) {
  //MCE_DEBUG("UserFeedPool::put --> " << item.feed << " size:" << usersWeight.size());

  {
    IceUtil::Mutex::Lock lock(_mutex);
    MyUtil::Int2IntMap::const_iterator it = usersWeight.begin();
    for (; it != usersWeight.end(); ++it) {
      UserIdIndex& index = _users.get<1> ();
      UserIdIndex::iterator uIt = index.find(it->first);
      if (uIt != index.end()) {
        const_cast<FeedItem&> (item).weight = it->second;
        (*uIt)->put(item);
      }
    }
  }

  MyUtil::Int2IntMap::const_iterator it = usersWeight.begin();
  for (; it != usersWeight.end(); ++it) {

    MyUtil::LongSeq resortedIds;
    FeedItemSeq seq;
    bool resortFlag = false;
    int uid = 0;

    {//mutex lock
      IceUtil::Mutex::Lock lock(_mutex);
      UserIdIndex& index = _users.get<1> ();
      UserIdIndex::iterator uIt = index.find(it->first);
      uid = it->first;

      if (uIt != index.end()) {
        if ((*uIt)->_size == FEED_COUNT) {
          for (int i = 0; i < FEED_COUNT; i++) {
            seq.push_back((*uIt)->_feeds[i]);
          }
        }
        resortFlag = true;
      }

    }//mutex release

    FeedItem tmpFeeds[FEED_COUNT];
    set<long> id_set;

    if (resortFlag == true) {

      try {
        resortedIds = FeedCacheLogicAdapter::instance().reSortFeedItems(uid,
            seq);
      } catch (Ice::Exception& e) {
        MCE_WARN("FeedCacheLogicAdapter reSortFeedItems Failed!! " << uid << " " << e);
        return;
      }

      for (int i = 0; i < resortedIds.size(); i++) {
        id_set.insert(resortedIds[i]);
      }

    }//end if resortFlag == true

    {//mutex lock

      if (resortFlag == true) {
        IceUtil::Mutex::Lock lock(_mutex);

        UserIdIndex& index = _users.get<1> ();
        UserIdIndex::iterator uIt = index.find(it->first);

        if (uIt != index.end()) {
          int count = 0;
          for (int i = 0; i < FEED_COUNT; i++) {
            if (id_set.find((*uIt)->_feeds[i].feed) == id_set.end()) {
              tmpFeeds[count] = (*uIt)->_feeds[i];
              count++;
            }
          }
          (*uIt)->_size = count;
          if ((*uIt)->_size >= 1) {
            (*uIt)->_end = ((*uIt)->_size - 1) % FEED_COUNT;
          } else {
            (*uIt)->_end = 0;
          }

          for (int i = 0; i < (*uIt)->_size; i++) {
            (*uIt)->_feeds[i] = tmpFeeds[i];
          }
        }

      }
    }//mutex release

  }//end for

}


void UserFeedPool::remove(int userid, Ice::Long feedid){
	//MCE_DEBUG("UserFeedPool::remove --> " << userid << " feed:" << feedid);
	IceUtil::Mutex::Lock lock(_mutex);
	UserIdIndex& index = _users.get<1>();
	UserIdIndex::iterator it = index.find(userid);
	if(it != index.end()){
		return (*it)->remove(feedid);
	}
}

void UserFeedPool::removeAll(int userid){
	IceUtil::Mutex::Lock lock(_mutex);
	UserIdIndex& index = _users.get<1>();
	UserIdIndex::iterator it = index.find(userid);
	if(it != index.end()){
		UserFeedPtr p = *it;
		index.erase(it);
		delete p; 
	}
}

int UserFeedPool::getUserFeedSize(int userid){
	{
		IceUtil::Mutex::Lock lock(_mutex);
		UserIdIndex& index = _users.get<1>();
		UserIdIndex::iterator it = index.find(userid);
		if(it != index.end()){
			return (*it)->size();
		}
	}
	return get(userid).size();	
}

pair<int, int> UserFeedPool::size(){
	IceUtil::Mutex::Lock lock(_mutex);
	pair<int, int> p = make_pair<int, int>(0, 0);
	p.first = _users.size();
	SequenceIndex& index = _users.get<0>();	
	SequenceIndex::iterator it = index.begin();
	for(; it!=index.end(); ++it){
		p.second += (*it)->size();
	}
	return p;
}

//////////////////////////////////////////////////////////////////
FeedItemSeq FeedCacheI::get(int userid, const Ice::Current&){
	//StatFunc stat("get stat --> ");
	return _pool[(userid>>7)%POOL_SIZE].get(userid);
}

int FeedCacheI::getCount(int userid, const Ice::Current&){
	return _pool[(userid>>7)%POOL_SIZE].getUserFeedSize(userid);
}

void FeedCacheI::put(const FeedItem& feed, const MyUtil::Int2IntMap& usersWeight, const Ice::Current&){
	//MCE_DEBUG("FeedCacheI::put --> " << feed.feed << " size:" << usersWeight.size());
	map<int, map<int, int> > splits;
	MyUtil::Int2IntMap::const_iterator it = usersWeight.begin();
	for(; it!=usersWeight.end(); ++it){
		splits[(it->first>>7)%POOL_SIZE][it->first] = it->second;
	}
	
	map<int, map<int, int> >::iterator sIt = splits.begin();
	for(; sIt!=splits.end(); ++sIt){
		_pool[sIt->first].put(feed, sIt->second);
	}

	FeedCacheConfigPtr cfg = getConfig();
	map<string, FeedDBConfigPtr>::iterator cIt = cfg->feedDBCfgs.begin();
	for(; cIt!=cfg->feedDBCfgs.end(); ++cIt){
		if(cIt->first == "FeedDB" && cIt->second->syncPut){
			MCE_INFO("FeedCacheI::put --> FeedDB feed:" << feed.feed << " actor:" << feed.actor 
				<< " stype:" << (feed.type&0xFFFF) << " size:" << usersWeight.size());
			try{
				FeedDBAdapter::instance().put(feed, usersWeight);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::put --> invoke FeeddB err, " << feed.feed << " "  << e);
			}
		}
		if(cIt->first == "SSDFeedDB" && cIt->second->syncPut){
			MCE_INFO("FeedCacheI::put --> SSDFeedDB feed:" << feed.feed << " actor:" << feed.actor 
				<< " stype:" << (feed.type&0xFFFF) << " size:" << usersWeight.size());
			try{
				SSDFeedDBAdapter::instance().put(feed, usersWeight);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::put --> invoke SSDFeeddB err, " << feed.feed << " " << e);
			}
		}
	}
}

void FeedCacheI::remove(int userid, Ice::Long feedid, const Ice::Current&){
	FeedCacheConfigPtr cfg = getConfig();
	map<string, FeedDBConfigPtr>::iterator it = cfg->feedDBCfgs.begin();
	for(; it!=cfg->feedDBCfgs.end(); ++it){
		if(it->first == "FeedDB" && it->second->syncRemove){
			MCE_INFO("FeedCacheI::remove --> FeedDB " << userid << " " << feedid);
			try{
				FeedDBAdapter::instance().remove(userid, feedid);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::remove --> invoke FeeddB err, " << feedid << " " << e);
			}

		}
		if(it->first == "SSDFeedDB" && it->second->syncRemove){
			MCE_INFO("FeedCacheI::remove --> SSDFeedDB " << userid << " " << feedid);
			try{
				SSDFeedDBAdapter::instance().remove(userid, feedid);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::remove --> invoke SSDFeeddB err, " << feedid << " " << e);
			}
		}
	}
	_pool[(userid>>7)%POOL_SIZE].remove(userid, feedid);
}

void FeedCacheI::removeAll(int userid, const Ice::Current&){
	FeedCacheConfigPtr cfg = getConfig();
	map<string, FeedDBConfigPtr>::iterator it = cfg->feedDBCfgs.begin();
	for(; it!=cfg->feedDBCfgs.end(); ++it){
		if(it->first == "FeedDB" && it->second->syncRemoveAll){
			MCE_INFO("FeedCacheI::removeAll --> FeedDB " << userid);
			try{
				FeedDBAdapter::instance().removeAll(userid);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::removeAll --> invoke FeeddB err, " << userid << " " << e);
			}
		}
		if(it->first == "SSDFeedDB" && it->second->syncRemoveAll){
			MCE_INFO("FeedCacheI::removeAll --> SSDFeedDB " << userid);
			try{
				SSDFeedDBAdapter::instance().removeAll(userid);
			}catch(Ice::Exception& e){
				MCE_WARN("FeedCacheI::removeAll --> invoke SSDFeeddB err, " << userid << " " << e);
			}
		}
	}
	_pool[(userid>>7)%POOL_SIZE].removeAll(userid);
}

pair<Ice::Long, Ice::Long> FeedCacheI::queryCacheCount(){
	pair<Ice::Long, Ice::Long> c;
	c.first = 0;
	c.second = 0;
	for(int i=0; i<POOL_SIZE; i++){
		pair<int, int> p = _pool[i].size();
		c.first += p.first;
		c.second += p.second;
	}
	return c;
}

/*void FeedCacheI::updateConfig(const MyUtil::Str2StrMap& cfgs, const Ice::Current&){
	map<string, string>::const_iterator it;
	if((it = cfgs.find("cache_size_cfg")) != cfgs.end()){
		int tmp = 0;
		try{
			tmp = boost::lexical_cast<int>(it->second);
		}catch(...){
		}
		if(tmp>1000000 && tmp<4000000){
			 FeedCacheConfig::instance().cacheSize(tmp);
		}
	}
	if((it = cfgs.find("deleted_count_cfg")) != cfgs.end()){
		int tmp = 0;
		try{
			tmp = boost::lexical_cast<int>(it->second);
		}catch(...){
		}
		FeedCacheConfig::instance().deletedCount(tmp);
	}
}*/

void FeedCacheI::updateConfig(const FeedCacheConfigPtr& cfg, const Ice::Current&){
	FeedCacheConfigPtr tmp = const_cast<FeedCacheConfigPtr&>(cfg);
	if(tmp->cacheSize < 0){
		tmp->cacheSize = _cfg->cacheSize;
	}
	if(tmp->deletedCount < 0){
		tmp->deletedCount = _cfg->deletedCount;
	}
	if(tmp->loadFrom.empty()){
		tmp->loadFrom = _cfg->loadFrom;
	}
	_cfg = tmp;
}

FeedCacheConfigPtr FeedCacheI::getConfig(const Ice::Current&){
	return _cfg;	
}
/*MyUtil::Str2StrMap FeedCacheI::getConfig(const Ice::Current&){
	MyUtil::Str2StrMap cfgs;
	cfgs["deleted_count_cfg"] = boost::lexical_cast<string>(FeedCacheConfig::instance().deletedCount());
	cfgs["cache_szie_cfg"] = boost::lexical_cast<string>(FeedCacheConfig::instance().cacheSize());
	return cfgs;	
}*/
