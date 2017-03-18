#pragma once
#include "Net\asio.hpp"
#include "Memory\MemoryPoolMan.hpp"

class OutPacket;
class InPacket;

class SocketBase : public std::enable_shared_from_this<SocketBase>
{
private:
	static unsigned int SocketCount;
	unsigned int nSocketID;

	asio::ip::tcp::socket mSocket;
	unsigned char* aRecvIV, *aSendIV;
	std::unique_ptr<unsigned char, ArenaUniquePtrDeleter> aRecivedPacket;

	bool bIsLocalServer = false;

	void(*OnNotifySocketDisconnected)(SocketBase *pSocket);

	void EncodeHandShakeInfo(OutPacket *oPacket);

	//�Ω�����ʥ]��Buffer�귽 (�Ӹ귽�D�g��Arena�t�m)
	void OnSendPacketFinished(const std::error_code &ec, std::size_t bytes_transferred, unsigned char *buffer);
	void OnReceive(const std::error_code &ec, std::size_t bytes_transferred);
	void ProcessPacket(const std::error_code &ec, std::size_t bytes_transferred);

protected:
	void OnWaitingPacket();
	virtual void OnClosed() = 0;

public:
	SocketBase(asio::io_service& serverService, bool isLocalServer = false);
	virtual ~SocketBase();

	void SetDisconnectedNotifyFunc(void(*FUNC)(SocketBase *))
	{
		OnNotifySocketDisconnected = FUNC;
	}

	unsigned int GetSocketID() const;
	void Init();
	void SendPacket(OutPacket *iPacket, bool handShakePacket = false);
	void OnDisconnect();
	virtual void OnPacket(InPacket *iPacket) = 0;

	asio::ip::tcp::socket& GetSocket();
};
