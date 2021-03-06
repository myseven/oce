#ifndef __USER_STATE_READER_ADAPTER_H__
#define __USER_STATE_READER_ADAPTER_H__

#include "UserBase.h"
#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "Markable.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userstate{

using namespace xce::userbase;
using namespace MyUtil;

const int CLUSTER = 100;

class UserStateInfo;

typedef IceUtil::Handle<UserStateInfo> UserStateInfoPtr;

class UserStateInfo : virtual public Ice::Object , public MyUtil::Markable {
public:
  UserStateInfo() : id_(0), state_(0), star_(0), level_(0) {}
  void parse(const UserStateDataPtr& data);
  int state() const {return state_;}
  int star() const {return star_;}
  int level() const {return level_;}
  void set_state(int state) {state_ = state;mark("STATE", boost::lexical_cast<std::string>(state));}
  void set_level(int level) {level_ = level;mark("LEVEL", boost::lexical_cast<std::string>(level));}
  void set_star(int star) {star_ = star;mark("STAR", boost::lexical_cast<std::string>(star));}
  int id() const {return id_;}
  void set_id(int id) {id_ = id; mark("ID", boost::lexical_cast<std::string>(id));}
  bool isSelected() const { return 0 != (state_ & (1 << CStateSELECTED));}
  bool isVipMember() const { return 0 != (state_ & (1 << CStateVIPMEMBER));}
  bool isMSNIMME() const { return 0 != (state_ & (1 << CStateMSNIMME));}
  bool isMobileBind() const { return 0 != (state_ & (1 << CStateMOBILEBIND));}
  bool isEverVipMember() const { return 0 != (state_ & (1 << CStateEVERVIPMEMBER));}
  bool isGuideDone() const { return 0 != (state_ & (1 << CStateGUIDEDONE));}
  bool isSocialGraphGood() const { return 0 != (state_ & (1 << CStateSOCIALGRAPHGOOD));}
  bool isKeepUse() const { return 0 != (state_ & (1 << CStateKEEPUSE));}
  bool isCharity() const { return 0 != (state_ & (1 << CStateCHARITY));}
  bool isCharityInvited() const { return 0 != (state_ & (1 << CStateCHARITYINVITED));}
  bool isDoubleAccount() const { return 0 != (state_ & (1 << CStateDOUBLEACCOUNT));}
  bool isHideAds() const { return 0 != (state_ & (1 << CStateHIDEADS));}
  bool isVest() const { return 0 != (state_ & (1 << CStateVEST));}
  bool isDisneyVIP() const {return 0 != (state_ & (1 << CStateDisneyVIP));}
  bool isGameManager() const { return 0 != (state_ & (1 << CStateGAMEMANAGER));}
  bool isSamePassword() const { return 0 != (state_ & (1 << CStateSAMEPASSWORD));}
  bool isCouplesSpace() const { return 0 != (state_ & (1 << CStateCOUPLESSPACE));}
  bool isPageManager() const { return 0 != (state_ & (1 << CStatePAGEMANAGER));}
  bool isApplySpammer() const { return 0 != (state_ & (1 << CStateAPPLYSPAMMER));}
private:
  int id_;
  int state_;
  int star_;
  int level_;
};


class UserStateReaderAdapter : public MyUtil::ReplicatedClusterAdapterI<UserStateReaderPrx>,
	public MyUtil::Singleton<UserStateReaderAdapter>
{
public:
	UserStateReaderAdapter() : MyUtil::ReplicatedClusterAdapterI<UserStateReaderPrx>("ControllerUserBaseReader",120,300,new MyUtil::OceChannel,true,"USM"){

	}

	void setStateOn(int id,int pos);
	void setStateOff(int id, int pos);
	UserStateInfoPtr getUserState(int id, const Ice::Context& ctx = Ice::Context());
	void setUserState(int id, const MyUtil::Str2StrMap& props);
	void setData(int id, const Ice::ObjectPtr& data);

	UserStateReaderPrx getUserStateReaderPrx(int id);

};

}
}
}

#endif

