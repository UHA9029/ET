#include <boost/bind.hpp>
#include <glog/logging.h>
#include "Rpc/RpcSession.h"
#include "Rpc/RpcServer.h"

namespace Egametang {

RpcSession::RpcSession(boost::asio::io_service& io_service, RpcServer& server):
		RpcCommunicator(io_service), rpc_server(server)
{
}

RpcSession::~RpcSession()
{
	RpcCommunicator::Stop();
}

void RpcSession::OnRecvMessage(RpcMetaPtr meta, StringPtr message)
{
	RpcSessionPtr session = shared_from_this();
	rpc_server.RunService(session, meta, message,
			boost::bind(&RpcSession::SendMeta, session, _1, _2));

	// 可以循环利用
	RecvMeta(meta, message);
}

void RpcSession::OnSendMessage(RpcMetaPtr meta, StringPtr message)
{
}

void RpcSession::Start()
{
	RpcMetaPtr meta(new RpcMeta());
	StringPtr message(new std::string);
	RecvMeta(meta, message);
}

void RpcSession::Stop()
{
	VLOG(2) << __FUNCTION__;
	RpcSessionPtr session = shared_from_this();
	rpc_server.Remove(session);
}

} // namespace Egametang
