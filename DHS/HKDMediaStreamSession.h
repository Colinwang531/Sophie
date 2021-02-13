//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-06-15
//		Description :					�������Ự��
//
//		History:						Author									Date										Description
//										������									2020-05-13									����
//

#include "CH-HCNetSDKV6.1.4.17_build20200331_Linux64/HCNetSDK.h"
#include "Session/TCPStreamTargetSession.h"
using TCPStreamTargetSession = base::network::TCPStreamTargetSession;

class HKDMediaStreamSession : public TCPStreamTargetSession

{
public:
	HKDMediaStreamSession(
		const std::string url,
		const int uid = -1,
		boost::asio::ip::tcp::socket* s = nullptr);
	~HKDMediaStreamSession(void);

public:
	int startSession(void) override;
	int stopSession(void) override;
	
protected:
	void receivedDataNotification(const unsigned char* data = nullptr, const int bytes = 0) override;

private:
	int sendURLFrameData(const std::string url);
	static void CALLBACK afterGotRealplayDataNotificationCallback(
		LONG sid = -1, DWORD type = 0, BYTE* data = nullptr, DWORD bytes = 0, void* ctx = nullptr);

private:
	const std::string streamURL;
	const int userID;
	int streamID;
	long long frameSequence;
	//��������ͷ��Ϣ���ݲ����25֡����һ��
	BYTE* frameSysHead;
	DWORD frameSysHeadBytes;
};//class HKDMediaStreamSession

